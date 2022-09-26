import logging

REMOTE_CRITICAL = 51
REMOTE_ERROR    = 41
REMOTE_WARNING  = 31
REMOTE_INFO     = 21
REMOTE_VALUE    = 22
REMOTE_DEBUG    = 11

class HugoLogger(logging.Handler):
  logger = logging.getLogger('Hugo')
  logging.addLevelName(REMOTE_CRITICAL, "=CRITICAL")
  logging.addLevelName(REMOTE_ERROR, "=ERROR")
  logging.addLevelName(REMOTE_WARNING, "=WARNING")
  logging.addLevelName(REMOTE_INFO, "=INFO")
  logging.addLevelName(REMOTE_VALUE, "=VALUE")
  logging.addLevelName(REMOTE_DEBUG, "=DEBUG")

  def __init__(self, verbose):
    logging.Handler.__init__(self)
    self.gui = None
    self.logger.setLevel(logging.DEBUG if verbose else logging.INFO)
    self.setFormatter(logging.Formatter("%(levelname)-8s%(message)s"))
    self.setLevel(logging.DEBUG if verbose else logging.INFO)
    self.logger.addHandler(self)

  def debug(self, *args):
    self.logger.debug(*args)

  def info(self, *args):
    self.logger.info(*args)

  def warning(self, *args):
    self.logger.warning(*args)

  def error(self, *args):
    self.logger.error(*args)

  def critical(self, *args):
    self.logger.critical(*args)

  def log(self, *args):
    self.logger.log(*args)

  def set_gui(self, gui):
    self.gui = gui

  def add_colorlog(self):
    from colorlog import ColoredFormatter
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
          "=VALUE": "bold_green",
          "=DEBUG": "bold_white"
        }
    ))
    #stream_handler.setLevel(log_level)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    #logger.setLevel(log_level)



  def emit(self, record):
    if self.gui:
      self.gui.add_log(self.format(record))
