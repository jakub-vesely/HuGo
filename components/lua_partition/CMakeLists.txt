set (main_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../main)
set(component_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(lua_files
    ${main_dir}/main.lua
    ${component_dir}/built_in_led/built_in_led.lua
    ${component_dir}/chassis/chassis.lua
    ${component_dir}/display/display.lua
    ${component_dir}/gpio/gpio.lua
    ${component_dir}/ir_remote/ir_remote.lua
    ${component_dir}/timer/timer.lua
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
