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

C_Camera::C_Camera() : Component(nullptr, C_TYPE::CAMERA), frameBuff(0), textColorBuff(0), rbo(0)
{
	name = "Camera";

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 600.f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f); //16/9 => 1.7
	FOV = 60.0f;

	frustum.pos = float3(0, 0, -10);
}

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, C_TYPE::CAMERA), frameBuff(0), textColorBuff(0), rbo(0)
{
	name = "Camera";

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 600.f;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.verticalFov = 60.0f * DEGTORAD;
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov / 2.0f) * 1.7f); //16/9 => 1.7
	FOV = 60.0f;

	frustum.pos = go->transform->transform.position;

}

C_Camera::~C_Camera()
{
	AppExtern->renderer3D->SetAsGameRender(nullptr);
	
	ClearBuffer();

}

void C_Camera::Update()
{
	frustum.pos = go->transform->transform.position;
	float4x4 matrix = go->transform->transform.globalPos;
	frustum.up = matrix.RotatePart().Col(1).Normalized();
	frustum.front = matrix.RotatePart().Col(2).Normalized();
}


void C_Camera::PrintGui()
{

	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Enabled: "); ImGui::SameLine(); ImGui::Checkbox("##Enabled", &enabled);

		ImGui::Spacing();
		ImGui::Spacing();

		if (AppExtern->menus->ButtonCenteredOnLine("Set as Game Camera"))
		{
			AppExtern->renderer3D->SetAsGameRender(this);
		}

		if(isActiveGameCam)
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "This Game Camera is ACTIVE");
		else
			ImGui::TextColored(ImVec4(255, 0, 0, 255), "This Game Camera is INACTIVE");

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Near Plane: ");
		ImGui::SameLine();
		ImGui::DragFloat("##NearPlane", &frustum.nearPlaneDistance, 0.1f, 0.1f, frustum.farPlaneDistance);

		ImGui::Spacing();

		ImGui::Text("Far Plane: ");
		ImGui::SameLine();
		ImGui::DragFloat("##FarPlane", &frustum.farPlaneDistance, 0.1f, frustum.nearPlaneDistance, 10000.f);

		ImGui::Spacing();
		
		ImGui::Text("Field Of View (FOV): ");
		ImGui::SameLine();
		if (ImGui::DragFloat("##FOV: ", &FOV, 0.1f, 1.0f, 160.f))
			frustum.verticalFov = FOV * DEGTORAD;

		ImGui::Separator();

		ImGui::TextColored(ImVec4(255,255,0,255), "Vertical FOV: ");
		ImGui::SameLine();
		ImGui::Text("%f", frustum.verticalFov);

		ImGui::TextColored(ImVec4(255, 255, 0, 255), "Horizontal FOV: ");
		ImGui::SameLine();
		ImGui::Text("%f", frustum.horizontalFov);

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

void C_Camera::SetAspectRatio(float aspectRatio)
{
	if(this != nullptr)
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspectRatio);
}

void C_Camera::SetGameCamera()
{
	AppExtern->renderer3D->SetAsGameRender(this);
}

void C_Camera::InitFrameBuffer()
{
	ClearBuffer();

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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, AppExtern->renderer3D->screenWidth, AppExtern->renderer3D->screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		AppExtern->menus->info.AddConsoleLog("ERROR! Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void C_Camera::ClearBuffer()
{
	if (frameBuff != 0)
		glDeleteFramebuffers(1, (GLuint*)&frameBuff);

	if (textColorBuff != 0)
		glDeleteTextures(1, (GLuint*)&textColorBuff);

	if (rbo != 0)
		glDeleteRenderbuffers(1, (GLuint*)&rbo);
}

void C_Camera::DrawCameraView()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)frustum.ProjectionMatrix().Transposed().v);

	glMatrixMode(GL_MODELVIEW);
	float4x4 mat = frustum.ViewMatrix();
	glLoadMatrixf((GLfloat*)mat.Transposed().v);

	//FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);

	glClearColor(0.08f, 0.08f, 0.08f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void C_Camera::EndDraw()
{
	//Is this important?

	glBindTexture(GL_TEXTURE_2D, textColorBuff);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuff);
	//glBlitFramebuffer(0, 0, msaaFBO.texBufferSize.x, msaaFBO.texBufferSize.y, 0, 0, msaaFBO.texBufferSize.x, msaaFBO.texBufferSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glDisable(GL_DEPTH_TEST);
}