set(modules "${CMAKE_CURRENT_SOURCE_DIR}/modules")
set(luvent "${CMAKE_CURRENT_SOURCE_DIR}/external/Luvent/")

set(sources
    "main.c"
    "${modules}/built_in_led/built_in_led.c"
    "${modules}/timer/timer.c"
    "${modules}/gpio/gpio.c"
    "${modules}/chassis/chassis.c"
    ".lua_files.c"
)

set(lua_files
    "${luvent}/src/Luvent.lua"
    "${modules}/built_in_led/built_in_led.lua"
    "${modules}/timer/timer.lua"
    "${modules}/program/program.lua"
    "${modules}/gpio/gpio.lua"
    "${modules}/chassis/chassis.lua"
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

idf_component_register(SRCS ${sources}
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES ${lua_files}
)

add_subdirectory("external")
target_link_libraries(${COMPONENT_LIB} PUBLIC lua)
