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
	enum class TYPE
	{
		NONE,
		TRANSFORM,
		MESH,
		TEXTURE,
	};


	Component(GameObject* game_object, TYPE type);
	virtual ~Component();

	virtual void PrintGui();

	TYPE GetType() { return type; }

private:

	TYPE type = TYPE::NONE;
	GameObject* go = nullptr;
};