#ifndef __HEARTBEATTHREAD_HPP__
#define __HEARTBEATTHREAD_HPP__

#include <string>

#include <SDL_net.h>
#include <SDL_thread.h>

/** This class is responsible for notifying the masterserver from time to time
 * that we're still alive.
 */
class HeartbeatThread
{
public:
    /** Starts a thread and sends a heartbeat packet to the masterserver every 5
     * minutes. Throws std::exception's when something goes wrong while
     * intializing.
     */
    HeartbeatThread(const std::string& masteraddress,
            const std::string& gamename, int gamestatusport);
    /** Sends a statechanged message to the masterserver indicating that the
     * server gets shutdown. Remember that the masterserver will query us to
     * make sure the packet wasn't faked. So the infothread should return the
     * same.
     */
    ~HeartbeatThread();

private:
    static int threadMain(void* data);
    void sendHeartbeat();
    
    volatile bool running;
    IPaddress serveraddr;
    SDL_Thread* thread;
    
    std::string gamename;
    int gameport;
};

#endif

