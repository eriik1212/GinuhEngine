#ifndef __ModuleFilesManager_H__
#define __ModuleFilesManager_H__

#include "Module.h"
#include "Globals.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "GameObject.h"

#include "Glew/include/glew.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib" )
#pragma comment (lib, "DevIL/libx86/ILUT.lib" )

#include <map>

using namespace std;

#define VERTEX_FEATURES 8

struct MeshData
{
	MeshData(){}

	~MeshData() {

		delete vertex;
		vertex = nullptr;

		delete index;
		index = nullptr;
	}

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	uint texture_id = 0;

	const char* meshName;

	void DrawMesh(const float* globalTransform, uint imgID);
};


class ModuleFilesManager : public Module
{
public:
	ModuleFilesManager(Application* app, bool start_enabled = true);
	~ModuleFilesManager();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadFile(const char* filePath);
	void LoadMeshData(MeshData* mesh);
	uint LoadTexture(const char* filePath);

	void NodeManager(const aiScene* rootScene, aiNode* rootNode, GameObject* goParent);

	uint id_count = 1;

	static vector<string> allText;

private:
	SDL_Event event;                        // Declare event handle
	char* dropped_filedir = "";                  // Pointer for directory of dropped file
	const char* assets_dir = "Assets/";
	char* texPath = "";

	vector<MeshData*> meshList;

	uint textID;

	static map<string, uint> loaded_textures;			//Know which textures have we loaded, to NOT load again


};

static ILuint ImgId;

#endif
