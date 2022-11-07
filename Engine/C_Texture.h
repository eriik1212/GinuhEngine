#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

class C_Texture : public Component
{
public:
	C_Texture(GameObject* gameObject);
	~C_Texture();

	void SetTexture(const char* texture_name);

	//void PrintGui();


	const char* name;
};