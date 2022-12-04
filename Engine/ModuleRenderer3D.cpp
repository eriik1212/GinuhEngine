#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "MeshImporter.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	gameCamera = nullptr;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	//LOG("Creating 3D Renderer context");
	App->menus->info.AddConsoleLog("Creating 3D Renderer context");

	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		//LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		App->menus->info.AddConsoleLog("OpenGL context could not be created! SDL_Error: %s", SDL_GetError());

		ret = false;
	}

	//GLEW
	GLenum err = glewInit();
	//LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		App->menus->info.AddConsoleLog("Error: %a\n", glewGetErrorString(err));
	}
	else {
		App->menus->info.AddConsoleLog("Using Glew %s", glewGetString(GLEW_VERSION));

	}
	//Should be 2.0

	App->menus->info.AddConsoleLog("Vendor: %s", glGetString(GL_VENDOR));
	App->menus->info.AddConsoleLog("Renderer: %s", glGetString(GL_RENDERER));
	App->menus->info.AddConsoleLog("OpenGL version supported %s", glGetString(GL_VERSION));
	App->menus->info.AddConsoleLog("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	SDL_GL_MakeCurrent(App->window->window, context);

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			//LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			App->menus->info.AddConsoleLog("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->menus->info.AddConsoleLog("Error initializing OpenGL! %s\n", gluErrorString(error));

			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->menus->info.AddConsoleLog("Error initializing OpenGL! %s\n", gluErrorString(error));

			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			App->menus->info.AddConsoleLog("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}


		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

	}

	// Projection matrix for
	OnResize(screenWidth, screenHeight);

	// Init ImGui (SDL&OpenGL)
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
	ImGui_ImplOpenGL3_Init("#version 120");

	/*App->camera->sceneCam.InitFrameBuffer();

	if(gameCamera != nullptr)
		gameCamera->InitFrameBuffer();*/

		/*GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][3];
		for (int i = 0; i < CHECKERS_HEIGHT; i++) {
			for (int j = 0; j < CHECKERS_WIDTH; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkerImage[i][j][0] = (GLubyte)c;
				checkerImage[i][j][1] = (GLubyte)c;
				checkerImage[i][j][2] = (GLubyte)c;
				//checkerImage[i][j][3] = (GLubyte)255;
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, 3, CHECKERS_HEIGHT, CHECKERS_HEIGHT, 0, GL_RGB,
			GL_UNSIGNED_BYTE, checkerImage);  // Create texture from image data
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		*/

	glEnable(GL_TEXTURE_2D);

	p = P_Plane(0, 1, 0, 0);
	p.axis = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// ------------------------------------------------------------------------ Scene Render ------------------------------------------------------------------------ //
	App->camera->sceneCam.SetAspectRatio((float)App->renderer3D->screenWidth / (float)App->renderer3D->screenHeight);
	App->camera->sceneCam.DrawCameraView();

	p.Render();

	//In scene view [RenderScene()] user can render bounding boxes.
	RenderScene();

	// Wireframe View
	if (App->scene_intro->wireframe)
	{
		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	App->camera->sceneCam.EndDraw();

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------- //
	//
	// ------------------------------------------------------------------------ Game Render ------------------------------------------------------------------------ //

	if (App->renderer3D->gameCamera != nullptr)
	{
		App->renderer3D->gameCamera->SetAspectRatio((float)App->renderer3D->screenWidth / (float)App->renderer3D->screenHeight);
		App->renderer3D->gameCamera->DrawCameraView();

		//In game view [RenderGame()] we avoid showing bounding boxes.
		RenderGame();

		// light 0 on cam pos
		App->renderer3D->lights[0].SetPos(App->renderer3D->gameCamera->frustum.pos.x, App->renderer3D->gameCamera->frustum.pos.y, App->renderer3D->gameCamera->frustum.pos.z);

		for (uint i = 0; i < MAX_LIGHTS; ++i)
			App->renderer3D->lights[i].Render();

		App->renderer3D->gameCamera->EndDraw();

	}
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------- //

	renderList.clear();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::RenderScene()
{
	for (int i = 0; i < renderList.size(); i++)
	{
		renderList[i]->RenderMesh();

		if (App->menus->showingBBoxes)
			renderList[i]->GetMesh()->RenderAABB();
	}
}


void ModuleRenderer3D::RenderGame()
{
	for (int i = 0; i < renderList.size(); i++)
	{
		renderList[i]->RenderMesh();
	}
}

void ModuleRenderer3D::DrawBox(float3* corners, float3 color)
{

	//Fill points
	int indices[24] = { 0,2,2,6,6,4,4,0,0,1,1,3,3,2,4,5,6,7,5,7,3,7,1,5 };

	glBegin(GL_LINES);
	glColor3fv(color.ptr());

	for (size_t i = 0; i < 24; i++)
	{
		glVertex3fv(corners[indices[i]].ptr());
	}

	glColor3f(255.f, 255.f, 255.f);
	glEnd();

}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	//LOG("Destroying 3D Renderer");
	App->menus->info.AddConsoleLog("Destroying 3D Renderer");

	if (context != NULL)
	{
		SDL_GL_DeleteContext(context);
	}

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	//glLoadMatrixf(&ProjectionMatrix);
	//
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	screenHeight = height;
	screenWidth = width;

	App->camera->sceneCam.InitFrameBuffer();

	if (gameCamera != nullptr)
		gameCamera->InitFrameBuffer();
}

void ModuleRenderer3D::SetAsGameRender(C_Camera* cam)
{
	if (cam == nullptr) {
		gameCamera = nullptr;
		AppExtern->menus->info.AddConsoleLog("GAME CAMERA does not exist");
		return;
	}

	if (gameCamera != nullptr)
		gameCamera->isActiveGameCam = false;

	cam->isActiveGameCam = true;

	gameCamera = cam;

	if (gameCamera != nullptr)
		gameCamera->InitFrameBuffer();
}