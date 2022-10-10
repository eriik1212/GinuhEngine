#include "Globals.h"
#include "Application.h"
#include "ModuleFilesManager.h"

#include <Windows.h>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

vector<MeshData*> ModuleFilesManager::meshList;

ModuleFilesManager::ModuleFilesManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleFilesManager::~ModuleFilesManager()
{}

// Called before render is available
bool ModuleFilesManager::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LoadFile("Assets/BakerHouse.fbx", &houseMesh);

	return ret;
}

update_status ModuleFilesManager::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleFilesManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleFilesManager::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleFilesManager::CleanUp()
{
	//LOG("Destroying Module");
	App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Destroying Module File Loader");

	meshList.clear();

	aiDetachAllLogStreams();

	return true;
}

void ModuleFilesManager::LoadFile(const char* file_path, MeshData* ourMesh)
{
	// ************************************ CONSTRUCTION SITE ************************************

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
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
				
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						App->menus->info.AddConsoleLog(__FILE__, __LINE__, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&ourMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
				meshList.push_back(ourMesh);
			}

		}

		aiReleaseImport(scene);

	}
	else
	{
		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Error loading scene % s", file_path);
	}
	
	

}

void ModuleFilesManager::Render()
{
	for (int i = 0; i < meshList.size(); i++) {
		meshList[i]->DrawMesh();
	}
}

void MeshData::DrawMesh()
{
		glBegin(GL_TRIANGLES); // Drawing with triangles

		for (int i = 0; i < num_index; i++) {
			glVertex3f(vertex[index[i] * 3], vertex[index[i] * 3 + 1], vertex[index[i] * 3 + 2]);
		}

		glEnd();
	
}
