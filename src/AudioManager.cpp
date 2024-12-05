#include "Audio.h"
#include <vector>

class AudioManager {
public:
    std::vector<Sound> sounds;

    
    void LoadSoundsFromXML(pugi::xml_node soundNode) {
        for (pugi::xml_node node = soundNode.child("sound"); node; node = node.next_sibling("sound")) {
            Sound sound;
            sound.LoadFromXML(node);
            sounds.push_back(sound);
        }
    }

    // Reproducir sonido por nombre
    bool PlaySound(const std::string& soundName, int loops = 0) {
        for (const auto& sound : sounds) {
            if (sound.name == soundName) {
                return sound.Play(loops);
            }
        }
        LOG("Sound %s not found!", soundName.c_str());
        return false;
    }

    // Detener música
    void StopMusic() const {
        Sound::Stop();
    }

    // Pausar música
    void PauseMusic() const {
        Sound::Pause();
    }

    // Reanudar música
    void ResumeMusic() const {
        Sound::Resume();
    }
};
