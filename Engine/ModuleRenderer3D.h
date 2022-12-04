#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Primitive.h"
#include "Light.h"
#include "C_Camera.h"
#include "ModuleFilesManager.h"
#include "C_Mesh.h"

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	//---------------------------------------------------------- CAMERA
	void SetAsGameRender(C_Camera* cam);
	void RenderScene();
	void RenderGame();
	void DrawBox(float3* corners, float3 color);

	

	C_Camera* gameCamera;

	P_Plane p;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	int screenWidth = SCREEN_WIDTH,
		screenHeight = SCREEN_HEIGHT;

	vector<C_Mesh*> renderList;
};