#pragma once

#include "Component.h"
#include "C_Transform.h"
//#include "C_MeshRenderer.h"
//#include "C_Camera.h"
#include "Globals.h"

#include <vector>
#include <string>
#include <map>

class C_Transform;

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default", bool is_camera = false);
	~GameObject();

	//TODO: Could do with an array of functions that creates a specific component
	Component* AddComponent(Component::TYPE type)
	{
		Component* new_component;
		switch (type)
		{
		case Component::TYPE::TRANSFORM:
			new_component = new C_Transform(this);
			break;
		default:
			LOG("component type error");
			break;
		}
		components.push_back(new_component);
		return new_component;
	}
	Component* GetComponent(Component::TYPE type)
	{
		for (auto component : components) // check all components
		{
			if (component->GetType() == type)
			{
				return component;
			}
		}
		return NULL; // component not find
	}
	/*void RemoveComponent(Component::TYPE type)
	{
		if (GetComponent(type) != NULL)
		{
			int pos_in_array = 0;
			for (uint i = 0; i < (uint)Component::TYPE::NUM_OF_COMPS; i++)
			{
				if (GetComponent((Component::TYPE)i))
				{
					if (type == (Component::TYPE)i)
					{
						components.erase(components.begin() + pos_in_array);
						return;
					}
					else pos_in_array++;
				}
			}
		}
		else return;
	}*/
	void RemoveComponent(Component* component)
	{
		int pos_in_array = 0;
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components.at(i) == component)
			{
				components.erase(components.begin() + pos_in_array);
				return;
			}
			else
			{
				pos_in_array++;
			}
		}
	}
	void SetSelectedGameObject(GameObject* go)
	{
		selected_go = go;
	}
	GameObject* GetSelectedGameObject()
	{
		return selected_go;
	}

	std::vector<Component*> GetComponentList()
	{
		return components;
	}

	bool AddChild(GameObject* child);
	std::vector<GameObject*> GetChilds()
	{
		return children;
	}

	bool SetParent(GameObject* parent);
	GameObject* GetParent()
	{
		return parent;
	}

	uint AddGameObject(GameObject* go);

	std::string name;
	bool enabled = true;
	bool visible = true;
	bool visible_on_editor = true;
	std::string tag;
	C_Transform* transform = nullptr;

	bool is_camera;

	std::map<uint, GameObject*> game_objects;

	uint id_counter = 1;
private:
	void RemoveChild(GameObject* child);

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	bool active = true;
	bool destroyed = false;

	uint id = 0; // ID =  0 is an invalid ID. First ID is 1

	//friend class C_Transform;
	friend class EW_Hierarchy;

	GameObject* selected_go = nullptr;
};