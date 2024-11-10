#include <Arduino.h>
#include <FSM2.h>

#define LD(func) [this]() { func(); }

class MusicPlayer
{
    enum Events
    {
        PLAY,
        PAUSE,
        RESUME,
        STOP
    };

    State Stopped;
    State Paused;
    State Playing;
    State Idle;

    FSM playerFSM;

    void pause()
    {
        Serial.println("pause");
    }

    void play()
    {
        Serial.println("play");
    }

    void resume()
    {
        Serial.println("resume");
    }

    void stop()
    {
        Serial.println("stop");
    }

    void idle()
    {
        Serial.println("idle");
    }

    MusicPlayer();

    void init()
    {
        State Stopped = playerFSM.createStateInClass<MusicPlayer>(pause, NO_HANDLE, NO_EXIT);
        State Playing = playerFSM.createStateInClass<MusicPlayer>(play, NO_HANDLE, NO_EXIT);
        State Paused = playerFSM.createStateInClass<MusicPlayer>(resume, NO_HANDLE, NO_EXIT);
        State Idle = playerFSM.createStateInClass<MusicPlayer>(idle, NO_HANDLE, NO_EXIT);

        playerFSM.addEventTransition(Stopped, Playing, PLAY);
        playerFSM.addEventTransition(Playing, Paused, PAUSE);
        playerFSM.addEventTransition(Playing, Stopped, STOP);
        playerFSM.addEventTransition(Paused, Playing, RESUME);
        playerFSM.addEventTransition(Paused, Stopped, STOP);
    }
    void run()
    {
        // if (button.pressed())
        // {
        //     (playerFSM.getState() == Idle) ? playerFSM.trigger(PLAY) : playerFSM.trigger(PAUSE);
        // }
        // playerFSM.run();
    }
};