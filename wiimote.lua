devices = {
    -- drums0 = {
    --     type = "wii",
    --     extension_type = "Drums"
    -- },
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
    -- serial0 = {
    --     type = "serial",
    --     device = "/dev/ttyACM0",
    --     baudrate = 1000000,
    -- },
    --    keys0 = {
    --        type = "midi_serial",
    --        device = "/dev/ttyACM1",
    --        baudrate = 115200,
    --        debug = false
    --    },
    --    nxt = {
    --        type = "serial",
    --        device = "/dev/rfcomm0",
    --        baudrate = 38400,
    --    }
}
v_devices = {
    --    key = {
    --        type="keyboard",
    --    },
    --    midi = {
    --        type="midi",
    --        device = "/dev/midi3"
    --    },
    vguitar0 = {
        buttons = 11,
        axes = 8
    },
    vguitar1 = {
        buttons = 11,
        axes = 6
    },
    vguitar2 = {
        buttons = 11,
        axes = 6
    },
    -- vguitar3 = {
    --     buttons = 11,
    --     axes = 6
    -- },
    -- vdrums0 = {
    --     buttons = 11,
    --     axes = 6,
    --     guitar = true
    -- },
    --    serial = {
    --        type = "serial",
    --        device = "/dev/ttyACM0",
    --        baudrate = 115200,
    --    },
    --    serial1 = {
    --        type = "serial",
    --        device = "/dev/ttyACM1",
    --        baudrate = 115200,
    --    },
    --    nxt = {
    --        type = "serial",
    --        device = "/dev/rfcomm0",
    --        baudrate = 57600,
    --    }
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
        --        v_devices.nxt.write({0x03, 0x00, 0x00, 0x06,0x01})
    end
end
current = -1;
function serial_in(device, byte)
    if byte == 128 then
        current = 0;
    end
    if current >= 0 then
        if current < 10 and current > 0 then v_devices.vguitar2.send_button(current-1,byte==1) end
        if current == 10 then v_devices.vguitar2.send_axis(0, (byte-32)*1024) end
        if current == 11 then v_devices.vguitar2.send_axis(1, (byte-32)*1024) end
        if current == 12 then v_devices.vguitar2.send_axis(2, (byte-15)*2048) end
        if current == 13 then
            if byte == 1 then
                v_devices.vguitar2.send_axis(3, 32767);
            else
                v_devices.vguitar2.send_axis(3, 0);
            end
        end
        current= current+1;
    end
end
function midi_in_note_on(device, channel, key, velocity)
    --    if velocity ~= 0 then
    --        local hz = math.floor(math.pow(2,(key-69) / 12) * (440))
    --        local duration = 100
    --        v_devices.nxt.write({6, 0, 0, 3, hz&0xff, hz >> 8, duration&0xff, duration >> 8})
    --    end
    -- local key2 = math.ceil(((key-41)/2));
    -- if key2 == 0 then v_devices.serial1.write({10, velocity>0}) end
    -- if key2 == 1 then v_devices.serial1.write({11, velocity>0}) end
    -- if key2 == 2 then v_devices.serial1.write({12, velocity>0}) end
    -- if key2 == 3 then v_devices.serial1.write({13, velocity>0}) end
    -- if key2 == 4 then v_devices.serial1.write({5, velocity>0}) end
    -- if key2 == 5 then v_devices.serial1.write({6, velocity>0}) end
    -- if key2 == 6 then v_devices.serial1.write({9, velocity>0}) end
    if (key >= 41 and (key-41)/2 < 9) then
        v_devices.vguitar3.send_button((key-41)/2,velocity ~= 0)
    end
    if key >= 65 and (key-65)/2 < 9 then
        v_devices.vguitar3.send_button((key-65)/2,velocity ~= 0)
    end
end
function midi_in(device, param1, param2, param3)
    -- v_devices.midi.write(param1, param2, param3)
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
            if axis == 0 then
                vDev.send_axis(4,math.max(math.pow((10000 - value),1.25),32768))
            end
        elseif axis == 0 then
            if value > 32767 / 2 then
                vDev.send_axis(6,32767)
            elseif value < -32767 / 2 then
                vDev.send_axis(6,-32767)
            else
                vDev.send_axis(6,0)
            end
        elseif axis == 1 then
            if value > 32767 / 2 then
                vDev.send_axis(7,-32767)
            elseif value < -32767 / 2 then
                vDev.send_axis(7,32767)
            else
                vDev.send_axis(7,0)
            end
        elseif axis == 3 then
            if value <= -32767 then
                vDev.send_axis(3,-32767)
            else
                vDev.send_axis(3,value/2 + 32767/2 - 3000)
            end
        end
    elseif string.starts(name,"drums") then
        if device.type == "Drums" then
            local vel = math.scale(value,-32767,0, 0, 127)
            vel = math.min(127,vel)
            --            v_devices.midi.note(1,50+axis,vel)
            if axis == 7 then
                v_devices.vguitar0.send_button(8,value > 0)
            end

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

guitar_mappings = {0,1,3,2,4,6,7}

function button_event(device, button, value)
    local name = device.name;
    local vDev = v_devices["v"..name];
    if string.starts(name,"guitar") then
        if button == 7 then
            vDev.send_axis(7, value and -32768 or 0)
        elseif button == 8 then
            vDev.send_axis(7, value and 32768 or 0)
        else
            vDev.send_button(guitar_mappings[button+1], value)
        end
    elseif string.starts(name,"drums") then
        vDev.send_button(button, value)
    end
end
