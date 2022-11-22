#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

class C_Material : public Component
{
public:
	C_Material(GameObject* gameObject);
	~C_Material();

	void SetTexture(const char* texture_name, uint textID);

	void PrintGui();

	const char* name;

	C_Mesh* meshComp = nullptr;

private:
	uint textureID = 0;
};