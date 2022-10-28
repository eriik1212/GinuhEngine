#ifndef __ModuleMenus_H__
#define __ModuleMenus_H__

#include <vector>

#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

class ModuleMenus : public Module
{
public:
	ModuleMenus(Application* app, bool start_enabled = true);
	~ModuleMenus();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool pOpen_config = true,
		pOpen_about = true,
		pOpen_console = true;

	bool vsync = VSYNC;
    

private:
	bool aboutVisible = false;
	void MenuAbout();

	bool configVisible = true;
	void MenuConfig();

	bool consoleVisible = true;
	void MenuConsole();


	//--------------------------------------------- OPTIONS
	//window
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool fullDesk = WIN_FULLSCREEN_DESKTOP;
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;

	//visual

	bool lights = true;
	bool depthTest = true;
	bool cullFace = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframeView = false;
	float screenBrightness = 1;

	bool hardwareActive = true;

	void OpenLink(const char* url);
	void PushLog(std::vector<float>* Log, float toPush);

	//FPS

	std::vector<float> fpsLog;
	std::vector<float> timeLog;

	//ConsoleInfo info;
	
};

#endif
