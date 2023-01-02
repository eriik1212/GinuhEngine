#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioSource.h"

C_AudioSource::C_AudioSource() : Component(nullptr, C_TYPE::AUDIO_SOURCE)
{

}

C_AudioSource::C_AudioSource(GameObject* gameObject) : Component(gameObject, C_TYPE::AUDIO_SOURCE)
{
	SourceGameObject = gameObject;

	AppExtern->audio->RegisterGameObject(SourceGameObject->id);

}

C_AudioSource::~C_AudioSource()
{
	AppExtern->audio->UnregisterGameObject(SourceGameObject->id);

}

void C_AudioSource::Update()
{
}

void C_AudioSource::PrintGui()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioSourceEnabled", &enabled);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Remove Component ##source", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			go->RemoveComponent(this);

		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}
}
