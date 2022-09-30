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
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	bool aboutVisible = false;
	void MenuAbout();
	
	bool hardwareVisible = false;
	void MenuHardware();

	//OPTIONS
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool full_desk = WIN_FULLSCREEN_DESKTOP;
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;
	float screen_brightness = 1;

	bool hardwareActive = true;

	void OpenLink(const char* url);

};

#endif