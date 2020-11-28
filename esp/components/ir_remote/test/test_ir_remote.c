#include <driver_rtm_abstraction.h>
#include <driver_rtm_mocks.h>
#include <esp_log.h>
#include <esp_ringbuffer_abstraction.h>
#include <esp_ringbuffer_mocks.h>
#include <event_loop.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>
#include <hugo_defines.h>
#include <ir_remote.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <soc/rmt_struct.h>
#include <stdbool.h>
#include <unity.h>


//#include <ir_remote_tools_mocks.h>

#define TAG "[ir_remote]"
DEFINE_FFF_GLOBALS;
static lua_State* L = NULL;

static void before_each(void)
{
    //GPIO_RESET_FAKES();
    LUA_REOPEN(L);
    hugo_event_loop_init(EVENT_LOOP_TYPE_PRIMARY);
}

static void after_each(void)
{
    LUA_CLOSE(L);
    FFF_RESET_HISTORY();
}

typedef struct
{
    uint32_t duration0 :15;
    uint32_t level0 :1;
    uint32_t duration1 :15;
    uint32_t level1 :1;
} rtm_mock_data_t;

static rtm_mock_data_t _stop_rmt_data[] =
{
    {9008, 0, 4493, 1},
    {534, 0, 1702, 1},
    {563, 0, 543, 1},
    {558, 0, 546, 1},
    {561, 0, 544, 1},
    {560, 0, 545, 1},
    {564, 0, 543, 1},
    {561, 0, 1676, 1},
    {559, 0, 546, 1},
    {561, 0, 544, 1},
    {562, 0, 1674, 1},
    {561, 0, 544, 1},
    {561, 0, 1676, 1},
    {561, 0, 545, 1},
    {561, 0, 1675, 1},
    {560, 0, 1674, 1},
    {562, 0, 1674, 1},
    {562, 0, 543, 1},
    {561, 0, 544, 1},
    {562, 0, 543, 1},
    {560, 0, 1676, 1},
    {560, 0, 545, 1},
    {562, 0, 543, 1},
    {561, 0, 1676, 1},
    {560, 0, 544, 1},
    {563, 0, 1675, 1},
    {586, 0, 1649, 1},
    {558, 0, 1677, 1},
    {561, 0, 543, 1},
    {560, 0, 1674, 1},
    {561, 0, 1674, 1},
    {563, 0, 542, 1},
    {563, 0, 1674, 1},
    {562, 0, 0, 1},
    {7350, 0, 32157, 1},
    {32472, 1, 4629, 0},
    {9435, 0, 4109, 0},
    {12200, 0, 17487, 0},
    {27854, 1, 17006, 1},
    {125, 0, 19557, 0},
    {2368, 1, 3355, 1},
    {24243, 1, 27813, 1},
    {22168, 0, 14190, 0},
    {8943, 1, 15614, 1},
    {15460, 0, 23833, 1},
    {29394, 1, 17791, 1},
    {20551, 1, 2877, 0},
    {1763, 1, 19026, 0},
    {24220, 0, 24511, 1},
    {32580, 0, 21734, 1},
    {4462, 0, 24747, 1},
    {24433, 1, 30280, 0},
    {6437, 1, 16619, 1},
    {24813, 1, 11582, 1},
    {14219, 1, 5561, 0},
    {9988, 1, 22065, 1},
    {46, 0, 21081, 0},
    {21091, 1, 3149, 1},
    {16728, 1, 24442, 1},
    {7407, 0, 29309, 1},
    {16980, 1, 22113, 0},
    {11145, 0, 5778, 0},
    {10268, 0, 22759, 1},
    {10758, 1, 13882, 1},
    {22184, 0, 28594, 1},
    {10664, 1, 4010, 1},
    {19838, 1, 20796, 0},
    {28013, 1, 16753, 0},
    {13008, 1, 29409, 1},
    {18040, 1, 5137, 1},
    {1666, 1, 20336, 0},
    {7531, 0, 28301, 0},
    {28755, 0, 6748, 0},
    {8083, 1, 30314, 1},
    {12390, 1, 2337, 1},
    {7857, 0, 13769, 0},
    {19310, 0, 29927, 0},
    {20472, 0, 23908, 0},
    {2311, 0, 23702, 1},
    {14778, 0, 31507, 0},
    {24611, 1, 30841, 1},
    {13755, 0, 30471, 0},
    {16141, 1, 16053, 1},
    {21225, 1, 31053, 0},
    {21541, 1, 16244, 1},
    {2819, 0, 11076, 1},
    {39, 0, 23032, 1},
    {14726, 1, 6357, 1},
    {9769, 0, 9975, 1},
    {7342, 1, 23854, 0},
    {10969, 1, 10008, 1},
    {11844, 0, 9673, 1},
    {3317, 0, 5973, 1},
    {14446, 0, 18935, 1},
    {30150, 0, 12372, 1},
    {15830, 1, 23042, 1},
    {1884, 1, 7711, 1},
    {718, 1, 12258, 1},
    {10309, 0, 4446, 1},
    {32522, 1, 30546, 1},
    {264, 0, 17179, 0},
    {16622, 1, 19181, 1},
    {14535, 1, 5745, 1},
    {13255, 1, 19753, 0},
    {13032, 0, 25587, 1},
    {26579, 0, 5383, 0},
    {1236, 0, 24417, 1},
    {2593, 0, 10400, 1},
    {1233, 0, 22799, 0},
    {29874, 1, 19456, 0},
    {25127, 1, 9043, 1},
    {16546, 0, 1350, 1},
    {28485, 1, 13240, 1},
    {2041, 1, 22004, 0},
    {24115, 1, 20117, 0},
    {25065, 0, 32151, 0},
    {13831, 1, 15495, 0},
    {21034, 1, 4902, 0},
    {14510, 1, 15915, 0},
    {9547, 1, 15749, 0},
    {20164, 0, 32587, 0},
    {2627, 1, 25795, 1},
    {20356, 1, 1533, 0},
    {14890, 1, 11279, 0},
    {15600, 1, 14695, 1},
    {29692, 1, 4054, 1},
    {12402, 0, 32627, 0},
    {18806, 1, 15668, 1},
    {7805, 1, 7575, 0},
    {16696, 1, 6695, 0},
    {32550, 0, 17722, 1},
    {15516, 0, 25068, 1},
    {13452, 0, 12329, 1},
    {26984, 0, 28587, 1},
    {25139, 0, 11202, 1},
    {11323, 0, 9264, 0},
};

TEST_CASE("Ir_remote_callback", TAG)
{
    before_each();
    set_mock_ringbuffer_content(_stop_rmt_data,  sizeof(_stop_rmt_data) / 4, 1);

    hugo_ir_remote_init_module(L, 19);

    CHECK_LUA_DOSTRING(L,
        "called = false;"
        "function was_called() return called end;"
        "ir_remote1 = require 'ir_remote';"
        "ir_remote1.add_data_recoveved_action("
        "  function (addr, code, repeated); if code == 46920 then called = true end end "
        ")");
    hugo_ir_remote_start_listening();

/*
    int countdown = 10;
    while (countdown)
    {
        countdown--;
        vTaskDelay(10);
    }
*/
    vTaskDelay(10);
    hugo_process_events(EVENT_LOOP_TYPE_PRIMARY, true);
    hugo_ir_remote_stop_listening();

    CHECK_LUA_GET_GLOBAL(L, "was_called", 0, 1, 0);
    TEST_ASSERT_EQUAL(lua_toboolean(L, -1), true);

    after_each();
}
