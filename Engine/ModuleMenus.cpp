#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleMenus.h"

ModuleMenus::ModuleMenus(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleMenus::~ModuleMenus()
{}

// Called before render is available
bool ModuleMenus::Init()
{
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	return ret;
}

// PostUpdate present buffer to screen
update_status ModuleMenus::PostUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	ImGui::Begin("GinuhEngine",0, ImGuiWindowFlags_MenuBar);
	ImGui::Text("Welcome to this fantastic engine!");


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Configuration"))
		{
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::Checkbox("FullScreen", &fullscreen))
				{
					if (fullscreen)	SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN); //FULLSCREEN ENABLED
					else SDL_SetWindowFullscreen(App->window->window, !SDL_WINDOW_FULLSCREEN); //FULLSCREEN DISABLED

				}
				if (!fullscreen) //RESIZABLE, BORDER AND SIZE OPTIONS ONLY APPEARS IF IT IS IN WINDOW MODE
				{
					if (ImGui::Checkbox("Resizable", &resizable))
					{
						if (resizable)	SDL_SetWindowResizable(App->window->window, SDL_TRUE); //RESIZABLE ENABLED
						else SDL_SetWindowResizable(App->window->window, SDL_FALSE);//RESIZABLE DISABLED

					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Borderless", &borderless))
					{
						if (borderless)	SDL_SetWindowBordered(App->window->window, SDL_FALSE); //BORDERLESS ENABLED
						else SDL_SetWindowBordered(App->window->window, SDL_TRUE); //BORDERLESS DISABLED

					}
					ImGui::SliderInt("Width", &screen_width, 720, 1920);
					ImGui::SliderInt("Height", &screen_height, 560, 1080);

					SDL_SetWindowSize(App->window->window, screen_width, screen_height);
				}
				

				ImGui::EndMenu();

			}

			ImGui::EndMenu();

		}


		if (ImGui::MenuItem("About"))
		{
			aboutVisible = !aboutVisible;
		}

		if (ImGui::MenuItem("Exit"))
		{
			return UPDATE_STOP;
		}
		ImGui::EndMenuBar();

	}

	ImGui::End();

	if (aboutVisible) MenuAbout();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleMenus::CleanUp()
{
	LOG("Destroying Editor");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleMenus::MenuAbout()
{
	if (ImGui::Begin("About", 0, ImGuiWindowFlags_MenuBar))
	{
		
		//Header
		ImGui::Text("Ginuh Engine");
		
		//About us
		ImGui::Text("A new open source 3D Game Engine made by two students at CITM:");
		ImGui::Text("");
		ImGui::Text("David Boces");
		ImGui::Text("Erik Martin");
		ImGui::Text(""); //Intro

		//Libraries
		ImGui::Text("3rd Party Libraries used:");
		ImGui::BulletText("SDL 2.24.0.0");
		ImGui::BulletText("Glew 2.1.0.0");
		ImGui::BulletText("ImGui 1.88");
		ImGui::BulletText("JSON Parser 1.1.0");
		ImGui::BulletText("MathGeoLib 1.15");
		ImGui::Text("");

		//License
		ImGui::Text("License:");
		ImGui::Text("");
		ImGui::Text("MIT License");
		ImGui::Text("");
		ImGui::Text("Copyright (c) 2022 Erik Martin Garzon");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this softwareand associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\n\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");
	}
	ImGui::End();

}