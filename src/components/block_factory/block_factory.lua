--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local bf = {}
local addresses = {}

local function check_address(address)
    for index, value in ipairs(addresses) do
        if value == address then
            print("address %s already present", address)
            return false
        end
    end
    table.insert(addresses, address)
    return true
end

function bf.rgb_led(address)
    if check_address(address) then
        return (require "rgb_led"):new(address)
    end
    return nil
end

function bf.motor_block(address)
    if check_address(address) then
        return (require "motor_block"):new(address)
    end
    return nil
end

return bf