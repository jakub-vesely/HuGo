--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local display = {}
function display.draw_point(x, y, color)
    return cl_display_draw_point(x, y, color);
end

function display.showtime()
    return cl_display_showtime();
end

function display.draw_elipse(x, y, r1, r2, color)
    return cl_display_draw_elipse(x, y, r1, r2, color);
end

function display.print_text_8x8(x, y, r1, r2, text, color)
    return cl_display_print_text_8x8(x, y, r1, r2, text, color);
end

function display.print_text_8x16(x, y, r1, r2, text, color)
    return cl_display_print_text_8x16(x, y, r1, r2, text, color);
end

return display
