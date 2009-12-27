
LOGGER:log("Script initialization");

LOGGER:log("gametype is " .. gameconfig.gametype);

UserCommands =
{
    say_help = "Says something to all players",
    say = function(param)
        if param then
            ChatInterface:say(param);
        end
    end,

    teamsay_help = "Says something to team players",
    teamsay = function(param)
        if param then
            ChatInterface:teamsay(param);
        end
    end,

    server_help = "Sends the command to the server, no need to add '/' in command";
    server = function(param)
        if param then
            ChatInterface:say('/' .. param);
        end
    end,

    listcommands_help = "List all the commands";
    listcommands = function(param)
        local out;
        for k,v in pairs(UserCommands) do
            if type(v) == "function" then
                if out then
                    out = out .. ", " .. k;
                else
                    out = k;
                end
            end
        end
        ConsoleInterface:post( Color.cyan, false, 0, out);
    end,

    _help = "Type /help <wanted_command> or /listcommands",
    help_help = "Provides this kind of help",
    help = function(param)
        local ht = UserCommands[param .. "_help"];
        if ht then
            ConsoleInterface:post( Color.cyan, false, 0, param .. ": " .. ht);
        else
            ConsoleInterface:post( Color.cyan, false, 0, "Help not found for " .. param .. ". Use /listcommands");
        end
    end,

    quit_help = "Quits NetPanzer.",
    quit = function(param)
        GameManager:exitNetPanzer();
    end,

    disconnect_help = "Disconnects from current server.",
    disconnect = function(param)
        GameManager:quitNetPanzerGame();
    end,

    info = function(param)
        ConsoleInterface:post( Color.cyan, false, 0, "You are player: " .. PlayerInterface:getLocalPlayerIndex());
        local ps = PlayerInterface:getLocalPlayer();
        ConsoleInterface:post( Color.cyan, true, ps:getFlag(), "Your name is " .. ps:getName());
        ConsoleInterface:post( Color.cyan, false, 0, "Your id is " .. ps:getID());
        ConsoleInterface:post( Color.cyan, false, 0, "Your have " .. ps:getObjectivesHeld() .. " bases");
    end,

    testrules = function(param)
        test_game_rules();
    end
};

function onUserMessage(msg)
    local cmd,arg = msg:match('^/(%w+) *(.*)');
    if cmd then
        local func = UserCommands[cmd];
        if type(func) == "function" then
            func(arg);
        end
    else
        ChatInterface:say(msg);
    end
end

-- modes: 0=objective, 1=frag limit, 2=time limit
function test_game_rules()
    if gameconfig.gametype == 2 then
        ConsoleInterface:post( Color.cyan, false, 0, "Game is time limit");
    end
end

ServerCommands =
{
    say = function(param, player)
        if param then
            ChatInterface:serversay(param);
        end
    end,

    kick = function(param, player)
        if param then
            GameManager:kickPlayer(param);
        end
    end,

    addbot = function(param, player)
        GameManager:addBot();
    end,

    removebots = function(param, player)
        GameManager:removeAllBots();
    end,

    map = function(param, player)
        local ok = GameManager:changeMap(param);
        if ok then
            ChatInterface:serversayTo(player, 'Switching map to "' .. param .. '"');
        else
            ChatInterface:serversayTo(player, 'Map "' .. param .. '" doesn\'t exists');
        end
    end,

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
        ChatInterface:serversayTo(player, out);
    end,

    testrules = function(param)
        test_game_rules();
    end
};

function serverHandleMessage(msg, player)
    local cmd,arg = msg:match('^/(%w+) *(.*)');
    if cmd then
        local func = ServerCommands[cmd];
        if func and type(func) == "function" then
            func(arg, player);
        end
--        ConsoleInterface:post( Color.cyan, false, 0, 'received command "' .. msg .. '" from player ' .. player);
        return false;
    end
    return true;
end
