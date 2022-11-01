#pragma once
#include "Module.h"
#include "Globals.h"
class GameObject;

class ModuleMenuGameObject : public Module
{
public:
	ModuleMenuGameObject(Application* app, bool start_enabled = true);
	~ModuleMenuGameObject();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void SetSelectedGameObject(GameObject* go)
	{
		selected_go = go;
	}

	GameObject* GetSelectedGameObject()
	{
		return selected_go;
	}

	GameObject CreateGameObject(aiNode* node, const aiScene* scene, std::string path, GameObject* parent);

	GameObject* rootGameObject = nullptr;
private:

	GameObject* selected_go = nullptr;
	//static GameObject* returnGameObject;

};

