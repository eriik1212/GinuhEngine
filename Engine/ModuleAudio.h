#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"
#include "AudioEvent.h"

#include <AK/SoundEngine/Common/AkMemoryMgr.h>         // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>            // Default memory and stream managers  

#include <AK/SoundEngine/Common/AkStreamMgrModule.h>   // Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>           // Thread defines

#include <AK/SoundEngine/Common/AkSoundEngine.h>	   // SoundEngine
#include <AK/MusicEngine/Common/AkMusicEngine.h>	   // MusicEngine

#include <AK/SoundEngine/Common/AkTypes.h>

#include <AK/SpatialAudio/Common/AkSpatialAudio.h>              // Spatial Audio

#ifdef _DEBUG
	// Input libraries
	#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkSoundEngine.lib")
	#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkMusicEngine.lib")
	#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "Wwise/Debug(StaticCRT)/lib/AkStreamMgr.lib")

	// External library dependencies
	#pragma comment( lib, "Wwise/dinput8.lib") // Microsoft DirectX DirectInput.Needed by Motion to support DirectInput devices.
	#pragma comment( lib, "Wwise/dsound.lib")  // Microsoft DirectX DirectSound library
	#pragma comment( lib, "Wwise/dxguid.lib")  // Microsoft DirectX Audio GUIDs
	#pragma comment( lib, "Wwise/WinMM.Lib")  // Microsoft Multimedia API. Needed by Motion for internal device targeting management. 
	#pragma comment( lib, "Wwise/WS2_32.Lib")  // Microsoft Winsock 2 library (used for Wwise profiling) 
	#pragma comment( lib, "Wwise/XInput.lib")  // Microsoft XInput. Needed by Motion to support XInput devices (Xbox controller).
#else
#define AK_OPTIMIZED
	// Input libraries
	#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkMusicEngine.lib")
	#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkStreamMgr.lib")
	#pragma comment( lib, "Wwise/Release(StaticCRT)/lib/AkSoundEngine.lib")

	// External library dependencies
	#pragma comment( lib, "Wwise/dinput8.lib") // Microsoft DirectX DirectInput.Needed by Motion to support DirectInput devices.
	#pragma comment( lib, "Wwise/dsound.lib")  // Microsoft DirectX DirectSound library
	#pragma comment( lib, "Wwise/dxguid.lib")  // Microsoft DirectX Audio GUIDs
	#pragma comment( lib, "Wwise/WinMM.Lib")  // Microsoft Multimedia API. Needed by Motion for internal device targeting management. 
	//#pragma comment( lib, "Wwise/WS2_32.Lib")  // Microsoft Winsock 2 library (used for Wwise profiling) 
	#pragma comment( lib, "Wwise/XInput.lib")  // Microsoft XInput. Needed by Motion to support XInput devices (Xbox controller).
#endif

//#include <Win32/AkFilePackageLowLevelIOBlocking.h>                    // Sample low-level I/O implementation

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
	#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#define MAX_LISTENERS 1

class AudioEvent;

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	// GO
	void RegisterGameObject(unsigned int id);
	void UnregisterGameObject(unsigned int id);

	// Listeners 
	void SetDefaultListener(const AkGameObjectID id);
	void AddListeners(unsigned int emitter_id, const AkGameObjectID listener_id);
	void SetListenerPos(GameObject* listener, unsigned int id);

	// Events
	void PostEvent(AudioEvent* event, unsigned int id);
	void StopEvent(const AudioEvent* event, unsigned int id);

private:

	// We're using the default Low-Level I/O implementation that's part
	// of the SDK's sample code, with the file package extension
	//CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	//// ----------------------------------- Init Functions
	bool InitMemoryManager();
	bool InitStreamingManager();
	bool InitSoundEngine();
	bool InitMusicEngine();
	//bool InitSpatialAudio(); // INIT OK!
	//bool InitCommunications();

	//// -----------------------------------

	void ProcessAudio();

	//// ----------------------------------- 
	//
	//// ----------------------------------- Term Functions
	//bool TermCommunicationModule();
	//bool TermSpatialAudio();  // NO TERM FUNCTION :(
	bool TermMusicEngine();
	bool TermSoundEngine();
	bool TermStreamingManager();
	bool TermMemoryManager();
};

#endif // __ModuleAudio_H__