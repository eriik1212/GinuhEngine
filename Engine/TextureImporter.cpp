#include "TextureImporter.h"
#include "Application.h"
#include "PhysFS/include/physfs.h"

std::map<std::string, uint> ModuleFilesManager::loaded_textures;
vector<string> ModuleFilesManager::allText;

uint TextureImporter::ImportTexture(const char* filePath)
{
	ilInit();
	iluInit();
	ilutInit();

	// Check loaded textures
	if (ModuleFilesManager::loaded_textures.find(filePath) != ModuleFilesManager::loaded_textures.end())
	{
		return ModuleFilesManager::loaded_textures[filePath];
	}

	// -------------------------------------- Loading Image
	if (ilLoadImage(filePath))
	{
		ilEnable(IL_FILE_OVERWRITE);
		ilSaveImage(filePath);

		ilGenImages(1, &ImgId);
		ilBindImage(ImgId);

		ilLoadImage(filePath);

		ImgId = ilutGLBindTexImage();

		ilBindImage(0);
		ilDeleteImages(1, &ImgId);

		ModuleFilesManager::allText.push_back(filePath);

		ModuleFilesManager::loaded_textures[filePath] = ImgId;

	//	// ------------------------------------------ It prints also de grid (WRONG!)
	//	//ilEnable(IL_FILE_OVERWRITE);
	//	//ilSaveImage(filePath);

	//	//ilGenImages(1, &ImgId);
	//	//ilBindImage(ImgId);

	//	//ilLoadImage(filePath);

	//	////ilBindImage(ImgId);
	//	//BYTE* data = ilGetData();

	//	//ILuint imgWidth, imgHeight;
	//	//imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
	//	//imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	//	//int const type = ilGetInteger(IL_IMAGE_TYPE);
	//	//int const format = ilGetInteger(IL_IMAGE_FORMAT);

	//	//// ---------------------------------------------------------------------------------------------------- Create Texture from ImageData
	//	//glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format,
	//	//	type, data);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//	//ImgId = ilutGLBindTexImage();
	//	//glBindTexture(GL_TEXTURE_2D, ImgId);
	//	//ilDeleteImages(1, &ImgId);

		AppExtern->menus->info.AddConsoleLog("TEX ID: %d", ImgId);

	//	return ImgId;
	}
	else
	{
		//App->menus->info.AddConsoleLog("DevIL ERROR: Could not Load Image. Error: %s", ilGetError());

		return 0;
	}

	return ImgId;

}

void TextureImporter::SaveTexture(char** fileBuffer, string path)
{
	ILuint size = 0;
	ILubyte* data = nullptr;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, nullptr, 0); // Get the size of the data buffer
	
	if (size > 0) 
	{
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			*fileBuffer = (char*)data;

			RELEASE_ARRAY(data);
	}

}