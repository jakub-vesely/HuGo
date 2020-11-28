--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]


local ir_remote =  {
    actions = {}
}

function Ir_remote_callback(addr, code, repeated)
    print("Ir_remote_callback", addr, code, repeated)
    for index, action in pairs(ir_remote.actions) do
        action(addr, code, repeated)
    end
end

function ir_remote.add_data_recoveved_action(action_function)
    table.insert(ir_remote.actions, action_function)
end

return ir_remote
