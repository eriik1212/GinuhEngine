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

void C_Mesh::SetMesh(MeshData* mesh, std::string mesh_name)
{
	this->mesh = mesh;
	name = mesh_name;
}

void C_Mesh::RenderMesh()
{
	C_Transform* transform = go->transform;

	mesh->DrawMesh(transform->GetGlobalTransposed());
}


void C_Mesh::PrintGui()
{

	if (ImGui::CollapsingHeader("Mesh"))
	{
		ImGui::Text("Mesh Name:");
		ImGui::SameLine();

		ImGui::Text(name.c_str());

	}

}
