#pragma once
#include "Globals.h"
#include "Application.h"
#include "Component.h"
#include "C_Transform.h"
#include "ModuleSceneIntro.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name);

	~GameObject();

	// ---------------------------------------------------------- IN PROGRESS
	/*GameObject(MeshData* mesh)
		: mesh_(mesh),
		local_(C_Transform::origin()),
			dirty_(true)
	{}

	void renderMesh(MeshData* mesh, C_Transform transform) {}

	void render(C_Transform parentWorld, bool dirty)
	{
		dirty |= dirty_;
		if (dirty)
		{
			world_ = local_.combine(parentWorld);
			dirty_ = false;
		}

		if (mesh_) renderMesh(mesh_, world_);

		for (int i = 0; i < children.size(); i++)
		{
			children[i]->render(world_, dirty);
		}
	}

	void setTransform(C_Transform local)
	{
		local_ = local;
		dirty_ = true;
	}*/
	
	void Update();
	Component* CreateComponent(Component::TYPE);
	Component* GetComponent(Component::TYPE);

	Component* GetComponentByNum(int i)
	{
		return components[i];
	}
	vector <Component*> GetComponents();


	GameObject* GetChild(int n);
	vector <GameObject*> GetChildren();

	bool AddChild(GameObject* child);

	C_Transform* transform = nullptr;

	bool active;
	string name;
	uint id = 0;

	uint id_count;
	GameObject* parent = nullptr;

private:
	// ---------------------------------------------------------- IN PROGRESS
	/*C_Transform world_;
	bool dirty_;

	C_Transform local_;
	MeshData* mesh_;*/

	vector <GameObject*> children;
	vector<Component*> components;

	Component* new_component;

};