--LOGGER:log("Script initialization");

--LOGGER:log("Video Config: " .. config.video.width .. " x " .. config.video.height)
--LOGGER:log("Fullscreen: " .. tostring(config.video.fullscreen))

function pairs(t)
    local mt = getmetatable(t)
    local iter = mt and mt.__next or next
    return iter, t, nil
end

function dump_table(result, t, extra)
    local lin = extra or ""

    if type(t) ~= 'table' then
        --LOGGER:log("ERROR dumping table: it is not a table")
        return
    end    
    
    for key,value in pairs(t) do
        local keytext
        local valuetext
    
        if type(key) == 'number' then
            if extra then
                keytext = "[" .. key .. "]"
            end
        else
            if extra then
                keytext = "." .. key
            else
                keytext = key;
            end
        end
        
        if type(value) == 'table' then
            if keytext then
                dump_table(result, value, (lin or "") .. keytext)
            end
        elseif type(value) == 'string' then
            valuetext = '"' .. string.gsub(value,'"','\\"') .. '"'
        elseif type(value) ~= 'function' then
            valuetext = tostring(value)
        end
        
        if keytext and valuetext then
            table.insert(result, lin .. keytext .. " = " .. valuetext)
        end
    end
end

gconcat = table.concat;

config.dump = function(table)
    result = {}
    dump_table(result, table)
    return gconcat(result,"\n");
end

--LOGGER:log("Dumping conf:\n" .. config.dump(config))

