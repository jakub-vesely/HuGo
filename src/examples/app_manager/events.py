#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from blocks.five_way_switch_block import FiveWaySwitchBlock
from blocks.display_block import DisplayBlock
from extra.menu import Menu, MenuItem
import os

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

        self.menu = Menu(self.fill_dir_items("/"))
        self.display_menu()

    def fill_dir_items(self, path):
        menu_items = []
        items = os.listdir(path)
        for file_name in items:
            self.logging.info(file_name)
            file_path = path + "/" + file_name
            if file_name.endswith(".py") or file_name.endswith(".mpy"):
                menu_items.append(MenuItem(file_name, [], self.open_file, file_path))
        return menu_items

    def open_file(self, menu_item: MenuItem, path:str):
        self.logging.info(f"asked file: {menu_item.label} with path: {path}")


    def open_dir(self, menu_item: MenuItem, path:str):
        self.logging.info(f"asked dir: {menu_item.label} with path: {path}")
        menu_item.submenu = self.fill_dir_items(path)

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
