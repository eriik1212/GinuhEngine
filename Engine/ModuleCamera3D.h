#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "C_Camera.h"

#include "ExternalLibraries/MathGeoLib/include/MathGeoLib.h"

class GameObject;
class ModuleSceneIntro;
class C_Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	float* GetViewMatrix();
	Color background;

	//----------------- Scene View CAM
	C_Camera sceneCam;

private:

	void CalculateViewMatrix();

private:
	float3 reference;
	float4x4 ViewMatrix;

};