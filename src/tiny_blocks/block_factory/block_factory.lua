--[[
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
--]]

local bf = {}
local addresses = {}

local function check_address(address)
    for index, value in ipairs(addresses) do
        if value == address then
            print("address", address, "already present") -- TODO: set a proper address when is changed
            return false
        end
    end
    table.insert(addresses, address)
    return true
end

function bf.rgb_led_block(address)
    if check_address(address) then
        return (require "tiny_block_rgb_led"):new(address)
    end
    return nil
end

function bf.motor_driver_block(address)
    if check_address(address) then
        return (require "tiny_block_motor_driver"):new(address)
    end
    return nil
end

function bf.power_block(address)
    if check_address(address) then
        return (require "tiny_block_power"):new(address)
    end
    return nil
end

function bf.ir_receiver_block(address)
    if check_address(address) then
        return (require "tiny_block_ir_receiver"):new(address)
    end
    return nil
end

function bf.display_block(address)
    if check_address(address) then
        return (require "tiny_block_display"):new(address)
    end
    return nil
end

return bf