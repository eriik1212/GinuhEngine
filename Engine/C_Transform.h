#pragma once

#include "Component.h"

struct Transform
{
	float3 position = float3(0.0f, 0.0f, 0.0f);
	float3 scale = float3(0.0f, 0.0f, 0.0f);
	float3 rotation = float3(0.0f, 0.0f, 0.0f);
};

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameObject);
	~C_Transform();

	void OnEditor();

	void SetPosition(float3 pos);
	void SetScale(float3 scl);
	void SetRotation(float3 rot);
	void SetTransform(float3 pos, float3 scl, float3 rot);

	void Translate(float3 translation);
	void Scale(float3 scale);
	void Rotate(float3 rotate);

	void OnPositionUpdate(float3 position) override;
	void OnScaleUpdate(float3 scale) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 position, float3 scale, float3 rotation) override;

	Transform GetGlobalTransform();

	float3 GetForward();
	float3 GetRight();
	float3 GetUp();

private:
	Transform localTransform;

	void UpdatePosition();
	void UpdateRotation();
	void UpdateScale();
	void UpdateTransform();

	void CalculateGlobalMatrix();

private:
	float4x4 globalMatrix;

	Transform parentGlobalTransform;
	friend class GameObject;
};