devices = {
    drums0 = {
        type = "Wii",
        extension = "Drums"
    }
}
v_devices = {
    vguitar0 =
    {
        buttons = 9,
        axes = 6
    },
    vguitar1 =
    {
        buttons = 9,
        axes = 6
    },
    vguitar2 =
    {
        buttons = 9,
        axes = 6
    },
    vguitar3 =
    {
        buttons = 9,
        axes = 6
    },
    vdrums0 =
    {
        buttons = 9,
        axes = 2
    }
}
count = 0
function tick(msec)
    count = count + msec
    if count%10 == 0 then
        for i = 0,9 do
            send_button_event("vdrums0", i, 0)
        end
    end
end
function test()
    v_devices["vdrums0"].send_button(count%9, count >= 9)
end
function axis_event(device, axis, value)
    if startsWith(device,"guitar") then
        local vDev = "v"..device;
        v_devices[vDev].send_axis(axis,value)
    elseif startsWith(device,"drums") then
        local vDev = "v"..device;
        if axis < 2 then
            v_devices[vDev].send_axis(axis,value)
        else
            if value > -32767 then
                v_devices[vDev].send_button(axis,true)
                send_button_event(vDev, axis, 1)
                count = 0
            else
                v_devices[vDev].send_button(axis,false)
            end
        end
    end
end


function button_event(device, button, value)
    if startsWith(device,"guitar") then
        local vDev = "v"..device;
        if button >= 2 then
            v_devices[vDev].send_button(button - 2, value)
        else
            v_devices[vDev].send_button(button + 7, value)
        end
    elseif startsWith(device,"drums") then
        local vDev = "v"..device;
        v_devices[vDev].send_button(button, value)
    end
end