local splitsay = function(player, prefix, tbl)
    local s = "";
    local i = 0;
    while ( i < #tbl )
    do
        i = i + 1;
        if ( #s + #tbl[i] > 80 ) then
            netpanzer.serversayto(player, prefix .. s);
            s = tbl[i];
        else
            if ( #s > 0 ) then s = s .. ',' end
            s = s .. tbl[i];
        end
    end

    if ( #s > 0 ) then
        netpanzer.serversayto(player, prefix .. s);
    end
end;

ServerCommands =
{
    say_help = "Says something to all players as server.",
    say = function(param, player)
        if param then
            netpanzer.serversay(param);
        end
    end,

    listplayers_help = "List the players with their Id",
    listplayers = function(param, player)
        netpanzer.serversayto(player, 'Listing players:');
        local players = netpanzer.listplayers();
        local i = 0;
        while ( i < #players )
        do
            i = i + 1;
            local p = players[i];
            netpanzer.serversayto(player, p.id .. " - " .. p.name .. " ip: " .. p.ip);
        end
    end,

    kick_help = "Kicks a player, use the player number (starts in 0)",
    kick = function(param, player)
        if param then
            netpanzer.kick(param);
        end
    end,

    map_help = "Change the map",
    map = function(param, player)
        netpanzer.serversayto(player, netpanzer.map(param));
    end,

    listmaps_help = "List the available maps in server",
    listmaps = function(param, player)
        netpanzer.serversayto(player, 'Listing maps:');
        splitsay(player, '', netpanzer.listmaps());
    end,

    listprofiles_help = "List the available unit profiles in server",
    listprofiles = function(param, player)
        netpanzer.serversayto(player, 'Listing profiles:');
        splitsay(player, '', netpanzer.listprofiles());
    end,

    listcommands_help = "List the server commands",
    listcommands = function(param, player)
        netpanzer.serversayto(player, 'Listing commands:');
        local out = {};
        for k,v in pairs(ServerCommands) do
            if type(v) == "function" then
                table.insert(out,k);
            end
        end
        splitsay(player, '', out);
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

    baselimit_help = "sets or get the base limit for players",
    baselimit = function(param, player)
        if ( param == "" ) then
            netpanzer.serversayto( player, "Base limit: " .. config.game.base_limit);
        else
            config.game.base_limit = param;
            netpanzer.serversayto( player, "Base limit set to: " .. config.game.base_limit);
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
