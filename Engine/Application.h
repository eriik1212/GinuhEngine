#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleMenus.h"
#include "ModuleFilesManager.h"

#include <vector>
using namespace std;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleMenus* menus;
	ModuleFilesManager* files_manager;

private:

	Timer	ms_timer;
	float	dt;
	vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	//utils
	float GetDt() const { return dt; }
	float GetFrameRate()const { return 1.f / dt; }

	int limitFPS=60;
	int screenAct;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};