#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Material.h"

#include <iostream>
#include <string>
#include <vector>

C_Material::C_Material(GameObject* gameObject) : Component(gameObject, C_TYPE::MATERIAL)
{

}

C_Material::~C_Material()
{

}

void C_Material::SetTexture(const char* texture_name)
{
	name = texture_name;
}

void C_Material::PrintGui()
{

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		if (ImGui::Button("Remove"))
			go->RemoveComponent(this);

		ImGui::Text("Texture Name:");
		ImGui::SameLine();
		ImGui::Text(name);

	}

}
