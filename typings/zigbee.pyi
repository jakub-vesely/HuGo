from typing import TypedDict

HA_PROFILE: int
SIMPLE_SENSOR: int
CONSUMPTION_AWARENESS: int
SERVER: int
CLIENT: int
READ: int
WRITE: int
REPORTING: int
BOOL: int
UINT8: int
UINT16: int
UINT32: int
MAP32: int
INT8: int
INT16: int
INT32: int
FLOAT32: int
ELECTRICAL_MEASUREMENT: int
TEMPERATURE_MEASUREMENT: int
RELATIVE_HUMIDITY_MEASUREMENT: int
PRESSURE_MEASUREMENT: int
WIND_SPEED_MEASUREMENT: int
CO2_MEASUREMENT: int
DC_VOLTAGE: int
DC_CURRENT: int

class Status(TypedDict):
    started: bool
    ready: bool
    joined: bool
    steering_in_progress: bool
    rejoining: bool
    startup_seen: bool
    initialization_status: int
    factory_new: int
    steering_status: int
    last_error: int
    last_report_error: int
    network_address: int
    pan_id: int
    channel: int
    ieee_address: int
    extended_pan_id: int

class Attribute:
    def send(self, value: int | float) -> None: ...

class Cluster:
    def add_attribute(self, attribute_id: int, data_type: int, initial: int | float,
                      *, access: int = ..., manufacturer_code: int = ...) -> Attribute: ...

class Endpoint:
    def add_cluster(self, cluster_id: int, *, role: int = ...) -> Cluster: ...

def configure(*, manufacturer: str = ..., model: str = ...,
              primary_channels: int = ..., secondary_channels: int = ...,
              min_join_lqi: int = ...) -> None: ...
def add_endpoint(endpoint_id: int, *, profile_id: int = ..., device_id: int = ...,
                 device_version: int = ...) -> Endpoint: ...
def start() -> None: ...
def joined() -> bool: ...
def status() -> Status: ...
def join() -> None: ...
def factory_reset() -> None: ...
