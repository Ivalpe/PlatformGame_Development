#include "Audio.h"
#include <unordered_map>
#include <vector>

class AudioManager {
public:
    std::unordered_map<std::string, int> soundMap; // Asocia nombres a índices (FX)
    std::vector<Audio> audios;                     // Para manejar objetos Audio
    Audio* musicAudio = nullptr;                   // Música actual en reproducción

    void LoadSoundsFromXML(pugi::xml_node soundNode) {
        for (pugi::xml_node node = soundNode.child("sound"); node; node = node.next_sibling("sound")) {
            std::string name = node.attribute("name").as_string();
            std::string path = node.attribute("path").as_string();

            // Si es música
            if (soundNode.name() == std::string("OST")) {
                if (!musicAudio) {
                    musicAudio = new Audio();
                }
                if (musicAudio->PlayMusic(path.c_str())) {
                    soundMap[name] = -1; // Marcador para música
                }
            }
            // Si es FX
            else if (soundNode.name() == std::string("Fx")) {
                int fxId = LoadFx(path.c_str()); // Usa el método de Audio para cargar
                soundMap[name] = fxId;
            }
        }
    }

    // Reproducir sonido FX o música
    bool PlaySound(const std::string& soundName, int loops = 0) {
        auto it = soundMap.find(soundName);
        if (it != soundMap.end()) {
            int fxId = it->second;
            if (fxId == -1) {
                return musicAudio->PlayMusic(soundName.c_str());
            }
            else {
                return PlayFx(fxId, loops);
            }
        }
        return false;
    }

    // Otros métodos...
};