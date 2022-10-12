#include "Globals.h"
#include "Application.h"
#include "ModuleFilesManager.h"

#include <Windows.h>
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

namespace fs = std::filesystem;

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

	// ------------------------------------- Load All Existing .fbx files -------------------------------------

	std::string path = "Assets/";
	for (const auto& entry : fs::directory_iterator(path))
	{

		// We get the FileName and Extension
		string fileName = entry.path().filename().string();
		const char* fileName_char = fileName.c_str();

		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s'", fileName_char);

		char existent_filedir[100];

		strcpy(existent_filedir, assets_dir);
		strcat(existent_filedir, fileName_char);
		
		if (existent_filedir != nullptr)
		{
			newMesh = new MeshData;

			LoadFile(existent_filedir, newMesh);

			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s' Loaded Succesfully", fileName_char);

		}
		else
		{
			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s' cannot be loaded", fileName_char);

		}
	}

	return ret;
}

update_status ModuleFilesManager::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleFilesManager::Update(float dt)
{	
	// ------------------------------------------------------------------------ Drag & Drop LOGIC
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case (SDL_DROPFILE): {      // In case if dropped file
			dropped_filedir = event.drop.file;

			// We get the FileName and Extension
			string fileName = fs::path(dropped_filedir).filename().string();
			const char* fileName_char = fileName.c_str();

			char new_filedir[100];

			strcpy(new_filedir, assets_dir);
			strcat(new_filedir, fileName_char);

			// We copy the dropped file to "Assets/" dir, with its name
			CopyFile(dropped_filedir, new_filedir, FALSE);

			newMesh = new MeshData;

			if(new_filedir != nullptr)	LoadFile(new_filedir, newMesh);

			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s' Dropped Succesfully", fileName_char);

		}
			SDL_free(dropped_filedir);    // Free dropped_filedir memory
			
			break;
		}

	}

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

	delete newMesh;

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
