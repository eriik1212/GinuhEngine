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

};

#endif