#pragma once
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "ModuleSceneIntro.h"

GameObject::GameObject(GameObject* parent, string name) : parent(parent), active(true)
{
	AppExtern->scene_intro->gameObjects[id_count] = this;
	if (name != "")
		this->name = name;
	else if( name == "" && parent != nullptr)
	{
		this->name = parent->name;
	}
	id = id_count;
	id_count++;

	transform = dynamic_cast<C_Transform*>(CreateComponent(Component::C_TYPE::TRANSFORM));

	go.push_back(this);

	if (parent != nullptr) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i] != nullptr)
		{
			delete components[i];
			components[i] = nullptr;

		}
	}
	components.clear();

	for (uint j = 0; j < this->GetChildren().size(); j++)
	{
		RemoveChild(this->GetChild(j));
	}
	this->GetChildren().clear();

	if(this->parent != nullptr)
		this->parent->RemoveChild(this);	//Little strange (GO->Parent->RemoveChild() ---> Is itself) But works!!
}

Component* GameObject::CreateComponent(Component::C_TYPE type)
{
	Component* new_component;

	switch (type)
	{
	case Component::C_TYPE::TRANSFORM:
		new_component = new C_Transform(this);
		break;

	case Component::C_TYPE::MESH:
		new_component = new C_Mesh(this);
		break;
	case Component::C_TYPE::MATERIAL:
		new_component = new C_Material(this);
		break;
	case Component::C_TYPE::CAMERA:
		new_component = new C_Camera(this);
		//AppExtern->scene_intro->SetAsGameCam(dynamic_cast<C_Camera*>(new_component));
		break;
	default:
		LOG("component type error");
		break;
	}
	components.push_back(new_component);

	return new_component;
}

Component* GameObject::GetComponent(Component::C_TYPE type)
{
	for (auto component : components)
	{
		if (component->GetType() == type)
		{
			return component;
		}
	}
	return NULL;
}

Component* GameObject::GetComponentByNum(int i)
{
	return components[i];
}

vector <GameObject*> GameObject::GetChildren()
{
	return children;
}

GameObject* GameObject::GetChild(int n)
{
	return children[n];
}

bool GameObject::AddChild(GameObject* child)
{
	children.push_back(child);

	return true;
}

void GameObject::RemoveChild(GameObject* child)
{
	children.erase(find(children.begin(), children.end(), child));
}

void GameObject::RelocateGO(GameObject* relocatedParent)
{
	if (this != nullptr)
	{
		if (parent != nullptr)
		{
			if (isChild(relocatedParent))
			{
				return;
			}
			parent->RemoveChild(this);
			parent = relocatedParent;
			relocatedParent->AddChild(this);
		}
	}
}

bool GameObject::isChild(GameObject* fromThis)
{
	if (this->parent == fromThis)
		return true;
	else
		return false;
}

void GameObject::DeleteGO()
{
}

void GameObject::Enable()
{
	active = true;

	if (parent != nullptr)
		parent->Enable();
}

void GameObject::Update()
{
	if (toDelete != nullptr)
	{
		components.erase(find(components.begin(), components.end(), toDelete));
		delete toDelete;
		toDelete = nullptr;
	}

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->IsEnabled())
			components[i]->Update();
	}
}


void GameObject::Disable()
{
	active = false;
}

bool GameObject::isActive()
{
	return active;
}

vector <Component*> GameObject::GetComponents()
{
	return components;
}

void GameObject::RemoveComponent(Component* component)
{
	toDelete = component;
}
