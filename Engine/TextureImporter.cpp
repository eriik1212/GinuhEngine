#include "TextureImporter.h"
#include "Application.h"

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

		// ------------------------------------------ It prints also de grid (WRONG!)
		//ilEnable(IL_FILE_OVERWRITE);
		//ilSaveImage(filePath);

		//ilGenImages(1, &ImgId);
		//ilBindImage(ImgId);

		//ilLoadImage(filePath);

		////ilBindImage(ImgId);
		//BYTE* data = ilGetData();

		//ILuint imgWidth, imgHeight;
		//imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
		//imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		//int const type = ilGetInteger(IL_IMAGE_TYPE);
		//int const format = ilGetInteger(IL_IMAGE_FORMAT);

		//// ---------------------------------------------------------------------------------------------------- Create Texture from ImageData
		//glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format,
		//	type, data);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//ImgId = ilutGLBindTexImage();
		//glBindTexture(GL_TEXTURE_2D, ImgId);
		//ilDeleteImages(1, &ImgId);

		AppExtern->menus->info.AddConsoleLog("TEX ID: %d", ImgId);

		return ImgId;
	}
	else
	{
		//App->menus->info.AddConsoleLog("DevIL ERROR: Could not Load Image. Error: %s", ilGetError());

		return 0;
	}

}

char* TextureImporter::SaveTexture(uint& size, string path)
{
	char* buffer = NULL;

	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (!ilLoadL(IL_TYPE_UNKNOWN, buffer, size))
	{
		AppExtern->menus->info.AddConsoleLog("Image Not Loaded.");
	}

	ILuint _size = 0;
	ILubyte* data = nullptr;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	_size = ilSaveL(IL_DDS, nullptr, 0);
	if (_size > 0)
	{
		data = new ILubyte[_size];
		size = ilSaveL(IL_DDS, data, _size);
	}

	ilDeleteImages(1, &imageID);

	return (char*)data;

}

uint TextureImporter::LoadTexture(string path)
{
	string texture_path = TEXTURES_PATH + AppExtern->files_manager->GetFileName(path, false) + ".dds";

	// Check loaded textures
	if (!AppExtern->files_manager->FS_Exists(texture_path))
	{
		// save custom format
		string file = TEXTURES_PATH;
		file += AppExtern->files_manager->GetFileName(path, false);
		file += ".dds";

		uint size = 0;
		char* buffer = SaveTexture(size, path);

		RELEASE_ARRAY(buffer);
	}

	return ImportTexture(texture_path.c_str());
}