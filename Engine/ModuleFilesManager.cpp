#include "Application.h"
#include "ModuleFilesManager.h"
#include "ConsoleInfo.h"


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

	return ret;
}

// -----------------------------------------------------------------
bool ModuleFilesManager::Start()
{
	bool ret = true;

	// ------------------------------------- Load All Existing .fbx files -------------------------------------
	std::string path = "Assets/";
	for (const auto& entry : fs::directory_iterator(path))
	{
		// We get the FileName and Extension
		string fileName = entry.path().filename().string();
		const char* fileName_char = fileName.c_str();

		LOGC("File '%s'", fileName_char);

		char existent_filedir[100];

		strcpy_s(existent_filedir, assets_dir);
		strcat_s(existent_filedir, fileName_char);

		// We get the Extension
		string extension = fs::path(fileName_char).extension().string();
		const char* extension_char = extension.c_str();

		if (existent_filedir != nullptr && extension == ".fbx")
		{
			LoadFile(existent_filedir);

			LOGC("File '%s' Loaded Succesfully", fileName_char);

		}
		else if (existent_filedir != nullptr && extension == ".png")
		{

			LoadTexture(existent_filedir);

			LOGC("File '%s' Loaded Succesfully", fileName_char);

		}
		else
		{
			LOGC("File '%s' cannot be loaded", fileName_char);

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

			LOGC("File '%s', with Extension '%s' Dropped Succesfully", fileName_char, extension_char);

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
	LOGC("Destroying Module File Loader");

	aiDetachAllLogStreams();

	//newMesh->~MeshData();
	for (int m = 0; m < meshList.size(); m++)
	{
		delete newMesh[m];
		newMesh[m] = nullptr;
	}


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
			int m = meshList.size();

				newMesh[m] = new MeshData();
				// copy vertices
				newMesh[m]->num_vertex = scene->mMeshes[i]->mNumVertices;
				newMesh[m]->vertex = new float[newMesh[m]->num_vertex * VERTEX_FEATURES];
				//memcpy(newMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->num_vertex * 3);
				LOGC("New mesh with %d vertices", newMesh[m]->num_vertex);

				for (int v = 0; v < newMesh[m]->num_vertex; v++) {
					// Vertex
					newMesh[m]->vertex[v * VERTEX_FEATURES] = scene->mMeshes[i]->mVertices[v].x;
					newMesh[m]->vertex[v * VERTEX_FEATURES + 1] = scene->mMeshes[i]->mVertices[v].y;
					newMesh[m]->vertex[v * VERTEX_FEATURES + 2] = scene->mMeshes[i]->mVertices[v].z;

					if (scene->mMeshes[i]->HasTextureCoords(0))
					{
						// UVs
						newMesh[m]->vertex[v * VERTEX_FEATURES + 3] = scene->mMeshes[i]->mTextureCoords[0][v].x;
						newMesh[m]->vertex[v * VERTEX_FEATURES + 4] = scene->mMeshes[i]->mTextureCoords[0][v].y;
					}
					// -------------------------------------------------------------------------------------- In a future
					if (scene->mMeshes[i]->HasNormals())
					{
						newMesh[m]->vertex[v * VERTEX_FEATURES + 5] = scene->mMeshes[i]->mNormals[v].x;
						newMesh[m]->vertex[v * VERTEX_FEATURES + 6] = scene->mMeshes[i]->mNormals[v].y;
						newMesh[m]->vertex[v * VERTEX_FEATURES + 7] = scene->mMeshes[i]->mNormals[v].z;
					}
				}

				// copy faces
				if (scene->mMeshes[i]->HasFaces())
				{
					newMesh[m]->num_index = scene->mMeshes[i]->mNumFaces * 3;
					newMesh[m]->index = new uint[newMesh[m]->num_index]; // assume each face is a triangle

					for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
					{
						if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
						{
							LOGC( "WARNING, geometry face with != 3 indices!");
						}
						else
						{
							memcpy(&newMesh[m]->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
						}
					}
					LoadMeshData(newMesh[m]);
				}
				else {
					LOGC( "Scene %s, has no faces.", file_path);

					delete newMesh[m];
					newMesh[m] = nullptr;
				}
		}
		LOGC( "% s Pushed In List Successfully", file_path);
		aiReleaseImport(scene);
		
		//He pensat que aqui potser es pot cridar a la funci� CreateEmptyGameObject() posant els varems de la mesh carregada, 
	}
	else
	{
		LOGC( "Error loading scene % s. ERROR: %s", file_path, aiGetErrorString());
	}
	
}

void ModuleFilesManager::Render()
{
	if(textureEnabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	// Wireframe View
	if (wireframe)
	{
		glDisable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < meshList.size(); i++) {
		meshList[i]->DrawMesh();

	}
}

void MeshData::DrawMesh()
{

	glEnable(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, ImgId);

	// ----------------------------------------------------------------------- Bind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, (void*)(3 * sizeof(float)));
	glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, NULL);

	glPushMatrix();

	// Draw
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	glPopMatrix();

	// ----------------------------------------------------------------------- DRAW TEXTURE CORRECTLY
	for (int v = 0; v < num_vertex; v++) {
		//glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES]);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 1]);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 2]);
		glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 3]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 4]);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 5]);
		glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 6]);
		glNormalPointer(GL_FLOAT, sizeof(float) * VERTEX_FEATURES, &vertex[v * VERTEX_FEATURES + 7]);

		//glDrawRangeElements(GL_TRIANGLES, vertex[v * VERTEX_FEATURES], vertex[v * VERTEX_FEATURES + 1], vertex[v * VERTEX_FEATURES + 2], GL_UNSIGNED_SHORT, index);

	}

	// ----------------------------------------------------------------------- UnBind Buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
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
		int const type = ilGetInteger(IL_IMAGE_TYPE);
		int const format = ilGetInteger(IL_IMAGE_FORMAT);
		
		// ---------------------------------------------------------------------------------------------------- Create Texture from ImageData
		glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format,
			type, data);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		ImgId = ilutGLBindTexImage();
		glBindTexture(GL_TEXTURE_2D, ImgId);
		ilDeleteImages(1, &ImgId);

		LOGC("TEX ID: %d", ImgId);

		return ImgId;
	}
	else
	{
		LOGC( "DevIL ERROR: Could not Load Image. Error: %s", ilGetError());

		return 0;
	}

}

void ModuleFilesManager::LoadMeshData(MeshData* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	// ---------------------------------------------------- Buffer Creation
	glGenBuffers(1, (GLuint*)&(mesh->id_vertex));
	glGenBuffers(1, (GLuint*)&(mesh->id_index));

	// ----------------------------------------------------------------------- Bind Buffers
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * VERTEX_FEATURES, mesh->vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);

	glDisableClientState(GL_VERTEX_ARRAY);

	//Push Mesh to the List
	meshList.push_back(mesh);
}