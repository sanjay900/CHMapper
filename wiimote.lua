devices = {
    drums0 = {
        type = "Wii",
        extension_type = "Drums"
    },
    guitar0 = {
        type = "Wii",
        extension_type = "Guitar",
    },
    guitar1 = {
        type = "Wii",
        extension_type = "Guitar",
    },
    guitar2 = {
        type = "Wii",
        extension_type = "Guitar",
    }
}
v_devices = {
    key = {
        keyboard = true
    },
    vguitar0 = {
        buttons = 9,
        axes = 6
    },
    vguitar1 = {
        buttons = 9,
        axes = 6
    },
    vguitar2 = {
        buttons = 9,
        axes = 6
    },
    vguitar3 = {
        buttons = 9,
        axes = 6
    },
    vdrums0 = {
        buttons = 9,
        axes = 2,
        guitar = true
    }
}
function string.starts(String,Start)
    return string.sub(String,1,string.len(Start))==Start
end

function string.ends(String,End)
    return End=='' or string.sub(String,-string.len(End))==End
end
count = 0
function tick(usec)
    count = count + usec
    if count > 100000 then
        for k, v in pairs(v_devices) do
            if string.starts(k, "vguitar") and v.guitar then
                for i = 0,v.buttons do
                    v.send_button(i,false)
                end
            end
        end
        count = 0
    end
end
function axis_event(device, axis, value)
    local name = device.name;
    local vDev = v_devices["v"..name];
    if string.starts(name,"guitar") then
        if device.type == "Accelerometer" then
            axis = axis + 3
        end
        vDev.send_axis(axis,value)
    elseif string.starts(name,"drums") then
        if vDev.guitar then
            if axis < 2 then
                vDev.send_axis(axis,value)
            elseif value > -32767 then
                vDev.send_button(axis,true)
                count = 0
            else
                vDev.send_button(axis,false)
            end
        else
            vDev.send_axis(axis,value)
        end
    end
end


function button_event(device, button, value)
    local name = device.name;
    local vDev = v_devices["v"..name];
    if string.starts(name,"guitar") then
        if button >= 2 then
            vDev.send_button(button - 2, value)
        else
            vDev.send_button(button + 7, value)
        end
    elseif string.starts(name,"drums") then
        vDev.send_button(button, value)
    end
end