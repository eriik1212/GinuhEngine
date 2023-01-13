#ifndef __AUDIOEVENT_H__
#define __AUDIOEVENT_H__

#include "SoundBank.h"
#include <string>

using namespace std;

class AudioEvent
{
public:

	AudioEvent();

	string name;
	unsigned int id = 0;

	int event_id = 0L;			// If event is playing, is different from 0L
	bool isSound3D = false;
	float maxAttenuation = 0.0f;

	bool IsPlaying() const;			
	void Unload();						
	void Unloaded();					
	bool IsReadyToUnload() const;

	SoundBank* soundBnk = nullptr;

private:

	bool unload = false;
};

#endif