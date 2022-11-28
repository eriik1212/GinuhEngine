#pragma once
#include "Globals.h"
#include "Application.h"

namespace MeshImporter
{
	void Import();
	void Save(MeshData* mesh);
	void Load(char* buffer, MeshData* mesh);

}