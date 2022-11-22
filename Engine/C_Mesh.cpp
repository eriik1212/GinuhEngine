#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Mesh.h"
#include "ModuleFilesManager.h"

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
	if (mesh == nullptr)
		return;
	else
		RenderMesh();
}

void C_Mesh::SetMesh(MeshData* mesh, string mesh_name)
{
	this->mesh = mesh;
	name = mesh_name;
}

void C_Mesh::RenderMesh()
{
	mesh->DrawMesh(go->transform->GetGlobalTransposed(), mesh->texture_id);
}


void C_Mesh::PrintGui()
{

	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		if (ImGui::Button("Remove"))
			go->RemoveComponent(this);

		ImGui::Text("Mesh Name:");
		ImGui::SameLine();
		ImGui::Text(name.c_str());

		ImGui::Text("Texture ID: ");
		ImGui::SameLine();
		ImGui::Text("%d", mesh->texture_id);
	}

}

MeshData* C_Mesh::GetMesh()
{
	return mesh;
}
