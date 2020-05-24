set(adapters modules/adapters)
set(interfaces modules/behaviors/interfcs)
set(behaviors modules/behaviors)

idf_component_register(SRCS "main.c" ${adapters}"/build_in_led_adapter.c"
                    INCLUDE_DIRS "." ${adapters}
                    EMBED_TXTFILES ${interfaces}"/build_in_led_int.lua" ${behaviors}"/build_in_led_beh.lua")

add_subdirectory("external")
target_link_libraries(${COMPONENT_LIB} PUBLIC lua)
