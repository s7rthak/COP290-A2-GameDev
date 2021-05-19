#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Sound {
    public:
        Sound ();
        ~Sound ();
        void playStarter ();
        void playBack ();
        void playGood ();
        void playBad ();
        void StopMusic ();
        void close ();

        Mix_Music* Starter = NULL;
        Mix_Music* Back = NULL;
        Mix_Music* Good = NULL;
        Mix_Music* Bad = NULL;

    private:

};

#endif