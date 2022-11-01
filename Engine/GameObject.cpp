#include "GameObject.h"
#include "Application.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, bool is_camera) : name(name), tag(tag), is_camera(is_camera)
{
	id = AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	if (parent != nullptr) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		RELEASE(components[i]);
	}
	components.clear();

	for (size_t i = 0; i < children.size(); i++)
	{
		RELEASE(children[i]);
	}
	children.clear();
}

bool GameObject::AddChild(GameObject* child)
{
	if (!child) return false;
	if (child->parent == this) return false;

	GameObject* p = parent;

	while (p)
	{
		if (p == child) return false;

		p = p->parent;
	}

	children.push_back(child);

	if (child->parent) child->parent->RemoveChild(child);

	child->parent = this;

	child->transform->parentGlobalTransform = transform->GetGlobalTransform();
	child->transform->UpdateTransform();

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	for (size_t i = 0; i < children.size(); ++i)
	{
		if (children[i] == child) children.erase(children.begin() + i);
	}
	child->parent = nullptr;
}

uint GameObject::AddGameObject(GameObject* go)
{
	game_objects[id_counter] = go;
	return id_counter++;
}