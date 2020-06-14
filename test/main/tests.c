
#include "unity.h"
#include <lua_partition.h>
#include <stdbool.h>

void app_main(void)
{
    hugo_lua_partition_init();

    // UNITY_BEGIN();
    // unity_run_tests_by_tag("[timer]", false);
    // UNITY_END();

    UNITY_BEGIN();
    unity_run_all_tests();
    UNITY_END();
}
