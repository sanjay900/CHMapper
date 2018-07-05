devices = {
    drums0 = {
        type = "Wii",
        extension_type = "Drums"
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
function string.starts(String,Start)
    return string.sub(String,1,string.len(Start))==Start
end

function string.ends(String,End)
    return End=='' or string.sub(String,-string.len(End))==End
end
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
function axis_event(name, device, axis, value)
    if not string.ends(name,"_ext") then
        return
    end
    name = string.gsub(name, "_ext", "")
    local vDev = v_devices["v"..name];
    if string.starts(name,"guitar") then
        vDev.send_axis(axis,value)
    elseif string.starts(name,"drums") then
        if axis < 2 then
            vDev.send_axis(axis,value)
        else
            if value > -32767 then
                vDev.send_button(axis,true)
                count = 0
            else
                vDev.send_button(axis,false)
            end
        end
    end
end


function button_event(name, device, button, value)
    if not string.ends(name,"_ext") then
        return
    end
    name = string.gsub(name, "_ext", "")
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