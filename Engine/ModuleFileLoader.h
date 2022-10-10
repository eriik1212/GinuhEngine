#ifndef __ModuleFileLoader_H__
#define __ModuleFileLoader_H__

#include "Module.h"
#include "Globals.h"
#include "Glew/include/glew.h"

struct MeshData
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	void DrawMesh();
};

class ModuleFileLoader : public Module
{
public:
	ModuleFileLoader(Application* app, bool start_enabled = true);
	~ModuleFileLoader();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void LoadFile(const char* filePath, MeshData* ourMesh);

	MeshData houseMesh;

};

#endif
