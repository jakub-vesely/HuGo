
import time
import logging
from colorlog import ColoredFormatter
import hashlib
import os
import subprocess
from terminal_kivy.ble import Ble

class Commands:
  SHELL_VERSION               = 0x80
  SHELL_STOP_PROGRAM          = 0x81
  SHELL_START_PROGRAM         = 0x82
  SHELL_GET_NEXT_FILE_INFO    = 0x83
  SHELL_REMOVE_FILE           = 0x84
  SHELL_HANDLE_FILE           = 0x85
  SHELL_GET_FILE_CHECKSUM     = 0x86
  SHELL_APPEND                = 0x87
  SHELL_MK_DIR                = 0x88

class Terminal():
  output_dir_prefix = ".build"
  def __init__(self, hook_keyboard, mac_address, verbose, source_dir):
    self.flashing_folder = source_dir
    self.output_dir = self.output_dir_prefix + "_" + source_dir.replace("/", "_").replace("\\", "_")
    self._init_logger(logging.DEBUG if verbose else logging.INFO)
    self.ble = Ble(hook_keyboard, mac_address)

  def _init_logger(self, log_level):
    REMOTE_CRITICAL = 51
    REMOTE_ERROR    = 41
    REMOTE_WARNING  = 31
    REMOTE_INFO     = 21
    REMOTE_DEBUG    = 11

    logging.addLevelName(REMOTE_CRITICAL, "=CRITICAL")
    logging.addLevelName(REMOTE_ERROR, "=ERROR")
    logging.addLevelName(REMOTE_WARNING, "=WARNING")
    logging.addLevelName(REMOTE_INFO, "=INFO")
    logging.addLevelName(REMOTE_DEBUG, "=DEBUG")

    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(ColoredFormatter(
        "%(log_color)s%(levelname)-9s%(reset)s%(message)s",
        log_colors={
          "CRITICAL": "red,bg_white",
          "ERROR": "red",
          "WARNING": "yellow",
          "INFO": "green",
          "DEBUG": "white",
          "=CRITICAL": "bold_red,bg_white",
          "=ERROR": "bold_red",
          "=WARNING": "bold_yellow",
          "=INFO": "bold_green",
          "=DEBUG": "bold_white"
        }
    ))
    stream_handler.setLevel(log_level)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    logger.setLevel(log_level)

  def connect(self):
    if not self.ble.connect():
      logging.error("not connected")
      return False
    return True

  def disconnect(self):
    self.ble.disconnect()
    while self.ble.connected:
      time.sleep(0.1)

  def stop_program(self):
    logging.info("terminating_program...")
    self.ble.command(Commands.SHELL_STOP_PROGRAM, b"") #terminate
    try:
      self.ble.disconnect()
    except Exception:
      pass
    if not self.connect():
      return False
    logging.info("terminating_program DONE")
    return True

  def start_program(self):
    logging.info("starting program...")
    ret_val = self.ble.command(Commands.SHELL_START_PROGRAM, b"") #start program
    if ret_val == b"\1":
      logging.info("starting program DONE")
    else:
      logging.warning("program terminated!")

  def _get_remote_files(self):
    remote_files = {}
    logging.debug("getting remote files...")
    while True:
      self.ble.notification_data = None
      file_info = self.ble.command(Commands.SHELL_GET_NEXT_FILE_INFO, b"")

      if file_info == b"\0":
        break
      remote_files[file_info[20:].decode("utf-8")] = file_info[:20].hex()
    logging.debug("getting remote files Done")
    return remote_files

  def _get_local_files(self, directory):
    local_files = {}
    dir_content = os.listdir(directory)
    for file_name in dir_content:
      file_path = directory + "/" + file_name
      if os.path.isdir(file_path):
        local_files.update(self._get_local_files(file_path))
      else:
        with open(file_path, "rb") as file:
          data = file.read()
          local_files[file_path[len(self.output_dir):]] = hashlib.sha1(data).digest().hex()
    return local_files

  def _remove_unnecessary_files(self, remote_files, local_files):
    logging.debug("removing unnecessary files ...")
    removed = False
    for file_path in remote_files:
      self.ble.notification_data = None
      if file_path not in local_files:
        logging.info("removing file %s", file_path)
        self.ble.command(Commands.SHELL_REMOVE_FILE, file_path.encode("utf-8"))
        removed = True
    logging.debug("removing unnecessary files Done")
    return removed

  def _import_files(self, remote_files, local_files):
    logging.debug("importing files ...")
    imported = False
    for file_path in local_files:
      self.ble.notification_data = None
      if file_path not in remote_files or remote_files[file_path] != local_files[file_path]:
        imported = True
        logging.info("uploading file %s", file_path)

        path_elements = file_path[1:].split("/") #file  path start by /
        if len(path_elements) > 1:
          path_elements = path_elements[:-1]
          path = ""
          for element in path_elements:
            path += "/" + element
            self.ble.command(Commands.SHELL_MK_DIR, path.encode("utf-8"))

        with open(self.output_dir + "/" + file_path, "rb") as f:
          data = f.read()
          self.ble.notification_data = None
          self.ble.command(Commands.SHELL_HANDLE_FILE, file_path.encode("utf-8"))
          step = self.ble.ble_message_max_size - 1 # - command header
          for i in range(0, len(data), step):
            self.ble.command(Commands.SHELL_APPEND, data[i: i + step])
        stored_file_checksum = self.ble.command(Commands.SHELL_GET_FILE_CHECKSUM, b"")
        if stored_file_checksum.hex() != local_files[file_path]:
          logging.error("file %s has not been stored properly: %s", file_path, str((stored_file_checksum.hex(), local_files[file_path])))
    logging.debug("importing files Done")
    return imported

  def _remove_old_dir_content(self, in_dir_content, out_dir_path):
    dir_content = os.listdir(out_dir_path)
    for out_file_name in dir_content:
      unbuild_name = out_file_name.replace(".mpy", ".py")
      if unbuild_name not in in_dir_content:
        out_file_path = out_dir_path + "/" + out_file_name
        if os.path.isdir(out_file_path):
          self._remove_old_dir_content([], out_file_path)
          os.rmdir(out_file_path)
        else:
          os.remove(out_file_path)

  def _update_required(self, in_path, out_path):
    if not os.path.exists(out_path):
      return True
    return os.path.getmtime(in_path) > os.path.getmtime(out_path)

  def _build_local_dir(self, in_path, out_path):
    in_dir_content = os.listdir(in_path)

    if not os.path.exists(out_path):
      os.mkdir(out_path)
    else:
      self._remove_old_dir_content(in_dir_content, out_path)

    for file_name in in_dir_content:
      if file_name in ("__init__.py"):
        continue

      in_file_path = in_path + "/" + file_name
      if os.path.isdir(in_file_path):
        if file_name != "__pycache__":
          self._build_local_dir(in_file_path, out_path + "/" + file_name)
        continue

      if file_name == "boot.py" or not file_name.endswith(".py"):
        if self._update_required(in_file_path, self.output_dir + "/" + file_name):
          os.system(f"cp {in_file_path} { self.output_dir}")
      else:
        out_file_path = out_path + "/" + file_name.split(".")[0] + ".mpy"
        if self._update_required(in_file_path, out_file_path):
          try:
            subprocess.run(f"../micropython/mpy-cross/mpy-cross {in_file_path} -o {out_file_path}", check=True, shell=True, stderr=subprocess.PIPE)
          except subprocess.CalledProcessError as error:
            logging.warning("build of '%s' unsuccessful: %s", str(in_file_path), error.stderr.decode("utf-8"))
            raise SyntaxError


  def _process_files(self):
    remote_files = self._get_remote_files()

    logging.debug("building local files...")
    self._build_local_dir(self.flashing_folder, self.output_dir)
    logging.debug("building local files Done")

    local_files = self._get_local_files(self.output_dir)
    changed = self._remove_unnecessary_files(remote_files, local_files)
    changed |= self._import_files(remote_files, local_files)
    return changed

  def flash_files(self):
    logging.info("Uploading...")
    timestamp = time.time()
    try:
      flashed = self._process_files()
    except SyntaxError:
      return False

    if flashed:
      logging.info("uploading time: %d s", time.time() - timestamp)

    if flashed:
      logging.info("Uploading DONE")
    else:
      logging.info("All files are up-to-date")
    return True

  def monitor(self):
    logging.info("Monitoring...")
    self.ble.monitor()
    logging.info("Monitoring Terminated")

  def run(self, args):
    if not self.connect():
      return False

    if not self.stop_program():
      return False

    if args.flash:
      if not self.flash_files():
        return False

      if args.extra_reboot:
        if not self.stop_program():
          return False

    self.start_program()

    if args.monitor:
      self.monitor()

    self.disconnect()
    return True
