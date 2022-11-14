#pragma once
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Texture.h"
#include "ModuleSceneIntro.h"

GameObject::GameObject(GameObject* parent, std::string name) : parent(parent), active(true)
{
	App->scene_intro->gameObjects[id_count] = this;
	this->name = name;
	id = id_count;
	id_count++;

	transform = dynamic_cast<C_Transform*>(CreateComponent(Component::C_TYPE::TRANSFORM));

	if (parent != nullptr) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] != nullptr)
		{
			delete components[i];
			components[i] = nullptr;

		}
	}
	components.clear();

}

Component* GameObject::CreateComponent(Component::C_TYPE type)
{
	switch (type)
	{
	case Component::C_TYPE::TRANSFORM:
		new_component = new C_Transform(this);
		break;

	case Component::C_TYPE::MESH:
		new_component = new C_Mesh(this);
		break;
	case Component::C_TYPE::TEXTURE:
		new_component = new C_Texture(this);
		break;
		/*case Component::TYPE::CAMERA:
			new_component = new C_Camera(this);
			break;*/
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

void GameObject::Enable()
{
	active = true;

	if (parent != nullptr)
		parent->Enable();
}

void GameObject::Update()
{
	for (uint i = 0; i < components.size(); i++)
	{
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