#include "Audio.h"
#include "Log.h"  // Suponiendo que tienes una función de log

Sound::~Sound() {
    if (chunk) Mix_FreeChunk(chunk);
    if (music) Mix_FreeMusic(music);
}

void Sound::LoadFromXML(pugi::xml_node soundNode) {
    name = soundNode.attribute("name").as_string();
    path = soundNode.attribute("path").as_string();
    std::string typeStr = soundNode.attribute("type").as_string();

    if (typeStr == "fx") {
        type = SoundType::FX;
        chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            LOG("Failed to load FX: %s. Mix_GetError(): %s", path.c_str(), Mix_GetError());
        }
    }
    else if (typeStr == "music") {
        type = SoundType::MUSIC;
        music = Mix_LoadMUS(path.c_str());
        if (!music) {
            LOG("Failed to load music: %s. Mix_GetError(): %s", path.c_str(), Mix_GetError());
        }
    }
    else {
        LOG("Unknown sound type for %s", name.c_str());
    }
}

bool Sound::Play(int loops) const {
    if (type == SoundType::FX) {
        if (chunk) {
            if (Mix_PlayChannel(-1, chunk, loops) == -1) {
                LOG("Failed to play FX: %s. Mix_GetError(): %s", name.c_str(), Mix_GetError());
                return false;
            }
            return true;
        }
    }
    else if (type == SoundType::MUSIC) {
        if (music) {
            if (Mix_PlayMusic(music, loops) == -1) {
                LOG("Failed to play music: %s. Mix_GetError(): %s", name.c_str(), Mix_GetError());
                return false;
            }
            return true;
        }
    }
    LOG("Sound %s not loaded or unknown type", name.c_str());
    return false;
}

void Sound::Stop() {
    Mix_HaltMusic();
}

void Sound::Pause() {
    Mix_PauseMusic();
}

void Sound::Resume() {
    Mix_ResumeMusic();
}