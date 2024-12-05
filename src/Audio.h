#pragma once

#include "SDL2/SDL_mixer.h"
#include "pugixml.hpp"
#include <string>
#include <unordered_map>

class Sound {
public:
    enum class SoundType {
        FX,     
        MUSIC   
    };

private:
    std::string name;        
    std::string path;         
    SoundType type;           
    Mix_Chunk* chunk = nullptr;  
    Mix_Music* music = nullptr; 

public:
    Sound() = default;
    ~Sound();

    
    void LoadFromXML(pugi::xml_node soundNode);

    
    bool Play(int loops = 0) const;

    
    static void Stop();

    
    static void Pause();

    
    static void Resume();
};