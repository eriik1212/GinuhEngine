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

void C_Material::SetTexture(const char* texture_name, MeshData* meshNode)
{
	name = texture_name;
	mesh = meshNode;
	textureID = meshNode->texture_id;
	auxID = meshNode->texture_id;
}

void C_Material::Update()
{

}

void C_Material::PrintGui()
{

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		if (ImGui::Button("Remove"))
		{
			mesh->texture_id = 0;
			go->RemoveComponent(this);
		}

		ImGui::Text("Texture Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());
		
		ImGui::Text("Texture ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", textureID);

	}

	UpdateStatus();

}

void C_Material::UpdateStatus()
{

	if (!enabled)
	{
		//auxID = textureID;
		auxID = 0;
		mesh->texture_id = auxID;
	}
	else
	{
		mesh->texture_id = textureID;
		auxID = mesh->texture_id;
	}
}
