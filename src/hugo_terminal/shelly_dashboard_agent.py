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
            a_act_powers = []
            b_act_powers = []
            c_act_powers = []
            total_act_powers = []
            count = 30
            for index in range(count):

                em_data = self.get_shelly_power()
                a_act_powers.append(em_data["a_act_power"])
                b_act_powers.append(em_data["b_act_power"])
                c_act_powers.append(em_data["c_act_power"])
                total_act_powers.append(em_data["total_act_power"])
                if (index == count - 1):
                    self.add_log("phase A", "act_power", f"{sum(a_act_powers)/count:.4g}", "W")
                    self.add_log("phase B", "act_power", f"{sum(b_act_powers)/count:.4g}", "W")
                    self.add_log("phase C", "act_power", f"{sum(c_act_powers)/count:.4g}", "W")
                    self.add_log("total", "total_act_power", f"{sum(total_act_powers)/count:.4g}", "W")
                time.sleep(10)

ShellyDashboardAgent().run()
