devices = {
    drums0 = {
        type = "Nintendo Wii Remote",
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
function axis_event(device, axis, value)
    if startsWith(device,"guitar") then
        local vDev = "v"..device;
        send_axis_event(vDev, axis, value)
    elseif startsWith(device,"drums") then
        local vDev = "v"..device;
        if axis < 2 then
            send_axis_event(vDev, axis, value)
        else
            if value > -32767 then
                send_button_event(vDev, axis, 1)
                count = 0
            else
                send_button_event(vDev, axis, 0)
            end
        end
    end
end


function button_event(device, button, value)
    if startsWith(device,"guitar") then
        local vDev = "v"..device;
        if button >= 2 then
            send_button_event(vDev, button - 2, value)
        else
            send_button_event(vDev, button + 7, value)
        end
    elseif startsWith(device,"drums") then
        local vDev = "v"..device;
        send_button_event(vDev, button, value)
    end
end