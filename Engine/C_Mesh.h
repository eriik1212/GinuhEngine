#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

struct MeshData;

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* gameObject);
	~C_Mesh();

	void SetMesh(MeshData* mesh, std::string mesh_name);

	void PrintGui();

	MeshData* GetMesh()
	{
		return mesh;
	}

	std::string name;
	MeshData* mesh;
};