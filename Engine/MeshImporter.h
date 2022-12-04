#pragma once
#include "Globals.h"
#include "ModuleFilesManager.h"
#include "GameObject.h"
#include "PhysFS/include/physfs.h"

#include "TextureImporter.h"

#include <vector>

struct MeshData;
struct TexthData;
//class aiScene;
//class aiNode;
class GameObject;

namespace MeshImporter
{
	void ImportMesh(const char* file_path);
	void Save(MeshData* mesh);
	void Load(MeshData* mesh);

	void LoadMeshData(vector<MeshData*>& meshList, MeshData* mesh);
	void NodeManager(aiMesh** meshArray, const aiScene* rootScene, vector<TextData*>& sceneTextures, vector<MeshData*>& meshList, aiNode* rootNode, GameObject* goParent, const char* name);
}