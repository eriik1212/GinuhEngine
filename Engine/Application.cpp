#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	menus = new ModuleMenus(this);
	files_manager = new ModuleFilesManager(this);
	
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	//ImGui Module
	AddModule(menus);

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);

	//Files Manager
	AddModule(files_manager);

	// Scene
	AddModule(scene_intro);

	//game obejct
	//still working on it :)

	// Renderer
	AddModule(renderer3D);

	

	

}

Application::~Application()
{
	for (int i = list_modules.size() - 1; i >= 0; i--)
	{
		delete list_modules[i];
		list_modules[i] = nullptr;
	}
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (size_t i = 0; i < list_modules.size(); i++)
	{
		ret = list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	//LOG("-------------- Application Start --------------");
	menus->info.AddConsoleLog("-------------- Application Start --------------");

	for (size_t i = 0; i < list_modules.size(); i++)
	{
		ret = list_modules[i]->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	//We make the update stops x time to keep a frecuency of actualization
	Uint32 lastFrameMs = ms_timer.Read();
	float timeToWait = (1000.f / (float)limitFPS) - (float)lastFrameMs;
	SDL_Delay(static_cast<Uint32>(fabs(timeToWait)));


}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (size_t i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate(dt);
	}

	for (size_t i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update(dt);
	}

	for (size_t i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (int i = list_modules.size() - 1; i >= 0; i--)
	{
		list_modules[i]->CleanUp();
		delete list_modules[i];
		list_modules[i] = nullptr;
	}
	list_modules.clear();

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}