#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"
#include "ModuleAudio.h"
#include "AudioEvent.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AudioEvent;

class C_AudioSource : public Component
{
public:

	C_AudioSource(GameObject* gameObject);
	~C_AudioSource();

	void Update() override;

	void PrintGui();

	GameObject* SourceGameObject;
	unsigned int source_id;

	string audio;

	void PlayEvent();

private:

	void StopEvent() const;
	void PauseEvent() const;
	void ResumeEvent() const;

	bool toPlayEvent = false;
	unsigned toPlayEventId = 0;

	void PrintAudioList();

	unsigned int playID;
	float volume;
	bool playOnAwake;
	bool isPlaying = false;

};