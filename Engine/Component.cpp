#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleWindow.h"

Component::Component(GameObject* game_object, TYPE type) : go(game_object), type(type)
{
}

Component::~Component()
{
}

void Component::ComponentOptions(bool undestructable)
{
	ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
	if (ImGui::BeginPopup("Component Options"))
	{
		ImGui::Text("Component Options");
		ImGui::Separator();
		if (!undestructable)
		{
			if (ImGui::Selectable("Destroy Component"))
			{
				GetGameObject()->RemoveComponent(this);
			}
		}
		ImGui::EndPopup();
	}
	/*if (!ImGuiH::CheckMouseInPopUp(ImVec2(app->window->width - 200.0f, ori.y), ImVec2(200.0f, 100.0f)))
	{
		popUpOpen = false;
		ImGui::CloseCurrentPopup();
	}*/
}