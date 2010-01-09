LOGGER:log("Script initialization");

LOGGER:log("Video Config: " .. config.video.width .. " x " .. config.video.height)
LOGGER:log("Fullscreen: " .. tostring(config.video.fullscreen))

config.morethings = 4

config.master = { master1="one", master2="two"}

function pairs(t)
    local mt = getmetatable(t)
    local iter = mt and mt.__next or next
    return iter, t, nil
end

function dump_table(result, t, extra)
    local lin = extra or ""
    if type(t) ~= 'table' then
        print('You fucker, give ma table')
        return
    end
	for key,value in pairs(t) do
	    -- print('temporal: ' .. key .. ' is ' .. type(value))
	    if type(value) == 'table' then
	        dump_table(result, value, (lin or "") .. key .. ".")
	    elseif type(value) == 'string' then
	        value = string.gsub(value,'"','\\"')
	        table.insert(result, lin .. key .. " = \"" .. value .. "\"");
		    --print(lin .. key .. " = \"" .. value .. "\"")
	    else
	        table.insert(result, lin .. key .. " = " .. tostring(value))
		    --print(lin .. key .. " = " .. tostring(value))
		end
	end
end

--for k,v in pairs(config.video) do
--    print(k .. " = " .. tostring(v))
--end

result={}
dump_table(result, config)

--table.sort(result)

print(table.concat(result,"\n"))

