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
{
	
}

bool ModuleSceneIntro::Init()
{
	SceneRoot = new GameObject(NULL, "Scene");

	App->scene_intro->gameObjects[0] = SceneRoot;

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

	delete SceneRoot;
	SceneRoot = nullptr;

	return true;
}



// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{

	P_Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	App->files_manager->Render();

	UpdateGO();

	//FrameBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}

uint ModuleSceneIntro::CreateGameObject(GameObject* parent, std::string name)
{
	NewGameObject = new GameObject(parent, name);

	return NewGameObject->id;
}

void ModuleSceneIntro::UpdateGO()
{
	RecursiveUpdate(SceneRoot);

}

void ModuleSceneIntro::RecursiveUpdate(GameObject* goToUpdate)
{
	if (goToUpdate->isActive())
	{
		goToUpdate->Update();

		for (uint i = 0; i < goToUpdate->children.size(); i++)
		{
			RecursiveUpdate(goToUpdate->children[i]);
		}
	}
}
