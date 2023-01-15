#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "GameObject.h"
#include "C_AudioListener.h"
#include "C_AudioSource.h"

#include "MeshImporter.h"

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
	mainCam->transform->transform.position = float3(0, 0, -7.0f);

	//------------------------------------------------------------------------- Create Empty GO with BG Music
	emptyGO = new GameObject(gameObjects[0], "Background Music");
	emptyGO->transform->transform.position = float3(0, 0, 0);


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

	//--------------------------------------------------------------------------------------------------------------- Camera Components

	C_Camera* camComp = dynamic_cast<C_Camera*>(mainCam->CreateComponent(Component::C_TYPE::CAMERA));
	camComp->SetGameCamera();

	C_AudioListener* listComp = dynamic_cast<C_AudioListener*>(mainCam->CreateComponent(Component::C_TYPE::AUDIO_LISTENER));

	//---------------------------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------------------------- EmptyGO Components

	C_AudioSource* sourceComp = dynamic_cast<C_AudioSource*>(emptyGO->CreateComponent(Component::C_TYPE::AUDIO_SOURCE));
	sourceComp->audio = "BACKGROUNDMUSIC";
	sourceComp->PlayEvent();

	//---------------------------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------------------------- Cube GO

	MeshImporter::ImportMesh("Assets/Primitives/cube.fbx");
	gameObjects[3]->transform->transform.position = float3(4, 0, -6);

	//---------------------------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------------------------- Sphere GO

	MeshImporter::ImportMesh("Assets/Primitives/sphere.fbx");
	gameObjects[5]->transform->transform.position = float3(-4, 0, -10);

	timer = 0;
	changeDir = false;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
	App->menus->info.AddConsoleLog( "Unloading Intro scene");
	
	for (int g = gameObjects.size() - 1; g >= 0; --g)
	{
		delete gameObjects[g];
		gameObjects[g] = nullptr;
	}
	gameObjects.clear();

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	UpdateGO();

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	timer++;

	if (timer >= 100.0f)
	{
		changeDir = !changeDir;
		timer = 0;
	}

	if (changeDir)
	{
		gameObjects[5]->transform->transform.position += float3(0, 0, -0.1f);
	}
	else if (!changeDir)
	{
		gameObjects[5]->transform->transform.position += float3(0, 0, 0.1f);
	}

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
