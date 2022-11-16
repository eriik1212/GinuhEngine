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

	void SetTexture(const char* texture_name);

	void PrintGui();


	const char* name;
};