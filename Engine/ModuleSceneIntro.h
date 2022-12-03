#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "C_Camera.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool wireframe = false;
	bool textureEnabled = true;

	uint CreateGameObject(GameObject* parent, string name);
	float3 GetGOGlobalPos(GameObject* go);

	GameObject* SceneRoot;

	static map<uint, GameObject*> gameObjects;

	GameObject* gameobject_selected = nullptr;

private:

	void UpdateGO();
	void RecursiveUpdate(GameObject* goToUpdate);

};
