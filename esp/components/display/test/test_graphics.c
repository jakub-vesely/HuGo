/*
#include <stdbool.h>
#include <built_in_led.h>
#include <esp_log.h>
#include <hugo_defines.h>

#include <gpio_mocks.h>
*/

#include <display_adapter.h>
#include <graphics.h>
#include <fff.h>
#include <stdint.h>
#include <unity.h>

#define TAG "[graphics]"

#define WIDTH 100
#define HEIGHT 50
static uint8_t _buffer[WIDTH * HEIGHT];

FAKE_VOID_FUNC(fff_clean);
FAKE_VALUE_FUNC(bool, fff_set_point, int, int, bool);
FAKE_VALUE_FUNC(bool, fff_get_point, int, int);
FAKE_VOID_FUNC(fff_showtime);

static void before_each(void)
{
    FFF_RESET_HISTORY();
    RESET_FAKE(fff_clean);
    RESET_FAKE(fff_set_point);
    RESET_FAKE(fff_get_point);
    RESET_FAKE(fff_showtime);

    memset(_buffer, 0, WIDTH * HEIGHT);
    display_adapter_t adapter = {
        .width = WIDTH,
        .height = HEIGHT,
        .clean = fff_clean,
        .get_point = fff_get_point,
        .set_point = fff_set_point,
        .showtime = fff_showtime
    };
    hugo_graphics_init(&adapter);
}
/*
static void after_each(void)
{
}
*/

TEST_CASE("hugo_graphics_set_point", TAG)
{
    before_each();
    fff_set_point_fake.return_val = false;
    bool ret_val = hugo_graphics_set_point(1, 2, true);
    TEST_ASSERT_EQUAL(1, fff_set_point_fake.call_count);
    TEST_ASSERT_EQUAL(1, fff_set_point_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(2, fff_set_point_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(true, fff_set_point_fake.arg2_history[0]);
    TEST_ASSERT_EQUAL(false, ret_val);

    fff_set_point_fake.return_val = true;
    ret_val = hugo_graphics_set_point(1, 2, true);
    TEST_ASSERT_EQUAL(true, ret_val);
}

TEST_CASE("hugo_graphics_get_point", TAG)
{
    before_each();
    fff_get_point_fake.return_val = false;
    bool ret_val = hugo_graphics_get_point(1, 2);
    TEST_ASSERT_EQUAL(1, fff_get_point_fake.call_count);
    TEST_ASSERT_EQUAL(1, fff_get_point_fake.arg0_history[0]);
    TEST_ASSERT_EQUAL(2, fff_get_point_fake.arg1_history[0]);
    TEST_ASSERT_EQUAL(false, ret_val);

    fff_get_point_fake.return_val = true;
    ret_val = hugo_graphics_get_point(1, 2);
    TEST_ASSERT_EQUAL(true, ret_val);
}

TEST_CASE("hugo_graphics_print_text_8x8", TAG)
{
    before_each();
    fff_set_point_fake.return_val = false;
    hugo_graphics_print_text_8x8(0, 0, WIDTH, HEIGHT, "a", 1);
    TEST_ASSERT_EQUAL(64, fff_set_point_fake.call_count);
}
