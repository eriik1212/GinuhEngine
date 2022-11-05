#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint CreateGameObject(GameObject* parent, std::string name);

	static std::map<uint, GameObject*> gameObjects;

	GameObject* gameobject_selected = nullptr;
};
