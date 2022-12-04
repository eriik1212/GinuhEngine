#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"
#include "ModuleFilesManager.h"
#include "MeshImporter.h"

#include <iostream>
#include <string>
#include <vector>

C_Mesh::C_Mesh(GameObject* gameObject) : Component(gameObject, C_TYPE::MESH)
{

}

C_Mesh::~C_Mesh()
{


}

void C_Mesh::Update()
{
	if (mesh == nullptr || !enabled)
		return;
	else if (mesh != nullptr || enabled)
		AppExtern->renderer3D->renderList.push_back(this);


	mesh->obbBox = mesh->aabbBox;
	mesh->obbBox.Transform(go->transform->transform.globalPos);
	mesh->globalAabbBox.SetNegativeInfinity();
	mesh->globalAabbBox.Enclose(mesh->obbBox);

}

void C_Mesh::SetMesh(MeshData* mesh)
{
	this->mesh = mesh;
	this->name = mesh->meshName;
}

void C_Mesh::RenderMesh()
{
	if (mesh != nullptr)
		mesh->DrawMesh(go->transform->GetGlobalTransposed(), mesh->texture_id);
}


void C_Mesh::PrintGui()
{

	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();

		ImGui::Text("Mesh Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());

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

MeshData* C_Mesh::GetMesh()
{
	return mesh;
}
