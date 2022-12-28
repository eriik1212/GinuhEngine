#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"


#ifdef _DEBUG  // Profile build configuration must be loaded instead of Debug
	#pragma comment( lib, "AK/Debug/lib/AkSoundEngine.lib")
	#pragma comment( lib, "AK/Debug/lib/AkMusicEngine.lib")
	#pragma comment( lib, "AK/Debug/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "AK/Debug/lib/AkStreamMgr.lib")
	#pragma comment( lib, "AK/Debug/lib/AkSpatialAudio.lib")
	#pragma comment( lib, "AK/Debug/lib/CommunicationCentral.lib")


	// External library dependencies
	#pragma comment( lib, "AK/dinput8.lib") // Microsoft DirectX DirectInput.Needed by Motion to support DirectInput devices.
	#pragma comment( lib, "AK/dsound.lib")  // Microsoft DirectX DirectSound library
	#pragma comment( lib, "AK/dxguid.lib")  // Microsoft DirectX Audio GUIDs
	#pragma comment( lib, "AK/WinMM.Lib")  // Microsoft Multimedia API. Needed by Motion for internal device targeting management. 
	#pragma comment( lib, "AK/WS2_32.Lib")  // Microsoft Winsock 2 library (used for Wwise profiling) 
	#pragma comment( lib, "AK/XInput.lib")  // Microsoft XInput. Needed by Motion to support XInput devices (Xbox controller). 
#else
#define AK_OPTIMIZED
	#pragma comment( lib, "AK/Release/lib/AkSoundEngine.lib")
	#pragma comment( lib, "AK/Release/lib/AkMusicEngine.lib")
	#pragma comment( lib, "AK/Release/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "AK/Release/lib/AkStreamMgr.lib")
	#pragma comment( lib, "AK/Debug/lib/AkSpatialAudio.lib")

	// External library dependencies
	#pragma comment( lib, "AK/dinput8.lib") // Microsoft DirectX DirectInput.Needed by Motion to support DirectInput devices.
	#pragma comment( lib, "AK/dsound.lib")  // Microsoft DirectX DirectSound library
	#pragma comment( lib, "AK/dxguid.lib")  // Microsoft DirectX Audio GUIDs
	#pragma comment( lib, "AK/WinMM.Lib")  // Microsoft Multimedia API. Needed by Motion for internal device targeting management.
	#pragma comment( lib, "AK/XInput.lib")  // Microsoft XInput. Needed by Motion to support XInput devices (Xbox controller). 
#endif

#include "AK/include/Win32/AkFilePackageLowLevelIOBlocking.h" // Sample low-level I/O implementation

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
	//CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	// ----------------------------------- Init Functions
	bool InitMemoryManager();
	bool InitStreamingManager();
	bool InitSoundEngine();
	bool InitMusicEngine();
	bool InitSpatialAudio(); // INIT OK!
	bool InitCommunications();

	// -----------------------------------

	void ProcessAudio();

	// ----------------------------------- 
	
	// ----------------------------------- Term Functions
	bool TermCommunicationModule();
	//bool TermSpatialAudio();  // NO TERM FUNCTION :(
	bool TermMusicEngine();
	bool TermSoundEngine();
	bool TermStreamingManager();
	bool TermMemoryManager();
};

#endif // __ModuleAudio_H__