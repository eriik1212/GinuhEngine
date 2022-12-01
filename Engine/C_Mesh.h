#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct MeshData;
struct TextData;

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* gameObject);
	~C_Mesh();

	void Update() override;

	void SetMesh(MeshData* mesh);

	void RenderMesh();

	void PrintGui();

	MeshData* GetMesh();

	string name;

	MeshData* mesh = nullptr;
	TextData* text = nullptr;
};