--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]


local ir_remote =  {
    data_received = nil
}

function ir_remote.set_event(luvent)
    ir_remote.data_received = luvent.newEvent()
end

function Ir_remote_callback(code, repeated)
    print("LUA", code, repeated)
    ir_remote.data_received:trigger(code, repeated)
end

return ir_remote
