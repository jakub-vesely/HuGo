#include <stdbool.h>
#include <built_in_led.h>
#include <fff.h>
#include <esp_log.h>
#include <hugo_defines.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <unity.h>
#include <gpio_mocks.h>

#define TAG "[built_in_led]"
DEFINE_FFF_GLOBALS;
static lua_State* L = NULL;

static void before_each(void)
{
    GPIO_RESET_FAKES();
    LUA_REOPEN(L);
    hugo_built_in_led_init_module(L);
}

static void after_each(void)
{
    LUA_CLOSE(L);
    FFF_RESET_HISTORY();
}

TEST_CASE("cl_built_in_led_change_state", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "led = require 'built_in_led'; led.change_state(true); led.change_state(false)");
    TEST_ASSERT_EQUAL(2, fff_gpio_set_pin_logic_value_fake.call_count);
    TEST_ASSERT_EQUAL(true, fff_gpio_set_pin_logic_value_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(false, fff_gpio_set_pin_logic_value_fake.arg1_history[1]);

    after_each();
}
