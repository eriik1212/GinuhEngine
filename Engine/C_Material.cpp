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
		const char* current_item = name.c_str();

		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();

		ImGui::Text("Texture Name:");
		ImGui::SameLine();
		ImGui::Text(current_item);
		
		ImGui::Text("Texture ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", ModuleFilesManager::loaded_textures[current_item]);

		ImGui::Spacing();

		// ----------------------------------------- CHANGE TEXTURE
		ImGui::TextColored(ImVec4(0,255,0,255), "Choose Texture: ");
		ImGui::Spacing();
		if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < ModuleFilesManager::allText.size(); n++)
			{
				bool is_selected = (current_item == ModuleFilesManager::allText[n].c_str());
				if (ImGui::Selectable(ModuleFilesManager::allText[n].c_str(), is_selected))
				{
					current_item = ModuleFilesManager::allText[n].c_str();
					name = current_item;
				}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Spacing();
		// Show Texture Image
		ImGui::ImageButton((ImTextureID)ModuleFilesManager::loaded_textures[current_item], ImVec2(100.0f, 100.0f));
		
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			mesh->texture_id = 0;
			go->RemoveComponent(this);
		}
		ImGui::Spacing();

		textureID = ModuleFilesManager::loaded_textures[current_item];

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
