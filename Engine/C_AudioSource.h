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

	vector<AudioEvent*> eventsList;
	AudioEvent* emptyEvent = nullptr;

private:

	void PlayEvent(unsigned int index) const;
	void StopEvent(unsigned int index) const;
	void StopAllEvents() const;

	void PrintAudioList();
};