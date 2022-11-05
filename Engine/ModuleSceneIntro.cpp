#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include "ModuleFilesManager.h"

std::map<uint, GameObject*> ModuleSceneIntro::gameObjects;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	

	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	//LOG("Loading Intro assets");
	App->menus->info.AddConsoleLog( "Loading Intro assets");

	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	App->menus->info.AddConsoleLog( "Unloading Intro scene");

	return true;
}



// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	P_Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	App->files_manager->Render();

	//FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

uint ModuleSceneIntro::CreateGameObject(GameObject* parent, std::string name)
{
	GameObject* go = new GameObject(parent, name);

	return go->id;
}

