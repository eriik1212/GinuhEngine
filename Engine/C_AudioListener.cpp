#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioListener.h"

C_AudioListener::C_AudioListener(GameObject* gameObject) : Component(gameObject, C_TYPE::AUDIO_LISTENER)
{
	ListenerGameObject = gameObject;
	listener_id = gameObject->id;

	AppExtern->audio->RegisterGameObject(listener_id);
	AppExtern->audio->SetDefaultListener(listener_id);

}

C_AudioListener::~C_AudioListener()
{
	AppExtern->audio->RemoveDefaultListener(listener_id);

	AppExtern->audio->UnregisterGameObject(listener_id);

	AppExtern->menus->info.AddConsoleLog("Destroying AudioListener");

}

void C_AudioListener::Update()
{
	AppExtern->audio->SetListenerPos(ListenerGameObject, listener_id);
}

void C_AudioListener::PrintGui()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioListenerEnabled", &enabled);

		ImGui::Spacing();
		ImGui::Spacing();

		if(enabled)
		{
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "'%s' is now a LISTENER", go->name.c_str());
			AppExtern->audio->SetDefaultListener(listener_id);

		}
		else
		{
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "'%s' is NOT a LISTENER", go->name.c_str());
			AppExtern->audio->RemoveDefaultListener(listener_id);

		}

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Remove Component ##listener", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			go->RemoveComponent(this);

		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}
}
