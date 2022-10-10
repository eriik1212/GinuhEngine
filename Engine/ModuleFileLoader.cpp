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

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)	LoadFile("C:/Users/david/OneDrive/Escritorio/baker_house/BakerHouse.fbx", &newMesh);

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
	App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Destroying Module File Loader");

	aiDetachAllLogStreams();

	return true;
}

void ModuleFileLoader::LoadFile(const char* file_path, MeshData* ourMesh)
{
	// ************************************ STAND BY ************************************

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			// copy vertices
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * 3];
			memcpy(ourMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh->num_vertex * 3);
			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "New mesh with %d vertices", ourMesh->num_vertex);

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh->index = new uint[ourMesh->num_index]; // assume each face is a triangle
				for (uint i = 0; i < scene->mMeshes[i]->mNumFaces; ++i)
				{
					if (scene->mMeshes[i]->mFaces[i].mNumIndices != 3)
					{
						App->menus->info.AddConsoleLog(__FILE__, __LINE__, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[i * 3], scene->mMeshes[i]->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}

		}
		
	}
	else
	{
		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Error loading scene % s", file_path);
	}
	
	

}
