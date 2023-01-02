#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioListener.h"

C_AudioListener::C_AudioListener() : Component(nullptr, C_TYPE::AUDIO_LISTENER)
{

}

C_AudioListener::C_AudioListener(GameObject* gameObject) : Component(gameObject, C_TYPE::AUDIO_LISTENER)
{

}

C_AudioListener::~C_AudioListener()
{
}

void C_AudioListener::Update()
{
}

void C_AudioListener::PrintGui()
{
	if (ImGui::CollapsingHeader("Audio Listener", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioListenerEnabled", &enabled);

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
