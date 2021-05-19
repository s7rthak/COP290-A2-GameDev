#include "sound.hpp"
#include <bits/stdc++.h>

Sound::Sound ()
{
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    Starter = Mix_LoadMUS("../assets/Music/starter.wav");
    // Back = Mix_LoadMUS("../assets/Music/back.wav");
    
    // Spook4 by PeriTune | http://peritune.com
    // Attribution 4.0 International (CC BY 4.0)
    // https://creativecommons.org/licenses/by/4.0/
    // Music promoted by https://www.chosic.com/ 
    Back = Mix_LoadMUS("../assets/Music/peritune-spook4.mp3");
    Good = Mix_LoadMUS("../assets/Music/winning.wav");
    Bad = Mix_LoadMUS("../assets/Music/losing.wav");
    if (Starter == NULL) std::cout << "WTF\n";
}

void Sound::playStarter ()
{
    StopMusic ();
    Mix_PlayMusic( Starter, -1 );
}

void Sound::playBack ()
{
    StopMusic ();
    Mix_PlayMusic( Back, -1 );
}

void Sound::playGood ()
{
    StopMusic ();
    Mix_PlayMusic( Good, 0 );
}

void Sound::playBad ()
{
    StopMusic ();
    Mix_PlayMusic( Bad, 0 );
}

void Sound::StopMusic ()
{
    if (Mix_PlayingMusic() != 0) {
        Mix_HaltMusic();
    }
}

void Sound::close ()
{
    Mix_FreeMusic(Starter);
    Mix_FreeMusic(Back);
    Mix_FreeMusic(Good);
    Mix_FreeMusic(Bad);

    Mix_Quit();
}