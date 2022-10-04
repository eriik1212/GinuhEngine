#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include "ModuleMenus.h"
#include "Module.h"

#include "SDL/include/SDL.h"
#include "PhysFS/include/physfs.h"

#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

/*#pragma comment( lib, "Assimp/libx86/assimp.lib")
#pragma comment( lib, "Assimp/libx86/zlibstatic.lib")

#pragma comment( lib, "DevIL/libx86/DevIL.lib")
#pragma comment( lib, "DevIL/libx86/ILU.lib")
#pragma comment( lib, "DevIL/libx86/ILUT.lib")

#pragma comment( lib, "PhysFS/libx86/physfs.lib")*/

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);
	
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;
	
	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			App = new Application();
			state = MAIN_START;
			//LOG("-------------- Application Creation --------------");
			App->menus->my_log.AddLog(("-------------- Application Creation --------------"));
			
			break;

		case MAIN_START:

			//LOG("-------------- Application Init --------------");
			App->menus->my_log.AddLog("-------------- Application Init --------------");

			if (App->Init() == false)
			{
				//LOG("Application Init exits with ERROR");
				App->menus->my_log.AddLog("Application Init exits with ERROR");

				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				//LOG("-------------- Application Update --------------");
				App->menus->my_log.AddLog("-------------- Application Update --------------");

			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				//LOG("Application Update exits with ERROR");
				App->menus->my_log.AddLog("Application Update exits with ERROR");

				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			//LOG("-------------- Application CleanUp --------------");
			App->menus->my_log.AddLog("-------------- Application CleanUp --------------");

			if (App->CleanUp() == false)
			{
				//LOG("Application CleanUp exits with ERROR");
				App->menus->my_log.AddLog("Application CleanUp exits with ERROR");

			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}
	//LOG("Exiting game '%s'...\n", TITLE);
	App->menus->my_log.AddLog(("Exiting game '%s'...", TITLE));
	delete App;
	

	return main_return;
}