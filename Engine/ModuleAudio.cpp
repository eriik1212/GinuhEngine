#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	//LOG("Initializing Audio context");
	App->menus->info.AddConsoleLog("Initializing Audio context");

    //InitMemoryManager();
    //InitStreamingManager();
    //InitSoundEngine();
    //InitMusicEngine();
    ////InitSpatialAudio();
    //InitCommunications();

	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleAudio::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleAudio::PostUpdate(float dt)
{
    //ProcessAudio();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	//LOG("Destroying 3D Renderer");
	App->menus->info.AddConsoleLog("Destroying Module Audio");

    //TermCommunicationModule();
    ////TermSpatialAudio();  // NO TERM FUNCTION :(
    //TermMusicEngine();
    //TermSoundEngine();
    //TermStreamingManager();
    //TermMemoryManager();

	return true;
}