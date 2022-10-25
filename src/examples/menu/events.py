#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___basal.___logging import Logging
from ___blocks.___five_way_switch_block import FiveWaySwitchBlock
from ___blocks.___display_block import DisplayBlock
from ___extra.___menu import Menu, MenuItem

class Plan():
    def __init__(self) -> None:
        self.logging = Logging("Plan")
        self.switch = FiveWaySwitchBlock()
        self.switch.value.changed(self.switch_changed)
        self.display = DisplayBlock()
        self.disp_width, self.disp_height = self.display.get_dimensions()
        self.line_height = self.display.get_text_height() + 1; #two spacs
        self.ver_offset = 1
        self.line_count = int((self.disp_height - self.ver_offset) / self.line_height)
        self.sel_pos = int(self.line_count / 2)

        self.menu = Menu([
            MenuItem(
                "A1 loong text", [
                    MenuItem(
                        "B1", [
                            MenuItem("C1", [MenuItem("")], function=self.on_menu_c1), #placeholder
                            MenuItem("C2", function=self.on_menu),
                            MenuItem("C3", function=self.on_menu),
                            MenuItem("C4", function=self.on_menu),
                            MenuItem("C5", function=self.on_menu),
                            MenuItem("C6", function=self.on_menu),
                            MenuItem("C6", function=self.on_menu),
                        ]
                    ),
                    MenuItem(
                        "B2", [
                            MenuItem("D1", function=self.on_menu),
                            MenuItem("D2", function=self.on_menu),
                            MenuItem("D3", function=self.on_menu)
                        ]
                    )
                ]
            ),
            MenuItem(
                "A2", [
                    MenuItem("E1", function=self.on_menu),
                    MenuItem("E2", function=self.on_menu),
                    MenuItem("E3", function=self.on_menu),
                    MenuItem("E4", function=self.on_menu),
                    MenuItem("E5", function=self.on_menu),
                ]
            )
        ])
        self.display_menu()

    def on_menu(self, menu_item: MenuItem):
        self.logging.info("asked menu item:", menu_item.label)


    def on_menu_c1(self, menu_item: MenuItem): # menu added dynamically
        menu_item.submenu = [
            MenuItem("X1", function=self.on_menu),
            MenuItem("X2", function=self.on_menu),
            MenuItem("X3", function=self.on_menu)
        ]

    def display_menu(self):
        order_selected = self.menu.get_current_order()
        self.logging.info(("order_selected", order_selected))
        items = self.menu.get_current_items();

        self.display.clean()
        if items:
            list_pos = 0
            disp_pos = 0
            disp_start = order_selected - self.sel_pos
            if disp_start + self.line_count > len(items):
                disp_start = len(items) - self.line_count
            if disp_start < 0:
                disp_start = 0

            for item in items:
                if list_pos >= disp_start:
                    color = 1
                    line_y = disp_pos * self.line_height + self.ver_offset
                    if list_pos == order_selected:
                        color = 0
                        self.display.fill_rect(0, line_y, self.disp_width, self.line_height)

                    text_y_pos = line_y + 1
                    self.display.print_text(1, text_y_pos, item.label, color)
                    if item.submenu:
                        last_x = self.disp_width - 8
                        self.display.fill_rect(last_x, line_y, 8, self.line_height, 0 if color else 1)
                        self.display.print_text(last_x + 1, line_y + 1, ">", color)
                    disp_pos+=1
                    if disp_pos >= self.line_count:
                        break
                list_pos += 1
        self.display.showtime()

    def switch_changed(self):
        value = self.switch.value.get()
        if value == FiveWaySwitchBlock.button_none:
            return
        elif value == FiveWaySwitchBlock.button_up:
            self.menu.up()
        elif value == FiveWaySwitchBlock.button_down:
            self.menu.down()
        elif value == FiveWaySwitchBlock.button_left:
            self.menu.back()
        elif value in(FiveWaySwitchBlock.button_right, FiveWaySwitchBlock.button_press):
            self.menu.enter()
        self.display_menu()

Plan()
