set(lua_dir ${CMAKE_CURRENT_SOURCE_DIR}/lua/src)
set(lua_source_files
    ${lua_dir}/lapi.c      ${lua_dir}/lcorolib.c  ${lua_dir}/ldump.c   ${lua_dir}/llex.c      ${lua_dir}/lopcodes.c
    ${lua_dir}/lstrlib.c   ${lua_dir}/luac.c      ${lua_dir}/lauxlib.c ${lua_dir}/lctype.c    ${lua_dir}/lfunc.c
    ${lua_dir}/lmathlib.c  ${lua_dir}/loslib.c    ${lua_dir}/ltable.c  ${lua_dir}/lundump.c   ${lua_dir}/lbaselib.c
    ${lua_dir}/ldblib.c    ${lua_dir}/lgc.c       ${lua_dir}/lmem.c    ${lua_dir}/lparser.c   ${lua_dir}/ltablib.c
    ${lua_dir}/lutf8lib.c  ${lua_dir}/lbitlib.c   ${lua_dir}/ldebug.c  ${lua_dir}/linit.c     ${lua_dir}/loadlib.c
    ${lua_dir}/lstate.c    ${lua_dir}/ltm.c       ${lua_dir}/lvm.c     ${lua_dir}/lcode.c     ${lua_dir}/ldo.c
    ${lua_dir}/liolib.c    ${lua_dir}/lobject.c   ${lua_dir}/lstring.c ${lua_dir}/lua.c       ${lua_dir}/lzio.c
)

set (main_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../main)
set(component_dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(lua_files
    ${main_dir}/main.lua
    ${component_dir}/built_in_led/built_in_led.lua
    ${component_dir}/timer/timer.lua
    ${component_dir}/gpio/gpio.lua
    ${component_dir}/chassis/chassis.lua
    ${component_dir}/ir_remote/ir_remote.lua
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
    SRCS ${lua_source_files} lua_partition.c .lua_files.c
    INCLUDE_DIRS ${lua_dir} .
    EMBED_TXTFILES ${lua_files}
    REQUIRES spi_flash spiffs
)
