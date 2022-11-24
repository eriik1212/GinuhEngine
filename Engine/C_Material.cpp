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
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();

		ImGui::Text("Texture Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());
		
		ImGui::Text("Texture ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", textureID);

		ImGui::Spacing();
		// Show Texture Image
		ImGui::ImageButton((ImTextureID)textureID, ImVec2(50.0f, 50.0f));


		static const char* current_item = name.c_str();

		ImGui::Text("Choose Texture: ");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < ModuleFilesManager::allText.size(); n++)
			{
				bool is_selected = (current_item == ModuleFilesManager::allText[n].c_str()); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(ModuleFilesManager::allText[n].c_str(), is_selected))
					current_item = ModuleFilesManager::allText[n].c_str();
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}

		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			mesh->texture_id = 0;
			go->RemoveComponent(this);
		}
		ImGui::Spacing();

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
