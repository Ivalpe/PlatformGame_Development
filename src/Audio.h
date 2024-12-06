#pragma once

#include "SDL2/SDL_mixer.h"
#include "pugixml.hpp"
#include <string>
#include <unordered_map>

class Audio {
public:
    enum class SoundType {
        FX,     
        MUSIC   
    };
    std::string name;        
    std::string path;         
    SoundType type;           
    Mix_Chunk* chunk = nullptr;  
    Mix_Music* music = nullptr; 
private:
    

public:
    Audio() = default;
    ~Audio();

    
    void LoadFromXML(pugi::xml_node soundNode);

    
    bool Play(int loops = 0) const;

    
    static void Stop();

    
    static void Pause();

    
    static void Resume();
};