#include "Application.h"
#include "ModuleFilesManager.h"
#include "TextureImporter.h"
#include "MeshImporter.h"
#include "PhysFS/include/physfs.h"


#pragma comment (lib, "PhysFS/libx86/physfs.lib")

namespace fs = std::filesystem;

//std::map<std::string, uint> ModuleFilesManager::loaded_textures;
//vector<string> ModuleFilesManager::allText;

ModuleFilesManager::ModuleFilesManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	PHYSFS_init(0);

	if (PHYSFS_setWriteDir(".") == 0) App->menus->info.AddConsoleLog("FileSystem:: %s\n", PHYSFS_getLastError());

	if (!PHYSFS_isDirectory(LIBRARY_PATH)) PHYSFS_mkdir(LIBRARY_PATH);
	if (!PHYSFS_isDirectory(MODELS_PATH)) PHYSFS_mkdir(MODELS_PATH);
	if (!PHYSFS_isDirectory(MESHES_PATH)) PHYSFS_mkdir(MESHES_PATH);
	if (!PHYSFS_isDirectory(TEXTURES_PATH)) PHYSFS_mkdir(TEXTURES_PATH);
	if (!PHYSFS_isDirectory(MATERIALS_PATH)) PHYSFS_mkdir(MATERIALS_PATH);
	if (!PHYSFS_isDirectory(SCENES_PATH)) PHYSFS_mkdir(SCENES_PATH);
	if (!PHYSFS_isDirectory(SCRIPTS_PATH)) PHYSFS_mkdir(SCRIPTS_PATH);
}

// Destructor
ModuleFilesManager::~ModuleFilesManager()
{
	PHYSFS_deinit();

	for (int m = 0; m < meshList.size(); m++)
	{
		delete meshList[m];
		meshList[m] = nullptr;
	}
	meshList.clear();

	for (int t = 0; t < textList.size(); t++)
	{
		delete textList[t];
		textList[t] = nullptr;
	}
	textList.clear();

}

// Called before render is available
bool ModuleFilesManager::Init()
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleFilesManager::Start()
{
	bool ret = true;

	// ------------------------------------- Load All Existing .fbx files -------------------------------------
	string path = "Assets/";
	for (const auto& entry : fs::directory_iterator(path))
	{
		// We get the FileName and Extension
		string fileName = entry.path().filename().string();
		const char* fileName_char = fileName.c_str();

		App->menus->info.AddConsoleLog("File '%s'", fileName_char);

		char existent_filedir[100];

		strcpy_s(existent_filedir, assets_dir);
		strcat_s(existent_filedir, fileName_char);

		// We get the Extension
		string extension = fs::path(fileName_char).extension().string();
		const char* extension_char = extension.c_str();

		if (existent_filedir != nullptr && (extension == ".fbx" || extension == ".FBX"))
		{
			MeshImporter::ImportMesh(existent_filedir);

			App->menus->info.AddConsoleLog("File '%s' Loaded Succesfully", fileName_char);

		}
		// Load Texture Here???
		else if (existent_filedir != nullptr && (extension == ".png" || extension == ".tga" || extension == ".dds"))
		{
			TextureImporter::ImportTexture(existent_filedir);

			App->menus->info.AddConsoleLog("File '%s' Loaded Succesfully", fileName_char);

		}
		else
		{
			App->menus->info.AddConsoleLog("File '%s' cannot be loaded", fileName_char);

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

			strcpy_s(new_filedir, assets_dir);
			strcat_s(new_filedir, fileName_char);

			// We copy the dropped file to "Assets/" dir, with its name
			CopyFile(dropped_filedir, new_filedir, FALSE);

			// We get the Extension
			string extension = fs::path(dropped_filedir).extension().string();
			const char* extension_char = extension.c_str();

			if ((extension == ".fbx" || extension == ".FBX") && new_filedir != nullptr)
			{
				MeshImporter::ImportMesh(new_filedir);

			}
			else if ((extension == ".png" || extension == ".tga") && new_filedir != nullptr)
			{
				TextureImporter::ImportTexture(new_filedir);

				/*C_Mesh* mesh = dynamic_cast<C_Mesh*>(App->scene_intro->gameobject_selected->GetComponent(Component::C_TYPE::MESH));
				mesh->GetMesh()->texture_id = LoadTexture(new_filedir);
				dynamic_cast<C_Material*>(App->scene_intro->gameobject_selected->GetComponent(Component::C_TYPE::MATERIAL))->SetTexture(new_filedir, textID);*/

			}

			App->menus->info.AddConsoleLog("File '%s', with Extension '%s' Dropped Succesfully", fileName_char, extension_char);

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
	App->menus->info.AddConsoleLog("Destroying Module File Loader");

	aiDetachAllLogStreams();

	//newMesh->~MeshData();
	//for (int m = 0; m < meshList.size(); m++)
	//{
		//delete newMesh;
		//newMesh = nullptr;
	//}

	//meshList.clear();

	return true;
}

string ModuleFilesManager::GetFileName(string path, bool wExtension)
{
	// We get the FileName and Extension
	string fileName = fs::path(path).filename().string();

	if (!wExtension)
	{
		uint found = fileName.find_last_of(".");
		if (found != string::npos)
			fileName = fileName.substr(0, found);
	}

	return fileName;
}

string ModuleFilesManager::NormalizePath(const char* path)
{
	string newPath = path;

	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

string ModuleFilesManager::AdaptPath(const char* full_path)
{
	string newPath = full_path;
	newPath.erase(remove(newPath.begin(), newPath.end(), (int)"..\\"), newPath.end());

	return newPath;
}

string ModuleFilesManager::SplitPath(const char* full_path)
{
	string path = full_path;

	size_t pos = path.find_last_of("/");

	string splittedPath = path.substr(0, pos + 1);

	return splittedPath;

}

string ModuleFilesManager::EraseSubStr(const char* mainStr, const char* toErase)
{
	string path = mainStr;
	string erase = toErase;

	// Search for the substring in string
	size_t pos = path.find(erase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		path.erase(pos, erase.length());
	}

	return path;
}

void MeshData::DrawMesh(const float* globalTransform, uint imgID)
{

	glEnable(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, imgID);

	// ----------------------------------------------------------------------- Bind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	// ----------------------------------------------------------------------- DRAW TEXTURE CORRECTLY
	glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, (void*)(3 * sizeof(float)));
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);

	glPushMatrix();
	glMultMatrixf(globalTransform);

	// Draw
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	// ----------------------------------------------------------------------- UnBind Buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_COORD_ARRAY);
}

void MeshData::InitAABB()
{
	vector<float3> correctVertex;
	for (size_t i = 0; i < num_vertex * VERTEX_FEATURES; i += VERTEX_FEATURES)
	{
		correctVertex.emplace_back(vertex[i], vertex[i + 1], vertex[i + 2]);
	}
	aabbBox.SetFrom(&correctVertex[0], correctVertex.size());

}

void MeshData::RenderAABB()
{
	float3 corners1[8];
	obbBox.GetCornerPoints(corners1);

	// Draw
	AppExtern->renderer3D->DrawBox(corners1, float3(255, 0, 0));

	float3 corners2[8];
	globalAabbBox.GetCornerPoints(corners2);

	// Draw
	AppExtern->renderer3D->DrawBox(corners2, float3(0, 0, 255));
}


