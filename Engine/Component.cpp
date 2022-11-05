#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

Component::Component(GameObject* game_object, TYPE type) : go(game_object), type(type)
{
}

Component::~Component()
{
}

void Component::OnGui()
{

}
