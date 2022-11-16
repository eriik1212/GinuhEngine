#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

Component::Component(GameObject* game_object, C_TYPE type) : enabled(true), go(game_object), type(type)
{
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
