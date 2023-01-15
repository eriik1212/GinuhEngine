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

	InitMemoryManager();
	InitStreamingManager();
	InitSoundEngine();
	InitMusicEngine();
	//InitSpatialAudio();
	//InitCommunications();

	g_lowLevelIO.SetBasePath(AKTEXT("Assets/Wwise/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	GetAudioID();

	AkBankID bankID = 0;

	if (AK::SoundEngine::LoadBank(L"Init.bnk", bankID) != AK_Success)
	{
		App->menus->info.AddConsoleLog("Couldn't find the bank: Init.bnk");
		return false;
	}

	if (AK::SoundEngine::LoadBank(L"Demo.bnk", bankID) != AK_Success)
	{
		App->menus->info.AddConsoleLog("Couldn't find the bank: Init.bnk");
		return false;
	}
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

	//TermCommunicationModule();
	//TermSpatialAudio();  // NO TERM FUNCTION :(
	TermMusicEngine();
	TermSoundEngine();
	TermStreamingManager();
	TermMemoryManager();

	return true;
}

bool ModuleAudio::InitMemoryManager()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	return true;
}

bool ModuleAudio::InitStreamingManager()
{
	//
   // Create and initialize an instance of the default streaming manager. Note
   // that you can override the default streaming manager with your own. 
   //

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

//bool ModuleAudio::InitCommunications()
//{
//#ifndef AK_OPTIMIZED
//	//
//	// Initialize communications (not in release build!)
//	//
//	AkCommSettings commSettings;
//	AK::Comm::GetDefaultInitSettings(commSettings);
//	if (AK::Comm::Init(commSettings) != AK_Success)
//	{
//		assert(!"Could not initialize communication.");
//		return false;
//	}
//#endif // AK_OPTIMIZED
//
//	return true;
//}

void ModuleAudio::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

//bool ModuleAudio::TermCommunicationModule()
//{
//#ifndef AK_OPTIMIZED
//	//
//	// Terminate Communication Services
//	//
//
//	AK::Comm::Term();
//#endif // AK_OPTIMIZED
//
//	return true;
//}

bool ModuleAudio::TermSpatialAudio()
{
	//
	// Terminate Spatial Audio
	//

	//AK::SpatialAudio::Term();

	return true;
}

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

void ModuleAudio::GetAudioID()
{
	std::ifstream filePath("Assets/Wwise/Wwise_IDs.h");

	string line;

	while (std::getline(filePath, line))
	{
		if (line.find("EVENTS") != string::npos)
		{
			while (std::getline(filePath, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					events.push_back(line);
				}
			}
		}
		else if (line.find("BANKS") != string::npos)
		{
			while (std::getline(filePath, line))
			{
				if (line.find("}") != string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					soundBanks.push_back(line);
				}
			}
		}
	}
}

void ModuleAudio::RegisterGameObject(unsigned int id)
{
	AK::SoundEngine::RegisterGameObj(id);

}

void ModuleAudio::UnregisterGameObject(unsigned int id)
{
	AK::SoundEngine::UnregisterGameObj(id);

}

void ModuleAudio::SetDefaultListener(const AkGameObjectID id)
{
	AK::SpatialAudio::RegisterListener(id);
	AK::SoundEngine::SetDefaultListeners(&id, MAX_LISTENERS);


}

void ModuleAudio::RemoveDefaultListener(const AkGameObjectID id)
{
	AK::SpatialAudio::UnregisterListener(id);
	AK::SoundEngine::RemoveDefaultListener(id);

}

void ModuleAudio::AddListeners(unsigned int emitter_id, const AkGameObjectID listener_id)
{
	AK::SoundEngine::SetListeners(emitter_id, &listener_id, MAX_LISTENERS);

}

void ModuleAudio::SetListenerPos(GameObject* listener, unsigned int id)
{
	// Position of the listener
	float3 pos = listener->transform->transform.position;

	// Bc we only want to know pos and orientation, we use "AkSoundPosition". "AkTransform" is more 'complex' (scale..., etc)
	AkSoundPosition listenerPosition;
	listenerPosition.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	listenerPosition.SetPosition(pos.x, pos.y, pos.z);

	AK::SoundEngine::SetPosition(id, listenerPosition);
}

void ModuleAudio::SetSourcePos(GameObject* source, unsigned int id)
{
	// Position of the listener
	float3 pos = source->transform->transform.position;

	// Bc we only want to know pos and orientation, we use "AkSoundPosition". "AkTransform" is more 'complex' (scale..., etc)
	AkSoundPosition sourcePosition;
	sourcePosition.SetOrientation({ 0,0,-1 }, { 0,1,0 });
	sourcePosition.SetPosition(pos.x, pos.y, pos.z);

	AK::SoundEngine::SetPosition(id, sourcePosition);
}

AkPlayingID ModuleAudio::PostEvent(const char* eventName, unsigned int source_id)
{
	AkPlayingID playingID = AK::SoundEngine::PostEvent(eventName, source_id);
	if (playingID == AK_INVALID_PLAYING_ID)
	{
		App->menus->info.AddConsoleLog("Post event %s failed", eventName);
		return -1;
	}
	return playingID;
}

void ModuleAudio::StopEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Stop, id);
}

void ModuleAudio::PauseEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Pause, id);
}

void ModuleAudio::ResumeEvent(const char* eventName, unsigned int id)
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName, AK::SoundEngine::AkActionOnEventType_Resume, id);
}