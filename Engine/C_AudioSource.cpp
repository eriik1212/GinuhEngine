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
	AppExtern->audio->UnregisterGameObject(source_id);

}

void C_AudioSource::Update()
{
	if (toPlayEvent)
	{
		PlayEvent(toPlayEventId);
		toPlayEvent = false;
	}
}

void C_AudioSource::PrintGui()
{
	if (ImGui::CollapsingHeader("Audio Source", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##audioSourceEnabled", &enabled);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.8f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.8f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.0f, 1.0f));

		// Add new audio source
		if (ImGui::Button("+"))
		{
			eventsList.push_back(emptyEvent);
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));

		// Remove last audio source added
		if (ImGui::Button("-") && !eventsList.empty())
		{
			if (eventsList.back() != nullptr)
			{
				StopEvent(eventsList.size() - 1);
				//eventsList.back()->Unload();       // Unload Resource
			}
			eventsList.pop_back(); // Pop Back Event
		}

		ImGui::PopStyleColor(6);

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

void C_AudioSource::PlayEvent(unsigned int index) const
{
	AppExtern->audio->PostEvent(eventsList[index], source_id);

}

void C_AudioSource::StopEvent(unsigned int index) const
{
	AppExtern->audio->StopEvent(eventsList[index], source_id);

}

void C_AudioSource::StopAllEvents() const
{
	for (int i = 0; i < eventsList.size(); ++i)
	{
		StopEvent(i);

	}
}

void C_AudioSource::PlayAudio(unsigned int audioId)
{
	if (audioId < eventsList.size())
	{
		toPlayEvent = true;
		toPlayEventId = audioId;
	}
}

void C_AudioSource::PrintAudioList()
{
	std::vector<AudioEvent*> events;

	// List of different events
	unsigned int index = 0;

	for (vector<AudioEvent*>::iterator currentEvent = eventsList.begin(); currentEvent != eventsList.end(); ++currentEvent)
	{
		ImGui::Text("ID: %d", index);
		ImGui::Text("Event: ");
		
		ImGui::SameLine();

		string eventName = "";

		if (*currentEvent != nullptr)
			eventName = (*currentEvent)->name.c_str();

		eventName += "##" + to_string(index);

		// Print Options
		if (ImGui::BeginMenu(eventName.c_str()))
		{
			for (vector<AudioEvent*>::iterator it = events.begin(); it != events.end(); ++it)
			{
				if (ImGui::MenuItem((*it)->name.c_str()))
				{
					// First, unloading previous Soundbank for old audio event.
					if ((*currentEvent) != nullptr)
						(*currentEvent)->Unload();

					// Now, loading new bank: first Init bank if it has been not loaded 
					if (!AppExtern->audio->IsSoundBankInit())
						AppExtern->audio->InitSoundBank();

					// Finally, the soundbank to the corresponding new audio event
					AppExtern->audio->LoadSoundBnk((*it)->soundBnk->path.c_str());

					eventsList[index] = *it;  // Updating list of events
				}
			}
			ImGui::EndMenu();
		}

		ImGui::Spacing();

		// Play and stop buttons for each event entry on Component
		string playButton = "PLAY##" + to_string(index);
		string stopButton = "STOP##" + to_string(index);

		if (ImGui::Button(playButton.c_str()))
			PlayEvent(index);

		ImGui::SameLine();

		if (ImGui::Button(stopButton.c_str()))
			StopEvent(index);

		ImGui::Spacing();
		ImGui::Separator();

		++index;
	}
}
