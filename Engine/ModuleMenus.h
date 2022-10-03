#ifndef __ModuleMenus_H__
#define __ModuleMenus_H__

#include "Module.h"
#include "Globals.h"

class ModuleMenus : public Module
{
public:
	ModuleMenus(Application* app, bool start_enabled = true);
	~ModuleMenus();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool pOpen_config = true,
		pOpen_about = true,
		pOpen_console = true;

private:
	bool aboutVisible = false;
	void MenuAbout();

	bool configVisible = false;
	void MenuConfig();

	bool consoleVisible = false;
	void MenuConsole();

	//--------------------------------------------- OPTIONS
	//window
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool full_desk = WIN_FULLSCREEN_DESKTOP;
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	//visual
	bool vsync = VSYNC;
	bool lights = true;
	bool depth_test = true;
	bool cull_face = true;
	bool color_material = true;
	bool texture2D = true;
	bool wireframe_view = false;
	float screen_brightness = 1;

	bool hardwareActive = true;

	void OpenLink(const char* url);

};

#endif