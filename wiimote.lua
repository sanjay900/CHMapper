devices = {
    drums0 = {
        type = "wii",
        extension_type = "Drums"
    },
    guitar0 = {
        type = "wii",
        extension_type = "Guitar",
    },
    guitar1 = {
        type = "wii",
        extension_type = "Guitar",
    },
    guitar2 = {
        type = "wii",
        extension_type = "Guitar",
    },
--    keys0 = {
--        type = "midi",
--        device = "Serial->MIDI:RtMidi Output 129:0",
--        debug = true
--    },
    keys0 = {
        type = "midi_serial",
        device = "/dev/ttyACM1",
        baudrate = 115200,
        debug = true
    }
}
v_devices = {
    key = {
        type="keyboard",
    },
    midi = {
        type="midi",
        device = "/dev/midi3"
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
function midi_in_note_on(device, channel, key, velocity)
    if (key >= 41 and (key-41)/2 < 9) then
        v_devices.vguitar1.send_button((key-41)/2,velocity ~= 0)
    end
    if key >= 65 and (key-65)/2 < 9 then
        v_devices.vguitar1.send_button((key-65)/2,velocity ~= 0)
    end
end
function disconnect_event(device)
    local name = device.name;
    local vDev = v_devices["v"..name];
    if string.starts(name,"guitar") then
        vDev.send_button(7,true)
        vDev.send_button(7,false)
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
        if device.type == "Drums" then
            local vel = math.scale(value,-32767,0, 0, 127)
            vel = math.min(127,vel)
            v_devices.midi.note(1,50+axis,vel)
        end
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