set (main_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../main)
set(component_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(tiny_block_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../tiny_blocks)
set(lua_files
    ${component_dir}/built_in_led/built_in_led.lua
    ${component_dir}/chassis/chassis.lua
    ${component_dir}/gpio/gpio.lua
    ${component_dir}/ir_remote/ir_remote.lua
    ${component_dir}/timer/timer.lua
    ${component_dir}/mpu9250/mpu9250.lua
    ${tiny_block_dir}/block_factory/block_factory.lua
    ${tiny_block_dir}/tiny_block_base/tiny_block_base.lua
    ${tiny_block_dir}/tiny_block_rgb_led/tiny_block_rgb_led.lua
    ${tiny_block_dir}/tiny_block_motor_driver/tiny_block_motor_driver.lua
    ${tiny_block_dir}/tiny_block_power/tiny_block_power.lua
    ${tiny_block_dir}/tiny_block_ir/tiny_block_ir_receiver.lua
    ${tiny_block_dir}/tiny_block_display/tiny_block_display.lua

    # main.lua should be the last one because is used to test if partition was initialized properly
    ${main_dir}/main.lua
)

message("LUA files processing...")
idf_build_get_property(python PYTHON)
set(command ${python} process_lua_files.py ${lua_files})

execute_process(
    COMMAND ${command}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE test
    RESULT_VARIABLE result
)

if(${result})
    # No way to have CMake silently fail, unfortunately
    message(FATAL_ERROR "${TOOL} failed")
endif()
message("LUA files - done")

idf_component_register(
    SRCS lua_partition.c .lua_files.c
    INCLUDE_DIRS .
    EMBED_TXTFILES ${lua_files}
    REQUIRES spi_flash spiffs
)
