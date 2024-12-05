#pragma once

#include "Module.h"
#include "SDL2/SDL_mixer.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;

class Audio : public Module
{
public:

	Audio();
	bool active;  
	std::unordered_map<std::string, Mix_Chunk*> namedFx;
	// Destructor
	virtual ~Audio();

	void LoadFxFromXML(pugi::xml_node fxNode);

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	 Load a WAV in memory
	//int LoadFx(const char* path);

	 Play a previously loaded WAV
	//bool PlayFx(int fx, int repeat = 0);



	//int LoadFx(const char* path, const std::string& name);  
	//bool PlayFx(int id,const std::string& name, int repeat = 0);

private:

	_Mix_Music* music;
	std::list<Mix_Chunk*> fx;
	//std::map<std::string, Mix_Chunk*> namedFx;
};
