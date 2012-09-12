﻿/*
Copyright (C) 2012 Netpanzer Team. (www.netpanzer.org), Laurent Jacques

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Classes/Network/NetMessage.hpp"
#include "Util/Timer.hpp"

class VoteManager
{
protected:
    static Uint8 type_vote;
    static bool vote_in_progress;
    static bool *player_vote;
    static Uint8 vote_counter;
    static Timer voteTimer;
public:
    static bool checkVoteTimer();
    static void checkPlayersVote();
    static void executeVoteAction();
    static void resetVote();
    static void startVote(Uint8 type);
    static void playerVote(bool responce);
    static void playerSendRequestVote(Uint8 type);
    static void serverSendRequestVote();
    static void netMessageReceiveRequestVote(const NetMessage* message, PlayerID playerid);
    static void netMessageVoteRequest(const NetMessage* message);
    static int getTimer();
};
