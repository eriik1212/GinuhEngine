#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

#include "ExternalLibraries/MathGeoLib/include/MathGeoLib.h"

class GameObject;

struct Transform
{
	float3 position, scale, eulerRot;

	Quat quatRot;

	float4x4 globalPos;
	float4x4 localPos;

	float4x4 globalPosTransposed;
};

class C_Transform : public Component
{
public:

	C_Transform(GameObject* gameObject);
	~C_Transform();

	void Update() override;

	void SetTransform(float3 pos, Quat quatRot, float3 scale);
	float* GetGlobalTransposed();

	void PrintGui();

	Transform transform;

};

