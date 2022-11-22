#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Material.h"

#include <iostream>
#include <string>
#include <vector>

C_Material::C_Material(GameObject* gameObject) : name("Undefined"), Component(gameObject, C_TYPE::MATERIAL)
{
	textureID = 0;
}

C_Material::~C_Material()
{

}

void C_Material::SetTexture(const char* texture_name, uint textID)
{
	name = texture_name;

	textureID = textID;
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
		
		ImGui::Text("Texture ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", textureID);

	}

}
