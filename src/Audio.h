#pragma once

#include "SDL2/SDL_mixer.h"
#include "pugixml.hpp"
#include <string>
#include <unordered_map>

class Sound {
public:
    enum class SoundType {
        FX,     // Efecto de sonido
        MUSIC   // Música
    };

private:
    std::string name;         // Nombre del sonido
    std::string path;         // Ruta del archivo
    SoundType type;           // Tipo de sonido (FX o MUSIC)
    Mix_Chunk* chunk = nullptr;   // Para efectos de sonido
    Mix_Music* music = nullptr;  // Para música

public:
    Sound() = default;
    ~Sound();

    // Cargar sonido desde un nodo XML
    void LoadFromXML(pugi::xml_node soundNode);

    // Reproducir el sonido
    bool Play(int loops = 0) const;

    // Detener la música (válido solo para OST)
    static void Stop();

    // Pausar la música (válido solo para OST)
    static void Pause();

    // Reanudar la música pausada (válido solo para OST)
    static void Resume();
};