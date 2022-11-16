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
	};


	Component(GameObject* game_object, C_TYPE type);
	virtual ~Component();

	bool IsEnabled() {
		return enabled;
	}

	virtual void Update();

	virtual void PrintGui();

	C_TYPE GetType() { return type; }

protected:

	bool enabled;
	C_TYPE type = C_TYPE::NONE;
	GameObject* go = nullptr;
};