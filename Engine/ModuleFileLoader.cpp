#include "Globals.h"
#include "Application.h"
#include "ModuleFileLoader.h"

#include <Windows.h>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleFileLoader::ModuleFileLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleFileLoader::~ModuleFileLoader()
{}

// Called before render is available
bool ModuleFileLoader::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

update_status ModuleFileLoader::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleFileLoader::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)	LoadFile();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFileLoader::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFileLoader::CleanUp()
{
	//LOG("Destroying Module");
	App->menus->my_log.AddLog("Destroying Module");

	aiDetachAllLogStreams();

	return true;
}

void ModuleFileLoader::LoadFile()
{
	// ************************************ STAND BY ************************************

	/*const char* file_path = "C:/Users/david/OneDrive/Escritorio/baker_house/BakerHouse.fbx";

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);

		aiMesh* houseMesh = new aiMesh;

		// copy vertices
		houseMesh->mNumVertices = scene->mMeshes[0]->mNumVertices;
		//houseMesh->mVertices = new float[houseMesh->mNumVertices * 3];
		memcpy(houseMesh->mVertices, scene->mMeshes[0]->mVertices, sizeof(float) * houseMesh->mNumVertices * 3);
		LOG("New mesh with %d vertices", houseMesh->mNumVertices);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}
	*/
	

}
