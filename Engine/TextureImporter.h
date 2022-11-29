#pragma once
#include "Globals.h"
#include "ModuleFilesManager.h"

namespace TextureImporter
{
	uint ImportTexture(const char* filePath);
	char* SaveTexture(uint& size, std::string path);
	GLuint LoadTexture(string path);

}
