import json
import requests
from datetime import datetime
import logging
class HugoDashBoard:
    def __init__(self, config_path, logger) -> None:
        self.logger = logger
        self.logger.set_dashboard(self)
        with open(config_path, "r") as file:
            content = file.read()
            config = json.loads(content)
            self.url = config["url"]
            self.username = config["username"]
            self.password = config["password"]

    def add_log(self, raw_message):
        first_separator_pos = raw_message.find(" ")
        level = raw_message[: first_separator_pos]
        if level == "=VALUE":
            message = raw_message[first_separator_pos + 1:]
            parts = message.split(":")
            hierarchy = parts[0]
            hierarchy_items = hierarchy.split(".")
            variable = parts[-1].strip()
            variable_items = variable.split(" ")

            if len(hierarchy_items) == 3 and len(variable_items) >  0:
                self.send(
                    hierarchy_items[0],
                    hierarchy_items[1],
                    hierarchy_items[2],
                    variable_items[0],
                    variable_items[1] if len(variable_items) > 1 else "")
            #self.send( (message[:separator], message[separator + 1:])

    def send(self, device_name, component_name, variable_name, value, unit):
        date_time = datetime.fromtimestamp(datetime.now().timestamp())
        data = {
            "command": "add_value",
            "username": self.username,
            "password": self.password,
            "device_name": device_name,
            "component_name": component_name,
            "variable_name": variable_name,
            "value": value,
            "unit": unit,
            "measured": date_time
        }
        response = requests.post(self.url, data = data)
        if response.status_code != 200:
            self.logger.info("dashboard request not delivered")

