#include "MeshImporter.h"
#include "Application.h"
#include "TextureImporter.h"

void MeshImporter::ImportMesh(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	vector<MeshData*> meshList;

	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* GameObjectRoot = new GameObject(AppExtern->scene_intro->gameObjects[0], fs::path(file_path).filename().string());

		//App->scene_intro->gameObjects[1] = GameObjectRoot;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			GameObject* GameObjectChild = new GameObject(GameObjectRoot, scene->mMeshes[i]->mName.C_Str());

			MeshData* newMesh = new MeshData();

			newMesh->meshName = scene->mMeshes[i]->mName.C_Str();

			// copy vertices
			newMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			newMesh->vertex = new float[newMesh->num_vertex * VERTEX_FEATURES];
			//memcpy(newMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * newMesh->num_vertex * 3);
			AppExtern->menus->info.AddConsoleLog("New mesh with %d vertices", newMesh->num_vertex);

			for (int v = 0; v < newMesh->num_vertex; v++) {
				// Vertex
				newMesh->vertex[v * VERTEX_FEATURES] = scene->mMeshes[i]->mVertices[v].x;
				newMesh->vertex[v * VERTEX_FEATURES + 1] = scene->mMeshes[i]->mVertices[v].y;
				newMesh->vertex[v * VERTEX_FEATURES + 2] = scene->mMeshes[i]->mVertices[v].z;

				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					// UVs
					newMesh->vertex[v * VERTEX_FEATURES + 3] = scene->mMeshes[i]->mTextureCoords[0][v].x;
					newMesh->vertex[v * VERTEX_FEATURES + 4] = scene->mMeshes[i]->mTextureCoords[0][v].y;
				}
				// -------------------------------------------------------------------------------------- In a future
				if (scene->mMeshes[i]->HasNormals())
				{
					newMesh->vertex[v * VERTEX_FEATURES + 5] = scene->mMeshes[i]->mNormals[v].x;
					newMesh->vertex[v * VERTEX_FEATURES + 6] = scene->mMeshes[i]->mNormals[v].y;
					newMesh->vertex[v * VERTEX_FEATURES + 7] = scene->mMeshes[i]->mNormals[v].z;
				}
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
						AppExtern->menus->info.AddConsoleLog("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&newMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

				// extract materials/textures
				if (scene->HasMaterials())
				{
					const aiMesh* mesh = scene->mMeshes[i];

					aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
					uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
					aiString path;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					aiString sourcePath;
					sourcePath.Set(ASSETS_PATH);

					sourcePath.Append(path.C_Str());

					LOG(sourcePath.C_Str());

					// ------------------------------------ Load Texture Here???
					// 
					newMesh->texture_id = TextureImporter::LoadTexture(sourcePath.C_Str());
					
					dynamic_cast<C_Material*>(GameObjectChild->CreateComponent(Component::C_TYPE::MATERIAL))->SetTexture(sourcePath.C_Str(), newMesh);
				}
			}
			else {
				AppExtern->menus->info.AddConsoleLog("Scene %s, has no faces.", file_path);

				delete newMesh;
				newMesh = nullptr;

			}

			NodeManager(scene, scene->mRootNode, GameObjectRoot);

			dynamic_cast<C_Mesh*>(GameObjectChild->CreateComponent(Component::C_TYPE::MESH))->SetMesh(newMesh, scene->mMeshes[i]->mName.C_Str());

			LoadMeshData(meshList, newMesh);
		}
		AppExtern->menus->info.AddConsoleLog("% s Pushed In List Successfully", file_path);
		aiReleaseImport(scene);


	}
	else
	{
	AppExtern->menus->info.AddConsoleLog("Error loading scene % s. ERROR: %s", file_path, aiGetErrorString());
	}

}
void MeshImporter::Save(MeshData* mesh)
{
	// amount of indices / vertices
	uint ranges[2] = { mesh->num_index, mesh->num_vertex };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->num_index + sizeof(float) * mesh->num_vertex * 3;
	
	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;
	
	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes;

	// Store indices
	bytes = sizeof(uint) * mesh->num_index;
	memcpy(cursor, mesh->index, bytes);
	cursor += bytes;

	// Store vertex
	bytes = sizeof(float) * mesh->num_vertex;
	memcpy(cursor, mesh->vertex, bytes);
	cursor += bytes;
}

void MeshImporter::Load(MeshData* mesh)
{
	char* buffer = NULL;
	char* cursor = buffer;

	// amount of indices / vertices
	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	mesh->num_index = ranges[0];
	mesh->num_vertex = ranges[1];

	// Load indices
	bytes = sizeof(uint) * mesh->num_index;
	mesh->index = new uint[mesh->num_index];
	memcpy(mesh->index, cursor, bytes);
	cursor += bytes;

	// Load vertex
	bytes = sizeof(float) * mesh->num_vertex;
	mesh->vertex = new float[mesh->num_vertex];
	memcpy(mesh->vertex, cursor, bytes);
	cursor += bytes;
}

void MeshImporter::LoadMeshData(vector<MeshData*> meshList, MeshData* mesh)
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

void MeshImporter::NodeManager(const aiScene* rootScene, aiNode* rootNode, GameObject* goParent)
{
	aiVector3D translation, scaling;
	aiQuaternion quatRot;
	rootNode->mTransformation.Decompose(scaling, quatRot, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(quatRot.x, quatRot.y, quatRot.z, quatRot.w);

	goParent->transform->SetTransform(pos, rot, scale);

	// We make it recursive for its children
	if (rootNode->mNumChildren > 0)
	{
		for (int n = 0; n < rootNode->mNumChildren; n++)
		{
			NodeManager(rootScene, rootNode->mChildren[n], goParent);
			//dynamic_cast<C_Transform*>(App->scene_intro->gameObjects[goID]->GetComponent(Component::C_TYPE::TRANSFORM))->SetTransform(pos, rot, scale);
			//dynamic_cast<C_Material*>(App->scene_intro->gameObjects[goID]->CreateComponent(Component::C_TYPE::MATERIAL))->SetTexture();

		}
	}
}