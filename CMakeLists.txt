cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

message("LUA files processing...")
idf_build_get_property(python PYTHON)
set(command ${python} process_lua_files.py)
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

project(hugo)

