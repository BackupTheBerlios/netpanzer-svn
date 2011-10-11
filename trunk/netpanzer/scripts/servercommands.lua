ServerCommands =
{
    say_help = "Says something to all players as server.",
    say = function(param, player)
        if param then
            netpanzer.serversay(param);
        end
    end,

    kick_help = "Kicks a player, use the player number (starts in 0)",
    kick = function(param, player)
        if param then
            GameManager:kickPlayer(param);
        end
    end,

    map_help = "Change the map",
    map = function(param, player)
        local ok = GameManager:changeMap(param);
        if ok then
            netpanzer.serversayto(player, 'Switching map to "' .. param .. '"');
        else
            netpanzer.serversayto(player, 'Map "' .. param .. '" doesn\'t exists');
        end
    end,

    listcommands_help = "List the server commands",
    listcommands = function(param, player)
        local out;
        for k,v in pairs(ServerCommands) do
            if type(v) == "function" then
                if out then
                    out = out .. ", " .. k;
                else
                    out = k;
                end
            end
        end
        netpanzer.serversayto(player, out);
    end,

    _help = "Type /server help <wanted_command> or /server listcommands",
    help_help = "Provides this kind of help",
    help = function(param, player)
        local ht = ServerCommands[param .. "_help"];
        if ht then
            netpanzer.serversayto( player, param .. ": " .. ht);
        else
            netpanzer.serversayto( player, "Help not found for " .. param .. ". Use /server listcommands");
        end
    end,

    listplayers_help = "List the players",
    listplayers = function(param,player)
        local playerst;
        local out;
        for p = 0,PlayerInterface:getMaxPlayers()-1 do
            playerst = PlayerInterface:getPlayer(p);
            if playerst:isConnecting() then
                if out then
                    out = out .. ",".. p .. ":" .. playerst:getName() .. "[connecting:" .. PlayerInterface:getPlayerIP(p) .. "]";
                else
                    out = p .. ":" .. playerst:getName() .. "[connecting:".. PlayerInterface:getPlayerIP(p) .. "]";
                end
            elseif playerst:isActive() then
                if out then
                    out = out .. ",".. p .. ":" .. playerst:getName() .. "[" .. PlayerInterface:getPlayerIP(p) .. "]";
                else
                    out = p .. ":" .. playerst:getName() .. "[" .. PlayerInterface:getPlayerIP(p) .. "]";
                end
            end
        end
        netpanzer.serversayto(player, out);
    end,

    autokick_help = "sets the idle time for kicking a player automatically",
    autokick = function(param, player)
        if ( param == "" ) then
            netpanzer.serversayto( player, "autokick time is " .. config.game.autokicktime);
        else
            config.game.autokicktime = param;
            netpanzer.serversayto( player, "autokick time was set to " .. config.game.autokicktime);
        end
    end,

    unitprofiles_help = "sets or get list of unit profiles loaded",
    unitprofiles = function(param, player)
        if ( param == "" ) then
            netpanzer.serversayto( player, "Unit profile list: " .. config.game.unit_profiles);
        else
            config.game.unit_profiles = param;
            netpanzer.serversayto( player, "Unit profile list set to: " .. config.game.unit_profiles);
        end
    end,

    unitspawnlist_help = "sets or get the unitspawnlist",
    unitspawnlist = function(param, player)
        if ( param == "" ) then
            netpanzer.serversayto( player, "Unit spawn list: " .. config.game.unit_spawnlist);
        else
            config.game.unit_spawnlist = param;
            netpanzer.serversayto( player, "Unit spawn list set to: " .. config.game.unit_spawnlist);
        end
    end,

    countdown_help = "Do a countdown, use 'countdown <time> <message>'",
    countdown = function(param, player)
        local counttime, message = string.match(param, "(%d+) *(.*)");
        counttime = counttime or 5;
        message = message or "Countdown...";
        local count = 0;
        netpanzer.serversay(message .. " " .. counttime);

        game.addTask(function()
                count = count + game.frametime;
                if count > 1.0 then
                    count = count - 1.0;
                    counttime = counttime - 1;
                    if counttime == 0 then
                        netpanzer.serversay(message .. " FIGHT!!!!");
                    else
                        netpanzer.serversay(message .. " " .. counttime);
                    end
                end
                return counttime == 0;
            end);
    end
};
