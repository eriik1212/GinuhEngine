#pragma once
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "Component.h"
#include "C_Transform.h"

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

	GameObject* parent = nullptr;

private:
	vector <GameObject*> children;
	vector<Component*> components;

};