#pragma once

class BlockBase {
    bool _start_i2c();


protected:
    /**
     * performs initial settings
    */
    bool setup();

    /**
     * start i2c listening loop
    */
    bool loop();
};