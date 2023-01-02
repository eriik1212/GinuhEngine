#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include <vector>

#define NUM_COMPONENTS_TYPES 6

using namespace std;

class GameObject;

class Component
{
public:
	enum class C_TYPE
	{
		NONE,
		TRANSFORM,
		MESH,
		MATERIAL,
		CAMERA,
		// AUDIO
		AUDIO_LISTENER,
		AUDIO_SOURCE
	};


	Component(GameObject* game_object, C_TYPE type);
	virtual ~Component();

	bool IsEnabled() {
		return enabled;
	}

	virtual void Update();

	virtual void PrintGui();

	C_TYPE GetType() { return type; }

	string componentName;

protected:

	bool enabled = true;
	C_TYPE type = C_TYPE::NONE;
	GameObject* go = nullptr;

};