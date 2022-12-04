#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"

#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include "ModuleFilesManager.h"

map<uint, GameObject*> ModuleSceneIntro::gameObjects;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
	
}

bool ModuleSceneIntro::Init()
{

	SceneRoot = new GameObject(NULL, "Scene");
	gameObjects[0] = SceneRoot;

	//------------------------------------------------------------------------- Create & Set the Camera
	mainCam = new GameObject(gameObjects[0], "Main Camera");


	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{
	//LOG("Loading Intro assets");
	App->menus->info.AddConsoleLog( "Loading Intro assets");

	bool ret = true;

	App->camera->Move(float3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(float3(0, 0, 0));

	C_Camera* camComp = dynamic_cast<C_Camera*>(mainCam->CreateComponent(Component::C_TYPE::CAMERA));
	camComp->SetGameCamera();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	App->menus->info.AddConsoleLog( "Unloading Intro scene");


	delete mainCam;
	mainCam = nullptr;

	delete SceneRoot;
	SceneRoot = nullptr;


	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	
	UpdateGO();

	return UPDATE_CONTINUE;
}

uint ModuleSceneIntro::CreateGameObject(GameObject* parent, string name)
{
	GameObject* NewGameObject = new GameObject(parent, name);

	return NewGameObject->id;
}

float3 ModuleSceneIntro::GetGOGlobalPos(GameObject* go)
{
	float3 globalPos;

	go->transform->transform.globalPos.Decompose(globalPos, float4x4(), float3());

	return globalPos;
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
