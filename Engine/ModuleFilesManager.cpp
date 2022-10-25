#include "Application.h"
#include "ModuleFilesManager.h"

namespace fs = std::filesystem;

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

		strcpy_s(existent_filedir, assets_dir);
		strcat_s(existent_filedir, fileName_char);

		// We get the Extension
		string extension = fs::path(fileName_char).extension().string();
		const char* extension_char = extension.c_str();
		
		if (existent_filedir != nullptr && extension == ".fbx")
		{
			LoadFile(existent_filedir);

			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s' Loaded Succesfully", fileName_char);

		}
		else if (existent_filedir != nullptr && extension == ".png")
		{

			LoadTexture(existent_filedir);

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

			strcpy_s(new_filedir, assets_dir);
			strcat_s(new_filedir, fileName_char);

			// We copy the dropped file to "Assets/" dir, with its name
			CopyFile(dropped_filedir, new_filedir, FALSE);

			// We get the Extension
			string extension = fs::path(dropped_filedir).extension().string();
			const char* extension_char = extension.c_str();

			if (extension == ".fbx" && new_filedir != nullptr)
			{
				LoadFile(new_filedir);
			}
			else if (extension == ".png" && new_filedir != nullptr)	LoadTexture(new_filedir);

			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "File '%s', with Extension '%s' Dropped Succesfully", fileName_char, extension_char);

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

	aiDetachAllLogStreams();

	meshList.clear();

	return true;
}

void ModuleFilesManager::LoadFile(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			MeshData* newMesh = new MeshData();
			// copy vertices
			newMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			newMesh->vertex = new float[newMesh->num_vertex * 5];
			memcpy(newMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->num_vertex * 3);
			App->menus->info.AddConsoleLog(__FILE__, __LINE__, "New mesh with %d vertices", newMesh->num_vertex);

			for (int v = 0; v < newMesh->num_vertex; v++) {
				// Vertex
				newMesh->vertex[v * 5] = scene->mMeshes[i]->mVertices[v].x;
				newMesh->vertex[v * 5 + 1] = scene->mMeshes[i]->mVertices[v].y;
				newMesh->vertex[v * 5 + 2] = scene->mMeshes[i]->mVertices[v].z;

				// UVs
				newMesh->vertex[v * 5 + 3] = scene->mMeshes[i]->mTextureCoords[0][v].x;
				newMesh->vertex[v * 5 + 4] = scene->mMeshes[i]->mTextureCoords[0][v].y;
			}

			// copy faces
			if (scene->mMeshes[i]->HasFaces())
			{
				newMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				newMesh->index = new uint[newMesh->num_index]; // assume each face is a triangle
				
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						App->menus->info.AddConsoleLog(__FILE__, __LINE__, "WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&newMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
				LoadMeshData(newMesh);
			}
			else {
				//if no faces, just delete mesh
				App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Scene %s, has no faces.", file_path);
				delete newMesh;
			}

		}
		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "% s Pushed In List Successfully", file_path);

		aiReleaseImport(scene);

	}
	else
	{
		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "Error loading scene % s. ERROR: %s", file_path, aiGetErrorString());
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
	//Bind Texture
	glEnable(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, ImgId);

	// Bind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	// Vertex Array [ x, y, z, u, v ]
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 5, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	glPushMatrix();

	// Draw
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	glPopMatrix(); // Unbind transform matrix

	// Unbind buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	
}

uint ModuleFilesManager::LoadTexture(const char* filePath)
{
	ilInit();
	iluInit();
	ilutInit();
	
	// -------------------------------------- Loading Image
	if (ilLoadImage(filePath))
	{
		ilEnable(IL_FILE_OVERWRITE);
		ilSaveImage(filePath);

		ilGenImages(1, &ImgId);
		ilBindImage(ImgId);

		ilLoadImage(filePath);

		//ilBindImage(ImgId);
		BYTE* data = ilGetData();

		ILuint imgWidth, imgHeight;
		imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
		imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		int const type = ilGetInteger(IL_IMAGE_TYPE); // matches OpenGL
		int const format = ilGetInteger(IL_IMAGE_FORMAT); // matches OpenGL
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format,
			type, data);  // Create texture from image data
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		ImgId = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, ImgId);
		ilDeleteImages(1, &ImgId);

		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "TEX ID: %d", ImgId);

		return ImgId;
	}
	else
	{
		App->menus->info.AddConsoleLog(__FILE__, __LINE__, "DevIL ERROR: Could not Load Image. Error: %s", ilGetError());

		return 0;
	}

}

void ModuleFilesManager::LoadMeshData(MeshData* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	//Create vertices and indices buffers
	glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
	glGenBuffers(1, (GLuint*)&(mesh->id_index));

	//Bind and fill buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 5, mesh->vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

	//Unbind buffers
	glDisableClientState(GL_VERTEX_ARRAY);

	//Add mesh to meshes vector
	meshList.push_back(mesh);
}