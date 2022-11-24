#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"

#include <iostream>
#include <string>
#include <vector>

C_Transform::C_Transform(GameObject* gameObject) : Component(gameObject, C_TYPE::TRANSFORM)
{
	transform.globalPos.SetIdentity();
	transform.localPos.SetIdentity();

	transform.localPos.Decompose(transform.position, transform.quatRot, transform.scale);
	transform.quatRot.Normalize();

	transform.eulerRot = transform.quatRot.ToEulerXYZ();

	transform.globalPosTransposed = transform.globalPos.Transposed();
}

C_Transform::~C_Transform()
{

}

void C_Transform::Update()
{
	// ---------------------------------------------------------------------------------------------------------------------------- Define rotation (QUAT from EULER)
	transform.quatRot = Quat::FromEulerXYZ(transform.eulerRot.x * DEGTORAD, transform.eulerRot.y * DEGTORAD, transform.eulerRot.z * DEGTORAD);
	transform.quatRot.Normalize();

	// -------------------------------------------------------------------------------------------------------------- Define the Local Position
	transform.localPos = float4x4::FromTRS(transform.position, transform.quatRot, transform.scale);

	if (this->go->parent != nullptr)
	{
		if (this->go->parent->transform != nullptr)
		{
			// We apply the posicion formula
			this->transform.globalPos = this->go->parent->transform->transform.globalPos * this->transform.localPos;
			this->transform.globalPosTransposed = this->transform.globalPos.Transposed();
		}
	}
}

void C_Transform::SetTransform(float3 pos, Quat quatRot, float3 scale)
{
	transform.position = pos;
	transform.quatRot = quatRot.Normalized();
	transform.scale = scale;

	transform.eulerRot = transform.quatRot.ToEulerXYZ() * RADTODEG;

	transform.localPos = float4x4::FromTRS(transform.position, transform.quatRot, transform.scale);

	if (go->parent->transform != nullptr) {
		transform.globalPos = go->parent->transform->transform.globalPos * transform.localPos;
	}
	transform.globalPosTransposed = transform.globalPos.Transposed();
}

float* C_Transform::GetGlobalTransposed()
{
	return transform.globalPosTransposed.ptr();
}

void C_Transform::PrintGui()
{

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Pos", &transform.position[0], 0.1f);

		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rot", &transform.eulerRot[0], 0.1f);

		ImGui::Text("Scale:");
		ImGui::SameLine();
		ImGui::DragFloat3("##Sca", &transform.scale[0], 0.1f);

		
		if (ImGui::Button("Reset Transform", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			ResetTransform();

		ImGui::Spacing();
	}
}

void C_Transform::ResetTransform()
{
	transform.globalPos.SetIdentity();
	transform.localPos.SetIdentity();

	transform.localPos.Decompose(transform.position, transform.quatRot, transform.scale);
	transform.quatRot.Normalize();

	transform.eulerRot = transform.quatRot.ToEulerXYZ();

	transform.globalPosTransposed = transform.globalPos.Transposed();
}