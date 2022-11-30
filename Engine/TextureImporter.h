#pragma once
#include "Globals.h"
#include "ModuleFilesManager.h"

namespace TextureImporter
{
	uint ImportTexture(const char* filePath);
	void SaveTexture(char** fileBuffer, string path);
	//GLuint LoadTexture(string path);

}
