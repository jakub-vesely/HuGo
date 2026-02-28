#!/usr/bin/env python3

import requests
import time

from hugo_dash_board import HugoDashBoard
from hugo_logger import HugoLogger

class ShellyDashboardAgent:
    config_path = "../.hugo-board.json"
    dashboard = HugoDashBoard(config_path, HugoLogger(False))
    device_name="shelly"
    shelly_ip = "192.168.1.3"

    def get_shelly_power(self):
        url = f"http://{self.shelly_ip}/rpc/Shelly.GetStatus"
        em_data = {}
        try:
            response = requests.get(url, timeout=5)
            response.raise_for_status()
            data = response.json()

            # The Pro 3EM stores phase data in the 'em:0' (or similar) component
            # We iterate through the phases usually labeled a, b, and c
            em_data = data.get("em:0", {})

        except requests.exceptions.RequestException as e:
            print(f"Error connecting to Shelly: {e}")

        return em_data

    def add_log(self, component, quantity, value, unit):
        self.dashboard.add_log(f"=VALUE {self.device_name}.{component}.{quantity}: {value}" + (f" {unit}" if unit else ""))

    def run(self):
        while True:

            em_data = self.get_shelly_power()
            #self.add_log("phase A", "voltage", em_data["a_voltage"], "V")
            #self.add_log("phase A", "current", em_data["a_current"], "mA")
            self.add_log("phase A", "act_power", em_data["a_act_power"], "W")

            #self.add_log("phase B", "voltage", em_data["b_voltage"], "V")
            #self.add_log("phase B", "current", em_data["b_current"], "mA")
            self.add_log("phase B", "act_power", em_data["b_act_power"], "W")

            #self.add_log("phase C", "voltage", em_data["b_voltage"], "V")
            #self.add_log("phase C", "current", em_data["b_current"], "mA")
            self.add_log("phase C", "act_power", em_data["c_act_power"], "W")

            self.add_log("total", "total_act_power", em_data["total_act_power"], "W")
            time.sleep(3 * 60)

ShellyDashboardAgent().run()
