#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

struct TextData;

class C_Material : public Component
{
public:
	C_Material(GameObject* gameObject);
	~C_Material();

	void SetTexture(const char* texture_name, TextData* textureNode);

	void Update() override;

	void PrintGui();

	void UpdateStatus();

	string name;

	//MeshData* mesh = nullptr;
	TextData* text = nullptr;

	uint textureID = 0;

private:

	uint auxID = 0;
};

