#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleMenus.h"

#include <Windows.h>

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

	ImGui::Begin("GinuhEngine", 0, ImGuiWindowFlags_MenuBar);
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
					ImGui::SliderFloat("Brightness", &screen_brightness, 0.0f, 1.0f);
					ImGui::SliderInt("Width", &screen_width, 720, 1920);
					ImGui::SliderInt("Height", &screen_height, 560, 1080);

					SDL_SetWindowSize(App->window->window, screen_width, screen_height);
					SDL_SetWindowBrightness(App->window->window, screen_brightness);

				}
				

				ImGui::EndMenu();

			}

			ImGui::EndMenu();

		}

		if (ImGui::MenuItem("Hardware"))
		{
			hardwareVisible = !hardwareVisible;

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
		ImGui::SetWindowSize(ImVec2(600.0f, 600.0f));

		//Header
		ImGui::Text("Ginuh Engine");
		
		//About us
		ImGui::Text("A new open source 3D Game Engine made by two students at CITM:");
		ImGui::Text("");
		if (ImGui::Button("David Boces", ImVec2(100, 20)))
		{
			OpenLink("https://github.com/VoZeS");

		}
		if (ImGui::Button("Erik Martin", ImVec2(100, 20)))
		{
			OpenLink("https://github.com/eriik1212");


		}
		ImGui::Text(""); //Intro

		//Libraries
		ImGui::Text("3rd Party Libraries used:");
		//------------------------------------------------------ LIBRARIES AND WEB
		//---------------------------------------- SDL
		ImGui::Bullet();
		if (ImGui::Button("SDL"))
		{
			OpenLink("https://www.libsdl.org/");
		}
		ImGui::SameLine();
		ImGui::Text("v. 2.24.0.0");

		//---------------------------------------- GLEW
		ImGui::Bullet();
		if (ImGui::Button("Glew"))
		{
			OpenLink("http://glew.sourceforge.net/");
		}
		ImGui::SameLine();
		ImGui::Text("v. 2.1.0.0");
		
		//---------------------------------------- IMGUI
		ImGui::Bullet();
		if (ImGui::Button("ImGui"))
		{
			OpenLink("https://github.com/ocornut/imgui");
		}
		ImGui::SameLine();
		ImGui::Text("v. 1.88");

		//---------------------------------------- JSON PARSER
		ImGui::Bullet();
		if (ImGui::Button("JSON Parser"))
		{
			OpenLink("https://www.json.org/json-en.html");
		}
		ImGui::SameLine();
		ImGui::Text("v. 1.1.0");

		//---------------------------------------- JSON PARSER
		ImGui::Bullet();
		if (ImGui::Button("MathGeoLib"))
		{
			OpenLink("https://github.com/juj/MathGeoLib");
		}
		ImGui::SameLine();
		ImGui::Text("v. 1.15");

		ImGui::Text(""); //Intro

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

void ModuleMenus::MenuHardware()
{

	// ERIK HAZ TU MAGIA PICHON

}

void ModuleMenus::OpenLink(const char* url)
{
	ShellExecuteA(0, "open", url, 0, 0, 0);

}
