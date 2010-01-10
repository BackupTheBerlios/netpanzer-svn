LOGGER:log("Script initialization");

LOGGER:log("Video Config: " .. config.video.width .. " x " .. config.video.height)
LOGGER:log("Fullscreen: " .. tostring(config.video.fullscreen))

function pairs(t)
    local mt = getmetatable(t)
    local iter = mt and mt.__next or next
    return iter, t, nil
end

function dump_table(result, t, extra)
    local lin = extra or ""

    if type(t) ~= 'table' then
        LOGGER:log("ERROR dumping table: it is not a table")
        return
    end
	for key,value in pairs(t) do
	    -- print('temporal: ' .. key .. ' is ' .. type(value))
	    if type(value) == 'table' then
	        dump_table(result, value, (lin or "") .. key .. ".")
	    elseif type(value) == 'string' then
	        value = string.gsub(value,'"','\\"')
	        table.insert(result, lin .. key .. " = \"" .. value .. "\"");
	    elseif type(value) ~= 'function' then
	        table.insert(result, lin .. key .. " = " .. tostring(value));
		end
	end
end

gconcat = table.concat;

config.dump = function(table)
    result = {}
    dump_table(result, table)
    return gconcat(result,"\n");
end

LOGGER:log("Dumping conf:\n" .. config.dump(config))

