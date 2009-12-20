
LOGGER:log("Script initialization");

function onUserMessage(msg)
    local msgx = "Wants to send message: '" .. msg .. "'";
    LOGGER:log(msgx);
    if msg == "/info" then
        ConsoleInterface:post( Color.cyan, false, 0, "It is info message");
        ConsoleInterface:post( Color.cyan, false, 0, "You are player: " .. PlayerInterface:getLocalPlayerIndex());
        ps = PlayerInterface:getLocalPlayer();
        ConsoleInterface:post( Color.cyan, true, ps:getFlag(), "Your name is " .. ps:getName());
        ConsoleInterface:post( Color.cyan, false, 0, "Your id is " .. ps:getID());
        ConsoleInterface:post( Color.cyan, false, 0, "Your have " .. ps:getObjectivesHeld() .. " bases");

    else
        ConsoleInterface:post( Color.cyan, false, 0, msgx);
    end

end