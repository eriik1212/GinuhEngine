#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

	//// ----------------------------------- Init Functions
	//bool InitMemoryManager();
	//bool InitStreamingManager();
	//bool InitSoundEngine();
	//bool InitMusicEngine();
	//bool InitSpatialAudio(); // INIT OK!
	//bool InitCommunications();

	//// -----------------------------------

	//void ProcessAudio();

	//// ----------------------------------- 
	//
	//// ----------------------------------- Term Functions
	//bool TermCommunicationModule();
	////bool TermSpatialAudio();  // NO TERM FUNCTION :(
	//bool TermMusicEngine();
	//bool TermSoundEngine();
	//bool TermStreamingManager();
	//bool TermMemoryManager();
};

#endif // __ModuleAudio_H__