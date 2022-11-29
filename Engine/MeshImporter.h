#pragma once
#include "Globals.h"
#include "ModuleFilesManager.h"
#include "GameObject.h"

#include "TextureImporter.h"

#include <vector>

struct MeshData;
//class aiScene;
//class aiNode;
class GameObject;

namespace MeshImporter
{
	void ImportMesh(const char* file_path);
	void Save(MeshData* mesh);
	void Load(MeshData* mesh);

	void LoadMeshData(vector<MeshData*> meshList, MeshData* mesh);
	void NodeManager(const aiScene* rootScene, aiNode* rootNode, GameObject* goParent);

}