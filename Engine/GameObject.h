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

class C_Transform;

class GameObject
{
public:
	GameObject(GameObject* parent, string name);

	~GameObject();
	
	void Enable();
	void Update();
	void Disable();

	bool isActive();

	Component* CreateComponent(Component::C_TYPE);
	Component* GetComponent(Component::C_TYPE);

	Component* GetComponentByNum(int i);

	vector <Component*> GetComponents();

	void RemoveComponent(Component* component);

	GameObject* GetChild(int n);
	vector <GameObject*> GetChildren();

	bool AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	void RelocateGO(GameObject* relocatedParent);
	bool isChild(GameObject* from);

	bool active;
	string name;
	uint id = 0;

	uint id_count;
	GameObject* parent;
	C_Transform* transform = nullptr;

	vector <GameObject*> children;

private:

	vector<Component*> components;

	Component* new_component;

	Component* toDelete = nullptr;
};