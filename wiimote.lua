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
    --    serial0 = {
    --        type = "serial",
    --        device = "/dev/ttyACM1",
    --        baudrate = 115200,
    --    }
    keys0 = {
        type = "midi_serial",
        device = "/dev/ttyACM2",
        baudrate = 115200,
        debug = false
    },
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
    },
    serial = {
        type = "serial",
        device = "/dev/ttyACM0",
        baudrate = 115200,
    },
    serial1 = {
        type = "serial",
        device = "/dev/ttyACM1",
        baudrate = 115200,
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
last = -1;
function serial_in(device, byte)
    if last ~= -1 then
        if byte==1 or byte == 0 then
            if last == 2 then v_devices.vguitar0.send_button(0,byte==1) end
            if last == 4 then v_devices.vguitar0.send_button(1,byte==1) end
            if last == 5 then v_devices.vguitar0.send_button(2,byte==1) end
            if last == 3 then v_devices.vguitar0.send_button(3,byte==1) end
            v_devices.serial.write({last, byte});
        end
    end
    last = byte;
end
function midi_in_note_on(device, channel, key, velocity)
    local key2 = math.ceil(((key-41)/2));
    if key2 == 0 then v_devices.serial1.write({10, velocity>0}) end
    if key2 == 1 then v_devices.serial1.write({11, velocity>0}) end
    if key2 == 2 then v_devices.serial1.write({12, velocity>0}) end
    if key2 == 3 then v_devices.serial1.write({13, velocity>0}) end
    if key2 == 4 then v_devices.serial1.write({5, velocity>0}) end
    if key2 == 5 then v_devices.serial1.write({6, velocity>0}) end
    if key2 == 6 then v_devices.serial1.write({9, velocity>0}) end
    if (key >= 41 and (key-41)/2 < 9) then
        v_devices.vguitar1.send_button((key-41)/2,velocity ~= 0)
    end
    if key >= 65 and (key-65)/2 < 9 then
        v_devices.vguitar1.send_button((key-65)/2,velocity ~= 0)
    end
end
function midi_in(device, param1, param2, param3)
    v_devices.midi.write(param1, param2, param3)
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
        if button == 2 then v_devices.serial.write({10, value}) end
        if button == 3 then v_devices.serial.write({11, value}) end
        if button == 4 then v_devices.serial.write({12, value}) end
        if button == 5 then v_devices.serial.write({13, value}) end
        if button == 6 then v_devices.serial.write({5, value}) end
        if button == 7 then v_devices.serial.write({6, value}) end
        if button == 8 then v_devices.serial.write({9, value}) end

        if button >= 2 then
            vDev.send_button(button - 2, value)
        else
            vDev.send_button(button + 7, value)
        end
    elseif string.starts(name,"drums") then
        vDev.send_button(button, value)
    end
end