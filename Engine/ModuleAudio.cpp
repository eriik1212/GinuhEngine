#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>	// Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>      // Default memory manager

#include <AK/SoundEngine/Common/IAkStreamMgr.h>										// Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>										// Thread defines

#include <AK/SoundEngine/Common/AkSoundEngine.h>                // Sound engine

#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine

#include <AK/SpatialAudio/Common/AkSpatialAudio.h>              // Spatial Audio

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED

    #include <AK/Comm/AkCommunication.h>

#endif // AK_OPTIMIZED


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

    InitMemoryManager();
    InitStreamingManager();
    InitSoundEngine();
    InitMusicEngine();
    //InitSpatialAudio();
    InitCommunications();

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
    ProcessAudio();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	//LOG("Destroying 3D Renderer");
	App->menus->info.AddConsoleLog("Destroying Module Audio");

    TermCommunicationModule();
    //TermSpatialAudio();  // NO TERM FUNCTION :(
    TermMusicEngine();
    TermSoundEngine();
    TermStreamingManager();
    TermMemoryManager();

	return true;
}

bool ModuleAudio::InitMemoryManager()
{
    // MemSettings Initialization

    AkMemSettings memSettings;
    memSettings.uMaxNumPools = 20;

    if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
    {
        assert(!"Could not create the memory manager.");
        return false;
    }

    return true;
}

bool ModuleAudio::InitStreamingManager()
{

    // We're using the default Low-Level I/O implementation that's part
    // of the SDK's sample code, with the file package extension

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    // Customize the Stream Manager settings here.

    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }

    //
    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own. 
    //
    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

    // Customize the streaming device settings here.

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
    {
        assert(!"Could not create the streaming device and Low-Level I/O system");
        return false;
    }

    return true;
}

bool ModuleAudio::InitSoundEngine()
{
    //
    // Create the Sound Engine
    // Using default initialization parameters
    //

    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings(initSettings);
    AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

    if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
    {
        assert(!"Could not initialize the Sound Engine.");
        return false;
    }


	return true;
}

bool ModuleAudio::InitMusicEngine()
{
    //
    // Initialize the music engine
    // Using default initialization parameters
    //

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings(musicInit);

    if (AK::MusicEngine::Init(&musicInit) != AK_Success)
    {
        assert(!"Could not initialize the Music Engine.");
        return false;
    }

    return true;
}

bool ModuleAudio::InitSpatialAudio()
{
    //
   // Initialize Spatial Audio
   // Using default initialization parameters
   //

    AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 
    if (AK::SpatialAudio::Init(settings) != AK_Success)
    {
        assert(!"Could not initialize the Spatial Audio.");
        return false;
    }

    return true;
}

bool ModuleAudio::InitCommunications()
{
#ifndef AK_OPTIMIZED
    //
    // Initialize communications (not in release build!)
    //
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings(commSettings);
    if (AK::Comm::Init(commSettings) != AK_Success)
    {
        assert(!"Could not initialize communication.");
        return false;
    }

#endif // AK_OPTIMIZED

    return true;

}

void ModuleAudio::ProcessAudio()
{
    // Process bank requests, events, positions, RTPC, etc.
   
    AK::SoundEngine::RenderAudio();
}

bool ModuleAudio::TermCommunicationModule()
{
    #ifndef AK_OPTIMIZED
        //
        // Terminate Communication Services
        //
      AK::Comm::Term();
    #endif // AK_OPTIMIZED

    return true;
}

//bool ModuleAudio::TermSpatialAudio()
//{
//    //
//    // Terminate Spatial Audio
//    //
//
//    AK::SpatialAudio::Term();
//
//    return true;
//}

bool ModuleAudio::TermMusicEngine()
{
    //
    // Terminate the music engine
    //

    AK::MusicEngine::Term();

    return true;
}

bool ModuleAudio::TermSoundEngine()
{
    //
    // Terminate the sound engine
    //

    AK::SoundEngine::Term();

    return true;
}

bool ModuleAudio::TermStreamingManager()
{
    // Terminate the streaming device and streaming manager

    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    g_lowLevelIO.Term();

    if (AK::IAkStreamMgr::Get())
        AK::IAkStreamMgr::Get()->Destroy();


    return true;
}

bool ModuleAudio::TermMemoryManager()
{
    // Terminate the Memory Manager
    AK::MemoryMgr::Term();

    return true;
}
