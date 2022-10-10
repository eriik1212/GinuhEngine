#ifndef __ModuleFilesManager_H__
#define __ModuleFilesManager_H__

#include "Module.h"
#include "Globals.h"
#include "Glew/include/glew.h"

using namespace std;

struct MeshData
{
	MeshData(){}

	~MeshData() {
		delete[num_vertex]  vertex;
		delete[num_index]  index;

	}
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	void DrawMesh();
};

class ModuleFilesManager : public Module
{
public:
	ModuleFilesManager(Application* app, bool start_enabled = true);
	~ModuleFilesManager();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadFile(const char* filePath, MeshData* ourMesh);

	static void Render();

	MeshData houseMesh;		
	static vector<MeshData*> meshList;
};

#endif
