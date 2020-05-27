set(interfaces modules/behaviors/interfcs)
set(behaviors modules/behaviors)
set(adapters modules/adapters)

set(sources
    "main.c"
    "${adapters}/build_in_led_adapter.c"
    "${adapters}/program_adapter.c"
    ".lua_files.c"
)

set(lua_files
    "${interfaces}/build_in_led_int.lua"
    "${behaviors}/build_in_led_beh.lua"
    "${interfaces}/program_int.lua"
    "${behaviors}/program_beh.lua"
)

idf_component_register(SRCS ${sources}
                    INCLUDE_DIRS "." ${adapters}
                    EMBED_TXTFILES ${lua_files}
)

add_subdirectory("external")
target_link_libraries(${COMPONENT_LIB} PUBLIC lua)
