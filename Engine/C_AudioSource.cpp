#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_AudioSource.h"

C_AudioSource::C_AudioSource(GameObject* gameObject) : Component(gameObject, C_TYPE::AUDIO_SOURCE)
{
	SourceGameObject = gameObject;
	source_id = gameObject->id;

	AppExtern->audio->RegisterGameObject(source_id);
}

C_AudioSource::~C_AudioSource()
{
	StopEvent();

	AppExtern->audio->UnregisterGameObject(source_id);

	AppExtern->menus->info.AddConsoleLog("Destroying AudioSource");
}

void C_AudioSource::Update()
{
	AppExtern->audio->SetSourcePos(SourceGameObject, source_id);
}

void C_AudioSource::PrintGui()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioSourceEnabled", &enabled);

		ImGui::Spacing();
		ImGui::Spacing();

		PrintAudioList();

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

void C_AudioSource::PlayEvent()
{
	playID = AppExtern->audio->PostEvent(audio.c_str(), source_id);

}

void C_AudioSource::StopEvent() const
{
	AppExtern->audio->StopEvent(audio.c_str(), source_id);

}

void C_AudioSource::ResumeEvent() const
{
	AppExtern->audio->ResumeEvent(audio.c_str(), source_id);

}

void C_AudioSource::PauseEvent() const
{
	AppExtern->audio->PauseEvent(audio.c_str(), source_id);

}

void C_AudioSource::PrintAudioList()
{

	if (ImGui::BeginCombo("##AudioClip", audio.c_str()))
	{	
		for (int i = 0; i < AppExtern->audio->events.size(); i++)
		{
			if (ImGui::Selectable(AppExtern->audio->events[i].c_str()))
			{
				audio = AppExtern->audio->events[i];
				
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Spacing();

	if (ImGui::Button("Play"))
	{
		if(!isPlaying)
		{
			PlayEvent();
			isPlaying = true;
		}

	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		StopEvent();
		isPlaying = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Pause"))
	{
		PauseEvent();
		isPlaying = false;
	}

	ImGui::SameLine();

	if (ImGui::Button("Resume"))
	{
		ResumeEvent();
		isPlaying = true;
	}

	ImGui::Spacing();
	ImGui::Separator();
}
