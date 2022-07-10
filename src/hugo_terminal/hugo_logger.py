import logging

REMOTE_CRITICAL = 51
REMOTE_ERROR    = 41
REMOTE_WARNING  = 31
REMOTE_INFO     = 21
REMOTE_DEBUG    = 11

# class MyLogger(logging.getLoggerClass()):
#     def __init__(self, name, level=logging.NOTSET):
#         super().__init__(name, level)

#         logging.addLevelName(REMOTE_INFO, "REMOTE_INFO")

#     def _log()
#     def remote_info(self, msg, *args, **kwargs):
#         if self.isEnabledFor(REMOTE_INFO):
#             self._log(REMOTE_INFO, msg, args, **kwargs)

class HugoLogger(logging.Handler):
  logger = logging.getLogger('Hugo')
  logging.addLevelName(REMOTE_CRITICAL, "=CRITICAL")
  logging.addLevelName(REMOTE_ERROR, "=ERROR")
  logging.addLevelName(REMOTE_WARNING, "=WARNING")
  logging.addLevelName(REMOTE_INFO, "=INFO")
  logging.addLevelName(REMOTE_DEBUG, "=DEBUG")

  #def remote_info(self, message, *args, **kws):
  #  print(REMOTE_INFO, message, args, **kws)

  def __init__(self, verbose):
    logging.Handler.__init__(self)
    self.widget = None
    self.logger.setLevel(logging.DEBUG)
    self.setFormatter(logging.Formatter("%(levelname)-8s%(message)s"))
    self.setLevel(logging.DEBUG if verbose else logging.INFO)
    self.logger.addHandler(self)

    #logging.addLevelName(self.REMOTE_CRITICAL, "=CRITICAL")
    #logging.addLevelName(self.REMOTE_ERROR, "=ERROR")
    #logging.addLevelName(self.REMOTE_WARNING, "=WARNING")
    #addLevelName(self.REMOTE_INFO, "AAA")
    #logging.addLevelName(self.REMOTE_DEBUG, "=DEBUG")

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

  def add_widget(self, widget):
    self.widget = widget

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
          "=DEBUG": "bold_white"
        }
    ))
    #stream_handler.setLevel(log_level)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    #logger.setLevel(log_level)



  def emit(self, record):
    if self.widget:
      self.widget.add_log(self.format(record))
