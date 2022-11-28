#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

class C_Material : public Component
{
public:
	C_Material(GameObject* gameObject);
	~C_Material();

	void SetTexture(const char* texture_name, MeshData* meshNode);

	void Update() override;

	void PrintGui();

	void UpdateStatus();

	string name;

	MeshData* mesh = nullptr;


private:

	uint textureID = 0;
	uint auxID = 0;
};

