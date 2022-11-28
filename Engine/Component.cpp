#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

Component::Component(GameObject* game_object, C_TYPE type) : enabled(true), go(game_object), type(type), componentName("")
{
	switch (type)
	{
	case Component::C_TYPE::NONE:
		componentName = "None";
		break;
	case Component::C_TYPE::TRANSFORM:
		componentName = "Transform";
		break;
	case Component::C_TYPE::MESH:
		componentName = "Mesh";
		break;
	case Component::C_TYPE::MATERIAL:
		componentName = "Material";
		break;
	case Component::C_TYPE::CAMERA:
		componentName = "Camera";
		break;
	default:
		componentName = "";
		break;
	}
}

Component::~Component()
{
}

void Component::Update()
{
}

void Component::PrintGui()
{

}