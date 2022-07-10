#from kivy import platform
from kivy.app import App
from kivy.core.window import Window
from kivy.uix.label import Label
from kivy.uix.bubble import Button
from kivy.uix.boxlayout import BoxLayout
#from kivy.uix.popup import Popup
from kivy.uix.rst import RstDocument

class LogWidget(RstDocument):
  def add_log(self, message:str):
    #print(message)
    if not message:
      return

    elements = message.split()

    color_str = ""
    if elements[0] == "DEBUG":
      color_str = "[color=9999aa]"
    elif elements[0] == "INFO":
      color_str = "[color=33aa33]"
    elif elements[0] == "WARNING":
      color_str = "[color=aaaa33]"
    elif elements[0] == "ERROR":
      color_str = "[color=aa3333]"
    elif elements[0] == "CRITICAL":
      color_str = "[b][u][color=aa3333]"

    if elements[0] == "=DEBUG":
      color_str = "[color=dddddd]"
    elif elements[0] == "=INFO":
     color_str = "[color=33ff33]"
    elif elements[0] == "=WARNING":
      color_str = "[color=ffff33]"
    elif elements[0] == "=ERROR":
      color_str = "[color=ff3333]"
    elif elements[0] == "=CRITICAL":
     color_str = "[color=ff0000]"

    elements[0] = color_str + elements[0] + "[/color]"

    if len (elements) > 1 and elements[1].endswith(":"):
    #   if elements[1] != ":":
    #     return
      elements[1] = "[color=33ffff][ " + elements[1][:-1] + " ]"
    else:
       elements[1] = "[color=eeeeee]" + elements[1]

    elements.insert(2, "[color=eeeeee]")
    modified = " ".join(elements)
    self.text += modified + "\n\n"

class KeyButton(Button):
  def __init__(self, key_value, **kwargs):
    super().__init__(**kwargs)
    self.key_value = key_value
    self.default_color = kwargs["background_color"]
    self.border = (1.3, 1.3, 1.3, 1.3)
    self.font_name = "NotoSerifCJK-Regular.ttc"

  def _do_press(self):
    new_color = []
    for i in range(len(self.background_color)):
      new_color.append(self.background_color[i] * 0.8)
    self.background_color =  new_color

  def _do_release(self, *args):
    self.background_color = self.default_color

class Gui(App):
  def __init__(self, terminal, logger):
    super().__init__()
    self.terminal = terminal
    self.running = True
    self.label = None
    self.logger = logger

    # if (platform != 'android'):
    #   Window.size = (500, 1000)

    self.horizontal_layout =  Window.size[0] > Window.size[1]
    self.log_view = None
    Window.clearcolor = (0.2, 0.2, 0.2, 1)
    Window.bind(on_request_close=self.on_request_close)

  def on_request_close(self, *args):
    raise KeyboardInterrupt
    #return True

  def dummy_callback(self, *args):
    pass
  def white_button(self, text, key_value, on_press=None, on_release=None):
    return KeyButton(
          text=text,
          key_value=key_value,
          on_press=on_press if on_press else self.dummy_callback,
          on_release=on_release if on_release else self.dummy_callback,
          color=(0.1,0.1,0.1, 1),
          background_color=(2.7, 2.7, 2.7, 1),
          font_size='48sp'
    )

  def blue_button(self, text, key_value, on_press=None, on_release=None):
    return KeyButton(
          text = text,
          key_value=key_value,
          on_press=on_press if on_press else self.dummy_callback,
          on_release=on_release if on_release else self.dummy_callback,
          color=(0.1,0.1,0.1, 1),
          background_color=(0.5, 0.5, 3, 1),
          font_size='48sp'
      )

  def red_button(self, text, key_value, on_press=None, on_release=None):
    return KeyButton(
          text = text,
          key_value=key_value,
          on_press=on_press if on_press else self.dummy_callback,
          on_release=on_release if on_release else self.dummy_callback,
          color=(0.1,0.1,0.1, 1),
          background_color=(3, 0.5, 0.5, 1),
          font_size='48sp'
    )

  def common_button(self, text, key_value, on_press=None, on_release=None):
    return KeyButton(
        text = text,
        key_value=key_value,
        on_press=on_press if on_press else self.dummy_callback,
        on_release=on_release if on_release else self.dummy_callback,
        color=(0.1,0.1,0.1, 1),
        background_color=(2.7, 2.7, 2.7, 1),
        font_size='24sp'
    )

  def build_keyboard_row(self, main_layout, widgets):
    line_layout = BoxLayout(orientation="horizontal")
    main_layout.add_widget(line_layout)
    for widget in widgets:
        line_layout.add_widget(widget)

  def press(self, instance):
    #self.logger.info(f"pressed {instance.key_value}")
    self.terminal.key_press(instance.key_value)
    #instance.background_color = [3, 0.82, 0.82, 1]

  def release(self, instance):
    pass
    #instance.background_color = instance.default_color # (3, 3, 3, 1)

  def build_keyboard(self, main_layout):
      self.build_keyboard_row(
              main_layout, (
                      self.white_button(text="①", key_value="1", on_press=self.press, on_release=self.release),
                      self.white_button(text="②", key_value="2", on_press=self.press, on_release=self.release),
                      self.white_button(text="③", key_value="3", on_press=self.press, on_release=self.release)
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      self.white_button(text="④", key_value="4", on_press=self.press, on_release=self.release),
                      self.white_button(text="⑤", key_value="5", on_press=self.press, on_release=self.release),
                      self.white_button(text="⑥", key_value="6", on_press=self.press, on_release=self.release)
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      self.white_button(text="⑦", key_value="7", on_press=self.press, on_release=self.release),
                      self.white_button(text="⑧", key_value="8", on_press=self.press, on_release=self.release),
                      self.white_button(text="⑨", key_value="9", on_press=self.press, on_release=self.release)
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      self.red_button(text="*", key_value="*", on_press=self.press, on_release=self.release),
                      self.white_button(text="⓪", key_value="0", on_press=self.press, on_release=self.release),
                      self.red_button(text="#", key_value="#", on_press=self.press, on_release=self.release)
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      Label(text=""),
                      self.blue_button(text="▲", key_value="up", on_press=self.press, on_release=self.release),
                      Label(text="")
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      self.blue_button(text="◀", key_value="left", on_press=self.press, on_release=self.release),
                      self.red_button(text="●", key_value="enter", on_press=self.press, on_release=self.release),
                      self.blue_button(text="▶", key_value="right", on_press=self.press, on_release=self.release)
              )
      )
      self.build_keyboard_row(
              main_layout, (
                      Label(text=""),
                      self.blue_button(text="▼", key_value="down", on_press=self.press, on_release=self.release),
                      Label(text="")
              )
      )

  # def monitor(self, *args):
  #   self.terminal.connect()
  # def flash(self, *args):
  #   # popup = Popup(title='Test popup',
  #   #       content=Label(text='Hello world'),
  #   #     size_hint=(None, None), size=(400, 400)
  #   # )
  #   # popup.open()

  #   self.terminal.flash()

  # def build_taskbar(self, main_layout):
  #   main_layout.add_widget(self.common_button(text="connect", key_value="connect", on_press=self.monitor))
  #   main_layout.add_widget(self.common_button(text="flash", key_value="flash", on_press=self.flash))

  def build(self):
    general_layout = BoxLayout(orientation="vertical")

    #taskbar_layout = BoxLayout(orientation="horizontal", size_hint_y=0.07)
    #general_layout.add_widget(taskbar_layout)
    #self.build_taskbar(taskbar_layout)

    main_layout = BoxLayout(orientation="horizontal" if self.horizontal_layout else "vertical")
    general_layout.add_widget(main_layout)

    keyboard_layout = BoxLayout(orientation="vertical", size_hint_y=1 if self.horizontal_layout else 0.8)
    main_layout.add_widget(keyboard_layout)
    self.build_keyboard(keyboard_layout)

    self.log_view = LogWidget(text="",size_hint_y=(1 if self.horizontal_layout else 0.3))
    main_layout.add_widget(self.log_view)

    temp = self.log_view.colors["background"]
    self.log_view.colors["background"] = self.log_view.colors["paragraph"]
    self.log_view.colors["paragraph"] = temp
    self.logger.add_widget(self.log_view)

    return general_layout


    # def on_stop(self):
    #   self.running = False

    # async def example(self):
    #     while self.running:
    #         try:
    #             self.line("scanning")
    #             scanned_devices = await bleak.BleakScanner.discover(1)
    #             self.line("scanned", True)

    #             if len(scanned_devices) == 0:
    #                 raise bleak.exc.BleakError("no devices found")

    #             scanned_devices.sort(key=lambda device: -device.rssi)

    #             for device in scanned_devices:
    #                 self.line(f"{device.name} {device.rssi}dB")

    #             for device in scanned_devices:
    #                 self.line(f"Connecting to {device.name} ...")
    #                 try:
    #                     async with bleak.BleakClient(device) as client:
    #                         services = await client.get_services()
    #                         for service in services.services.values():
    #                             self.line(f"  service {service.uuid}")
    #                             for characteristic in service.characteristics:
    #                                 self.line(
    #                                     f"  characteristic {characteristic.uuid} {hex(characteristic.handle)} ({len(characteristic.descriptors)} descriptors)"
    #                                 )
    #                 except (EOFError, bleak.exc.BleakError, asyncio.exceptions.TimeoutError) as e:
    #                     self.line(f"  error {e}")
    #                     asyncio.sleep(10)
    #         except bleak.exc.BleakError as e:
    #             self.line(f"ERROR {e}")
    #             await asyncio.sleep(1)
    #     self.line("example loop terminated", True)
