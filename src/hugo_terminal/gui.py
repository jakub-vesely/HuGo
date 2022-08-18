from kivy import platform
from kivy.app import App
from kivy.core.window import Window
from kivy.uix.label import Label
from kivy.uix.bubble import Button
from kivy.uix.boxlayout import BoxLayout
#from kivy.uix.popup import Popup
from kivy.uix.rst import RstDocument
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.uix.treeview import TreeView, TreeViewLabel, TreeViewNode

class LogWidget(RstDocument):
  def __init__(self, **kwargs):
    super().__init__(**kwargs)
    self.lines = 0
    self.remote_values = None

  def add_log(self, level:str, message:str):
    if not message:
      return

    elements = message.split(" ")
    color_str = ""
    if level == "DEBUG":
      color_str = "[color=9999aa]"
    elif level == "INFO":
      color_str = "[color=33aa33]"
    elif level == "WARNING":
      color_str = "[color=aaaa33]"
    elif level == "ERROR":
      color_str = "[color=aa3333]"
    elif level == "CRITICAL":
      color_str = "[b][u][color=aa3333]"

    if level == "=DEBUG":
      color_str = "[color=dddddd]"
    elif level == "=INFO":
     color_str = "[color=33ff33]"
    elif level == "=WARNING":
      color_str = "[color=ffff33]"
    elif level == "=ERROR":
      color_str = "[color=ff3333]"
    elif level == "=CRITICAL":
     color_str = "[color=ff0000]"

    elements.insert(0, color_str + level + "[/color]")

    if len (elements) > 1 and elements[1].endswith(":"):
      elements[1] = "[color=33ffff][ " + elements[1][:-1] + " ]"
    else:
       elements[1] = "[color=eeeeee]" + elements[1]

    elements.insert(2, "[color=eeeeee]")
    modified = " ".join(elements)
    if self.lines > 100:
      self.text = self.text[self.text.find("\n\n") + 2:]
    else:
      self.lines += 1
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
  platform_android = 'android'
  def __init__(self, terminal, logger):
    super().__init__()
    self.terminal = terminal
    self.running = True
    self.label = None
    self.logger = logger
    self.logger.set_gui(self)
    self.screen_manager = None
    self.move_left = False
    self.move_right = False

    if (platform != self.platform_android):
      Window.size = (1200, 600)

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

  def _on_touch_move(self, touch):
    if touch.ox - touch.x > 100 and abs(touch.oy - touch.y) < 50:
      self.move_left = True

    if touch.x - touch.ox > 100 and abs(touch.oy - touch.y) < 50:
      self.move_right = True


  def _on_touch_up(self, touch):
    if self.move_left:
      self.move_left = False
      self._go_to_next_screen()

    if self.move_right:
      self.move_right = False
      self._go_to_prev_screen()

  def _go_to_prev_screen(self, *args):
    self.screen_manager.transition.direction = 'right'
    self.screen_manager.current = self.screen_manager.previous()

  def _go_to_next_screen(self, *args):
      self.screen_manager.transition.direction = 'left'
      self.screen_manager.current = self.screen_manager.next()

  def build_navigation_bar(self, layout):
    navigation_layout = BoxLayout(orientation="horizontal", size_hint_y=0.07)

    navigation_layout.add_widget(self.common_button(text="<", key_value="<", on_press=self._go_to_prev_screen))
    navigation_layout.add_widget(self.common_button(text=">", key_value=">", on_press=self._go_to_next_screen))
    layout.add_widget(navigation_layout)

  def build_remote_value_screen(self, remote_value_screen:Screen):
    self.remote_values = TreeView(root_options={'text': 'Remote Values'})
    remote_value_screen.add_widget(self.remote_values)

  def build(self):
    main_layout = BoxLayout(orientation="vertical")

    if platform == self.platform_android:
      self.build_navigation_bar(main_layout)
      self.screen_manager = ScreenManager()
      self.screen_manager.on_touch_move = self._on_touch_move
      self.screen_manager.on_touch_up = self._on_touch_up
    else:
      self.screen_manager = BoxLayout(orientation="horizontal")
    main_layout.add_widget(self.screen_manager)

    log_screen = Screen(name="log")

    self.log_view = LogWidget(text="") #,size_hint_y=(1 if self.horizontal_layout else 0.3))
    log_screen.add_widget(self.log_view)
    self.screen_manager.add_widget(log_screen)

    keyboard_screen = Screen(name="keyboard")
    keyboard_layout = BoxLayout(orientation="vertical")#, size_hint_y=1 if self.horizontal_layout else 0.8)
    keyboard_screen.add_widget(keyboard_layout)
    self.build_keyboard(keyboard_layout)
    self.screen_manager.add_widget(keyboard_screen)

    remote_value_screen = Screen(name="remote_values")
    self.build_remote_value_screen(remote_value_screen)
    self.screen_manager.add_widget(remote_value_screen)

    temp = self.log_view.colors["background"]
    self.log_view.colors["background"] = self.log_view.colors["paragraph"]
    self.log_view.colors["paragraph"] = temp

    return main_layout

  def _process_tree_view_element(self, node:TreeViewNode, info_path_elements, info_path_value):
    separator = ": "
    if not isinstance(node, TreeViewLabel):
      self.logger.error("node is not TreeViewLabel")
      return False

    for child in node.nodes:
      if not isinstance(child, TreeViewLabel):
        self.logger.error("node is not TreeViewLabel")
        return False

      if (len(info_path_elements) > 1 and child.text == info_path_elements[0]) or child.text.startswith(info_path_elements[0] + separator):
          return self._process_tree_view_element(child, info_path_elements[1:], info_path_value) #done in child
        # else:

        #   if len(child.nodes) > 0 and isinstance(child.nodes[0], TreeViewLabel):
        #     child.nodes[0].text = info_path_value
        #     return True # done
        #   else:
        #     self.logger.error("whole path has been found but no value node is present")
        #     return False
    #not found
    if len(info_path_elements) == 0:
      node.text = node.text.split(separator)[0] + separator + info_path_value
    else:
      if len(info_path_elements) == 1:
        text = info_path_elements[0] + separator
      else:
        text = info_path_elements[0]
      child = self.remote_values.add_node(TreeViewLabel(text=text, is_open=True), node)

      return self._process_tree_view_element(child, info_path_elements[1:], info_path_value)

  def add_remote_value(self, node_path, value):
    if not self.remote_values:
      return

    path_elements = node_path.split(".")
    self._process_tree_view_element(self.remote_values.get_root(), path_elements, value)

  def add_log(self, raw_message):
    first_separator_pos = raw_message.find(" ")
    level = raw_message[: first_separator_pos]
    message = raw_message[first_separator_pos + 1:]
    if level == "=VALUE":
      separator = message.find(":")
      self.add_remote_value(message[:separator], message[separator + 1:])
    elif self.log_view:
      self.log_view.add_log(level, message)
