#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <string>
#include <vector>

#include "ExternalLibraries/MathGeoLib/include/MathGeoLib.h"


struct Transform
{
	float3 position;

	float3 rotation;

	float3 scale;
};

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameObject);
	~C_Transform();

	// ---------------------------------------------------------- IN PROGRESS
	/*static C_Transform origin() {};

	C_Transform combine(C_Transform& other) {};*/

	void SetTransform(float3 position, float3 rotation, float3 scale);

	void PrintGui();

	Transform transform;
};

