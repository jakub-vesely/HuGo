#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <fff.h>


#include <esp_log.h>
#include <gpio.h>
#include <unity.h>
#include <hugo_defines.h>
#include <driver_gpio_mocks.h>

#define TAG "[gpio]"
DEFINE_FFF_GLOBALS;

static lua_State* L = NULL;

static void before_each(void)
{
    DRIVER_GPIO_RESET_FAKES();
    LUA_REOPEN(L);
    hugo_gpio_init_module(L);
}

static void after_each(void)
{
    LUA_CLOSE(L);
    FFF_RESET_HISTORY();
}

TEST_CASE("gpio_set_pin_for_out", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_pin_for_out(1)");
    TEST_ASSERT_EQUAL(1, fff_gpio_set_direction_fake.arg0_val);

    after_each();
}

TEST_CASE("gpio_set_level", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_logic_value(1, false)");
    TEST_ASSERT_EQUAL(1, fff_gpio_set_level_fake.arg0_val);
    TEST_ASSERT_EQUAL(0, fff_gpio_set_level_fake.arg1_val);

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_logic_value(2, true)");
    TEST_ASSERT_EQUAL(2, fff_gpio_set_level_fake.arg0_val);
    TEST_ASSERT_EQUAL(1, fff_gpio_set_level_fake.arg1_val);

    after_each();
}

TEST_CASE("gpio_set_pin_for_pwm", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_pin_for_pwm(1, 1000, 0.5)");
    TEST_ASSERT_EQUAL(1, fff_mcpwm_gpio_init_fake.arg2_val);
    TEST_ASSERT_EQUAL(1, fff_mcpwm_init_fake.call_count);

    after_each();
}

TEST_CASE("gpio_set_pwm_duty", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_pwm_duty(0.5)");
    TEST_ASSERT_EQUAL_DOUBLE(0.5, fff_mcpwm_set_duty_fake.arg3_val);

    after_each();
}

TEST_CASE("gpio_set_pwm_frequency", TAG)
{
    before_each();

    CHECK_LUA_DOSTRING(L, "gpio = require 'gpio'; gpio.set_pwm_frequency(500)");
    TEST_ASSERT_EQUAL_DOUBLE(500, fff_mcpwm_set_frequency_fake.arg2_val);

    after_each();
}

