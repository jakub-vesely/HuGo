#include <stdbool.h>
#include <chassis.h>
#include <fff.h>
#include <esp_log.h>
#include <hugo_defines.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <unity.h>
#include <gpio_mocks.h>

#define TAG "[chassis]"
DEFINE_FFF_GLOBALS;
static lua_State* L = NULL;

static void before_each(void)
{
    GPIO_RESET_FAKES();
    LUA_REOPEN(L);
    hugo_chassis_init_module(L, 1 ,2, 3, 4);
}

static void after_each(void)
{
    LUA_CLOSE(L);
    FFF_RESET_HISTORY();
}

static void test_logic_values(uint8_t val0, uint8_t val1, uint8_t val2, uint8_t val3)
{
    TEST_ASSERT_EQUAL(4, fff_gpio_set_pin_logic_value_fake.call_count);
    TEST_ASSERT_EQUAL(val0, fff_gpio_set_pin_logic_value_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(val1, fff_gpio_set_pin_logic_value_fake.arg1_history[1]);
    TEST_ASSERT_EQUAL(val2, fff_gpio_set_pin_logic_value_fake.arg1_history[2]);
    TEST_ASSERT_EQUAL(val3, fff_gpio_set_pin_logic_value_fake.arg1_history[3]);
}

TEST_CASE("cl_chassis_stop", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "chassis = require 'chassis'; chassis.stop()");
    test_logic_values(false, false, false, false);


    after_each();
}

TEST_CASE("cl_chassis_go_forward", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "chassis = require 'chassis'; chassis.go_forward()");
    test_logic_values(true, false, true, false);

    after_each();
}

TEST_CASE("cl_chassis_go_backward", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "chassis = require 'chassis'; chassis.go_backward()");
    test_logic_values(false, true, false, true);

    after_each();
}

TEST_CASE("cl_chassis_rotate_clockwise", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "chassis = require 'chassis'; chassis.rotate_clockwise()");
    test_logic_values(false, true, true, false);

    after_each();
}

TEST_CASE("cl_chassis_rotate_counterclockwise", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "chassis = require 'chassis'; chassis.rotate_counterclockwise()");
    test_logic_values(true, false, false, true);

    after_each();
}
