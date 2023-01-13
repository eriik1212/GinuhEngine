#ifndef __ModuleFilesManager_H__
#define __ModuleFilesManager_H__

#include "Module.h"
#include "Application.h"
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

#include "MathGeoLib/include/Geometry/AABB.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib" )
#pragma comment (lib, "DevIL/libx86/ILUT.lib" )

#include <map>
#include <iostream>
#include <vector>

using namespace std;
namespace fs = std::filesystem;


#define VERTEX_FEATURES 8



#define ASSETS_PATH "Assets/"
#define LIBRARY_PATH "Library/"
#define MODELS_PATH "Library/Models/"
#define MESHES_PATH "Library/Meshes/"
#define TEXTURES_PATH "Library/Textures/"
#define SCENES_PATH "Library/Scenes/"
#define SCRIPTS_PATH "Library/Scripts/"
#define MATERIALS_PATH "Library/Materials/"

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

	AABB aabbBox;
	AABB globalAabbBox;
	OBB obbBox;

	const char* meshName;

	void InitAABB();
	void RenderAABB();
	
	void DrawMesh(const float* globalTransform, uint imgID);
};

struct TextData
{
	string name;

	int textW,
		textH;

	GLuint textureID;
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

	string GetFileName(string path, bool wExtension = false);
	string NormalizePath(const char* path);
	string AdaptPath(const char* full_path);
	string SplitPath(const char* full_path);

	string EraseSubStr(const char* mainStr, const char* toErase);

	unsigned int LoadFile(const char* file, char** buffer) const;
	unsigned int SaveFile(const char* file, const void* buffer, unsigned int size) const;

	//void LoadMeshData(MeshData* mesh);
	//uint LoadTexture(const char* filePath);

	//void NodeManager(const aiScene* rootScene, aiNode* rootNode, GameObject* goParent);

	//uint id_count = 1;

	void SoundBankLoad(const char* path, string base_dir) const;
	bool DuplicateFile(const char* src, const char* dst) const;

	static vector<string> allText;
	static map<string, uint> loaded_textures;			//Know which textures have we loaded, to NOT load again

	vector<MeshData*> meshList;
	vector<TextData*> textList;

private:
	SDL_Event event;                        // Declare event handle
	char* dropped_filedir = "";                  // Pointer for directory of dropped file
	const char* assets_dir = ASSETS_PATH;
	char* texPath = "";

};

static ILuint ImgId;

#endif
