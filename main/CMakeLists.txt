set(modules "${CMAKE_CURRENT_SOURCE_DIR}/modules")
set(interfaces "${modules}/interfcs")
set(adapters "${modules}/adapters")
set(luvent "${CMAKE_CURRENT_SOURCE_DIR}/external/Luvent/")

set(sources
    "main.c"
    "${adapters}/build_in_led_adapter.c"
    ".lua_files.c"
)

set(lua_files
    "${luvent}/src/Luvent.lua"
    "${interfaces}/build_in_led_int.lua"
    "${modules}/program.lua"
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
                    INCLUDE_DIRS "." ${adapters}
                    EMBED_TXTFILES ${lua_files}
)

add_subdirectory("external")
target_link_libraries(${COMPONENT_LIB} PUBLIC lua)
