#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"
#include "ModuleAudio.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class C_AudioSource : public Component
{
public:
	C_AudioSource();
	C_AudioSource(GameObject* gameObject);
	~C_AudioSource();

	void Update() override;

	void PrintGui();

	GameObject* SourceGameObject;
};