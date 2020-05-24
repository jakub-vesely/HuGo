idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES "test.lua" "test2.lua")

add_subdirectory("external")
target_link_libraries(${COMPONENT_LIB} PUBLIC lua)
