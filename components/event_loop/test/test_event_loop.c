#include <event_loop.c>
#include <stdbool.h>
#include <unity.h>

#define TEST_TAG "[event_loop]"

TEST_CASE("get_new_event_id", TEST_TAG)
{
    TEST_ASSERT_EQUAL(hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY), 0);
    TEST_ASSERT_EQUAL(hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY), 1);
}
static int action_variable = 0;

void action_func(event_data_t data, int data_size)
{
    TEST_ASSERT_EQUAL(*((int*)data), 1);
    TEST_ASSERT_EQUAL(data_size, sizeof(int));
    action_variable = *((int*)data);
}

TEST_CASE("add_event_action", TEST_TAG)
{
    int event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
    TEST_ASSERT_TRUE(hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, event_id, action_func));
    //add the same action twice
    TEST_ASSERT_TRUE(hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, event_id, action_func));

    //test full event-action array
    bool pass = false;
    for (int i = 0; i < 1000; ++i) // 1000 - just a number higher than size of array
    {
        if (!hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, event_id, action_func))
        {
            pass = true;
            break;
        }
    }
    TEST_ASSERT_TRUE(pass);

    TEST_ASSERT_TRUE(hugo_remove_event_action(EVENT_LOOP_TYPE_PRIMARY, event_id, action_func));
    TEST_ASSERT_FALSE(hugo_remove_event_action(EVENT_LOOP_TYPE_PRIMARY, -1, action_func));
}

// TEST_CASE("remove_event_action", TEST_TAG)
// {
//     TEST_ASSERT_TRUE(false);
// }

TEST_CASE("raise_event", TEST_TAG)
{
    int event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
    hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, event_id, action_func);
    int variable = 1;
    hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, event_id, &variable, sizeof(int));
    _process_buffer(EVENT_LOOP_TYPE_PRIMARY);
    TEST_ASSERT_EQUAL(variable, action_variable);
}

