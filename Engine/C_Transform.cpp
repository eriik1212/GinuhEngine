#include "C_Transform.h"
#include "GameObject.h"

C_Transform::C_Transform(GameObject* gameObject) : Component(gameObject, TYPE::TRANSFORM)
{
	// If this is not the root node, we need the parents global transform.
	if (this->GetGameObject()->GetParent() != NULL)
	{
		C_Transform* parentTransform = dynamic_cast<C_Transform*>(gameObject->GetParent()->GetComponent(Component::TYPE::TRANSFORM));
		parentGlobalTransform = parentTransform->GetGlobalTransform();
	}
}

C_Transform::~C_Transform()
{
}

void C_Transform::OnEditor()
{
	float3 temp_pos = localTransform.position;
	float3 temp_rot = localTransform.rotation;
	float3 temp_scl = localTransform.scale;

	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button("##Transform", ImVec2(20.0f, 0)))
	{
		popUpOpen = true;
		ImGui::OpenPopup("Component Options");
		ori = ImGui::GetMousePosOnOpeningCurrentPopup();
	}
	else
	{
		ImGui::CloseCurrentPopup();
	}
	if (no_collapsed)
	{
		float offset = ImGui::CalcTextSize("Position: ").x + 16;
		ImGui::Text("Position: ");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##lPos", &temp_pos[0], 0.1f)) SetPosition(temp_pos);


		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::DragFloat3("##lRot", &temp_rot[0], 0.1f)) SetRotation(temp_rot);


		ImGui::Text("Scale: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::DragFloat3("##lScale", &temp_scl[0], 0.1f)) SetScale(temp_scl);
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
}

void C_Transform::SetPosition(float3 pos)
{
	this->localTransform.position = pos;
	UpdatePosition();
}

void C_Transform::SetScale(float3 scl)
{
	this->localTransform.scale = scl;
	UpdateScale();
}

void C_Transform::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
	UpdateRotation();
}

void C_Transform::SetTransform(float3 pos, float3 scl, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scl;
	this->localTransform.rotation = rot;
	UpdateTransform();
}

void C_Transform::Translate(float3 translation)
{
	this->localTransform.position += translation;
	UpdatePosition();
}

void C_Transform::Scale(float3 scale)
{
	this->localTransform.scale += scale;
	UpdateScale();
}

void C_Transform::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
	UpdateRotation();
}

void C_Transform::OnPositionUpdate(float3 pos)
{
	parentGlobalTransform.position = pos;
	UpdatePosition();
}

void C_Transform::OnRotationUpdate(float3 rot)
{
	parentGlobalTransform.rotation = rot;
	UpdateRotation();
}

void C_Transform::OnScaleUpdate(float3 scale)
{
	parentGlobalTransform.scale = scale;
	UpdateScale();
}

void C_Transform::OnTransformUpdate(float3 pos, float3 scale, float3 rot)
{
	parentGlobalTransform.position = pos;
	parentGlobalTransform.rotation = rot;
	parentGlobalTransform.scale = scale;
	UpdateTransform();
}

Transform C_Transform::GetGlobalTransform()
{
	// If we have no parent, we are the root gameObject. which means our local transform is also our global.
	if (this->GetGameObject()->GetParent() == NULL) return localTransform;

	Transform global_transform;
	global_transform.position = parentGlobalTransform.position + localTransform.position;
	global_transform.rotation = parentGlobalTransform.rotation + localTransform.rotation;
	global_transform.scale = parentGlobalTransform.scale + localTransform.scale;

	return global_transform;
}

float3 C_Transform::GetForward()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(2).Normalized();
}

float3 C_Transform::GetRight()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(0).Normalized();
}

float3 C_Transform::GetUp()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(1).Normalized();
}

void C_Transform::UpdatePosition()
{
	float3 globalPosition = parentGlobalTransform.position + localTransform.position;
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (size_t i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnPositionUpdate(globalPosition);
	}

	// Give current position change to every component linked to this transform's gameobject.
	for (size_t i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnPositionUpdate(globalPosition);
	}
}

void C_Transform::UpdateRotation()
{
	float3 totalRotation = parentGlobalTransform.rotation + localTransform.rotation;
	// Give current rotation change to the transform component of every child of this transform's gameobject.
	for (size_t i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnRotationUpdate(totalRotation);
	}

	// Give current rotation change to every component linked to this transform's gameobject.
	for (size_t i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnRotationUpdate(totalRotation);
	}
}

void C_Transform::UpdateScale()
{
	float3 totalScale = parentGlobalTransform.scale + localTransform.scale;
	// Give current scale change to the transform component of every child of this transform's gameobject.
	for (size_t i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnScaleUpdate(totalScale);
	}

	// Give current scale change to every component linked to this transform's gameobject.
	for (size_t i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnScaleUpdate(totalScale);
	}
}

void C_Transform::UpdateTransform()
{
	Transform globalTransform = GetGlobalTransform();
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (size_t i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}

	// Give current position change to every component linked to this transform's gameobject. i = 1 so we skip this transform instance
	for (size_t i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}
}

void C_Transform::CalculateGlobalMatrix()
{
	Transform globalTransform = GetGlobalTransform();

	math::Quat rotation = Quat::FromEulerXYZ(math::DegToRad(globalTransform.rotation.x), math::DegToRad(globalTransform.rotation.y), math::DegToRad(globalTransform.rotation.z));

	globalMatrix = float4x4::FromTRS(globalTransform.position, rotation, float3(1.0f, 1.0f, 1.0f));
}