# Zigbee button and LED example

By default, `ENABLE_BUTTONS = False`: only the internal temperature is reported
every 180 seconds, with the onboard status LED enabled. The I/O behaviour below
applies when `ENABLE_BUTTONS` is set to `True`.

The example uses a HuGo `ButtonBlock` and `RgbLedBlock` on I2C. The onboard
WS2812 remains a separate status indicator.

- Endpoint 2: existing temperature sensor.
- Endpoint 3: Binary Input cluster (`0x000F`), PresentValue (`0x0055`), reporting
  the physical button state (pressed/released).
- Endpoint 4: On/Off light (`0x0100`) with On/Off cluster (`0x0006`), supporting
  remote On, Off and Toggle commands.

Pressing the button turns the external LED on; releasing it turns it off.
Remote commands also control that LED. The most recently processed local or
remote event determines its state. Remote LED commands do not change the
reported physical button state.

The example polls the live Zigbee LED attribute through Planner every 0.1 s.
I2C operations run in Python, never in the Zigbee task. Button changes trigger
reports; failed reports are retried every 5 s, and current states are reported
again after joining. Reports use the existing synchronous delivery confirmation
(timeout 3 s per report), so radio timeouts can delay Planner and button sampling.
While disconnected, local control still works; reconnection reports current
states, not a history of button presses.

## Deployment and hardware check

1. Build and flash HUGO_H2 with the updated native Zigbee module and exported
   `basal.zigbee` wrapper. Updating `events.py` alone is insufficient.
2. Upload this `events.py` using the normal HuGo workflow.
3. Have the coordinator rediscover the device endpoints. An existing pairing
   may need to be removed and paired again to discover the new entities.
4. Check that button press/release controls the external LED and updates the
   button state in the coordinator.
5. Send On, Off and Toggle to endpoint 4. Verify the LED and reported state.
6. Alternate remote commands and local presses, then repeat local control
   with the coordinator unavailable and verify state reporting after reconnection.

Host tests cover application logic with mocked hardware. They do not validate
I2C wiring, radio delivery, or Home Assistant entity discovery.

## Power saving with Zigbee

Starting either Zigbee role through `basal.zigbee` installs a radio-aware idle
handler in `PowerMgmt`. Planner's idle interval blocks only the Python task;
it no longer forces `machine.lightsleep()` while the Zigbee stack is running.
For a joined end device, ESP-IDF automatic light sleep respects native tasks,
timers and radio power-management locks. Routers and devices still joining only
yield the CPU, without enabling automatic sleep. End devices use RX off when
idle; routers keep their receiver on.

The HUGO_H2 build enables `CONFIG_PM_ENABLE`,
`CONFIG_FREERTOS_USE_TICKLESS_IDLE` and `CONFIG_IEEE802154_SLEEP_ENABLE`.
Existing build directories need these options enabled in their `sdkconfig` as
well; changed defaults do not override saved options. The native idle handler
reports PM configuration/restore errors, but returning from it does not prove
that hardware actually slept: active tasks or PM locks may prevent sleep.

The temperature interval remains 180 seconds. The existing Zigbee keep-alive
interval is 4000 ms, so the radio can wake between temperature reports. BLE
advertising windows also remain scheduled independently. Verify reporting,
rejoining after coordinator loss, BLE reconnection and the current waveform on
hardware after flashing. Without Zigbee, HuGo retains manual light sleep.
