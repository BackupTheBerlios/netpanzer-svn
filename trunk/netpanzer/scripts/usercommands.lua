UserCommands =
{
    say_help = "Says something to all players",
    say = function(param)
        if param then
            netpanzer.say(param);
        end
    end,

    teamsay_help = "Says something to team players",
    teamsay = function(param)
        if param then
            netpanzer.teamsay(param);
        end
    end,

    server_help = "Sends the command to the server, no need to add '/' in command";
    server = function(param)
        if param then
            netpanzer.say('/' .. param);
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

	-- doesn't work as expected, needs checking.
    --disconnect_help = "Disconnects from current server.",
    --disconnect = function(param)
    --    GameManager:quitNetPanzerGame();
    --end,

    info = function(param)
        ConsoleInterface:post( Color.cyan, false, 0, "You are player: " .. PlayerInterface:getLocalPlayerIndex());
        local ps = PlayerInterface:getLocalPlayer();
        ConsoleInterface:post( Color.cyan, true, ps:getFlag(), "Your name is " .. ps:getName());
        ConsoleInterface:post( Color.cyan, false, 0, "Your id is " .. ps:getID());
        local numOb = ps:getObjectivesHeld();
        if (numOb == 0) then ConsoleInterface:post( Color.cyan, false, 0, "You have'nt bases")
        elseif (numOb == 1) then ConsoleInterface:post( Color.cyan, false, 0, "You have 1 base")
    	else ConsoleInterface:post( Color.cyan, false, 0, "You have " .. numOb .. " bases") end
    end
    
};

