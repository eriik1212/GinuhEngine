#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "C_Camera.h"
#include "ModuleFilesManager.h"

#include <iostream>
#include <string>
#include <vector>

C_Camera::C_Camera() : Component(nullptr, C_TYPE::CAMERA)
{
	name = "Camera";

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 600.f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f); //That 1.7 comes from 16/9 aspect ratio

	frustum.pos = float3(0, 0, -10);

	AppExtern->scene_intro->SetAsGameCam(this);
}

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, C_TYPE::CAMERA)
{
	name = "Camera";

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 600.f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f); //That 1.7 comes from 16/9 aspect ratio

	frustum.pos = go->transform->transform.position;

	AppExtern->scene_intro->SetAsGameCam(this);
}

C_Camera::~C_Camera()
{
	glDeleteFramebuffers(1, &frameBuff);
	glDeleteFramebuffers(1, &textColorBuff);
}

void C_Camera::Update()
{

}


void C_Camera::PrintGui()
{

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();

		ImGui::Spacing();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Remove Component", ImVec2(ImGui::GetWindowSize().x, 20.0f)))
			go->RemoveComponent(this);
		ImGui::PopStyleColor(3);

		ImGui::Spacing();

	}

}

//void C_Camera::SetAspectRatio(float aspectRatio)
//{
//	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
//}

void C_Camera::InitFrameBuffer()
{
	glGenFramebuffers(1, &frameBuff);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

	// generate texture
	glGenTextures(1, &textColorBuff);
	glBindTexture(GL_TEXTURE_2D, textColorBuff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textColorBuff, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		AppExtern->menus->info.AddConsoleLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

