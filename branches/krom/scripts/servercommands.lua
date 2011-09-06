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

    testrules = function(param)
        test_game_rules();
    end
};

-- modes: 0=objective, 1=frag limit, 2=time limit
function test_game_rules()
    if gameconfig.gametype == 2 then
        ConsoleInterface:post( Color.cyan, false, 0, "Game is time limit");
    end
end
