#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class C_AudioListener : public Component
{
public:
	C_AudioListener();
	C_AudioListener(GameObject* gameObject);
	~C_AudioListener();

	void Update() override;

	void PrintGui();

};