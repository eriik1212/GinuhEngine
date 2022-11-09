#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Texture.h"

#include <iostream>
#include <string>
#include <vector>

C_Texture::C_Texture(GameObject* gameObject) : Component(gameObject, TYPE::TEXTURE)
{

}

C_Texture::~C_Texture()
{

}

void C_Texture::SetTexture(const char* texture_name)
{
	name = texture_name;
}

void C_Texture::PrintGui()
{

	if (ImGui::CollapsingHeader("Texture"))
	{
		ImGui::Text("Texture path:");
		ImGui::SameLine();
		ImGui::Text(name);

	}

}
