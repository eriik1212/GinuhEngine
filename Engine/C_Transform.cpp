#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"

#include <iostream>
#include <string>
#include <vector>

C_Transform::C_Transform(GameObject* gameObject) : Component(gameObject, TYPE::TRANSFORM)
{

}

C_Transform::~C_Transform()
{

}

void C_Transform::SetTransform(float3 position, float3 rotation, float3 scale)
{
	transform.position = position;

	transform.rotation = rotation;

	transform.scale = scale;
}

void C_Transform::OnGui()
{

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position:");
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.position.x).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.position.y).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.position.z).c_str());



		ImGui::Text("Rotation:");
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.rotation.x).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.rotation.y).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.rotation.z).c_str());



		ImGui::Text("Scale:");
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.scale.x).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.scale.y).c_str());
		ImGui::SameLine();

		ImGui::Text(std::to_string(transform.scale.z).c_str());

	}

}