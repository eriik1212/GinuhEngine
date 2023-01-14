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
	std::ifstream file("Assets/Wwise/Wwise_IDs.h");

	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("EVENTS") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != std::string::npos)
				{
					line = line.substr(0, line.find("=") - 1);
					line = line.substr(line.find_last_of(" ") + 1, line.length());

					events.push_back(line);
				}
			}
		}
		else if (line.find("BANKS") != std::string::npos)
		{
			while (std::getline(file, line))
			{
				if (line.find("}") != std::string::npos)
				{
					break;
				}
				else if (line.find("AkUniqueID") != std::string::npos)
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
	AK::SoundEngine::SetDefaultListeners(&id, MAX_LISTENERS);
}

void ModuleAudio::AddListeners(unsigned int emitter_id, const AkGameObjectID listener_id)
{
	AK::SoundEngine::SetListeners(emitter_id, &listener_id, MAX_LISTENERS);

}

void ModuleAudio::SetListenerPos(GameObject* listener, unsigned int id)
{
	// Position of the listener
	float3 pos;
	pos.x = listener->transform->transform.position.x;
	pos.y = listener->transform->transform.position.y;
	pos.z = listener->transform->transform.position.z;

	/*AkTransform listenerTransform;
	listenerTransform.Set(pos, front, top);*/

	// Bc we only want to know pos and orientation, we use "AkSoundPosition". "AkTransform" is more 'complex' (scale..., etc)
	AkSoundPosition listenerPosition;
	listenerPosition.SetPosition(pos.x, pos.y, pos.z);

	AK::SoundEngine::SetPosition(id, listenerPosition);
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

bool ModuleAudio::IsSoundBankInit()
{
	return initSoundBank;
}

void ModuleAudio::InitSoundBank()
{
	initSoundBank = true;
}

bool ModuleAudio::IsSoundBank(string &file)
{
	size_t extension = file.find_last_of('.');
	if (extension != string::npos && file.substr(extension + 1).compare("bnk") == 0)
		return true;

	return false;
}

//unsigned int ModuleAudio::GetBnkInfo(string soundbank_path)
//{
//	char* buf;
//	AkUInt32 ret = 0;
//
//	string json_file = soundbank_path.substr(0, soundbank_path.find_last_of('.')) + ".json"; // Changing .bnk with .json
//	if (App->files_manager->LoadFile(json_file.c_str(), &buf))
//	{
//		Data general_info(buf);
//		Data sb_info = general_info.GetJObject("SoundBanksInfo").GetArray("SoundBanks", 0);
//
//		// Soundbank Information
//		SoundBank* soundbank = new SoundBank();
//		soundBankList.push_back(soundbank);
//
//		ret = soundbank->id = stoul(sb_info.GetString("Id"));
//		soundbank->name = sb_info.GetString("ShortName");
//		soundbank->path = soundbank_path;
//
//		// Is Init Soundbank?
//		if (soundbank->name.compare("Init") == 0)
//			initSoundBnk = soundbank;
//
//		// Events Information
//		uint num_events = sb_info.GetArraySize("IncludedEvents");
//		for (uint i = 0; i < num_events; ++i)
//		{
//			Data events_info = sb_info.GetArray("IncludedEvents", i);
//
//			AudioEvent* a_event = new AudioEvent();
//			soundbank->events.push_back(a_event);
//			a_event->soundBnk = soundbank;
//
//			a_event->id = stoul(events_info.GetString("Id"));
//			a_event->name = events_info.GetString("Name");
//
//			const char* attenuation = events_info.GetString("MaxAttenuation");
//			if (attenuation != nullptr)
//			{
//				a_event->maxAttenuation = stof(attenuation);
//				if (a_event->maxAttenuation != 0.0f)
//					a_event->isSound3D = true;
//			}
//		}
//
//		delete buf; // Freeing memory
//	}
//
//	return ret;
//}
