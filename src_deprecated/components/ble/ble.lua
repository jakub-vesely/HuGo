--[[
Copyright (c) 2021 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local getters = {}
local setters = {}
local commands = {}

function ble_get_value_callback(property_id)
    if getters[property_id] == nil then
        return -math.huge
    else
        return getters[property_id](property_id)
    end
end

function ble_set_value_callback(property_id, value)
    if setters[property_id] == nil then
        return false
    else
        return setters[property_id](property_id, value)
    end
end

function ble_command_callback(command_id, data)
    if commands[command_id] == nil then
        return false
    else
        return commands[command_id](command_id, data)
    end
end

local ble = {}

function ble.register_getter(property_id, callback)
    getters[property_id] = callback
end

function ble.register_setter(property_id, callback)
    setters[property_id] = callback
end

function ble.register_command(command_id, callback)
    commands[command_id] = callback
end

return ble
