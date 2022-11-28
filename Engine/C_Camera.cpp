#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Camera.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, C_TYPE::MESH)
{
	name = "Camera";
}

C_Camera::~C_Camera()
{

}

void C_Camera::Update()
{

}


void C_Camera::PrintGui()
{

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();

		ImGui::Spacing();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Remove Component", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			go->RemoveComponent(this);
		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}

}