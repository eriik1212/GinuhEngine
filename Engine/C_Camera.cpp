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
		if (ImGui::Button("Remove Component"))
			go->RemoveComponent(this);
		ImGui::Spacing();

	}

}