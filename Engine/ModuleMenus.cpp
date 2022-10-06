#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"

#include "Primitive.h"
#include "Application.h"

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

update_status ModuleMenus::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

update_status ModuleMenus::Update(float dt)
{
	float FPS = floorf(App->GetFrameRate());
	float MS = (App->GetDt() * 1000.f);

	PushLog(&fpsLog, FPS);
	PushLog(&timeLog, MS);

	return UPDATE_CONTINUE;
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
		if (ImGui::MenuItem("Configuration"))
		{
			pOpen_config = true; // Window can close

			configVisible = !configVisible;

		}

		if (ImGui::MenuItem("About"))
		{
			pOpen_about = true; // Window can close

			aboutVisible = !aboutVisible;
		}

		if (ImGui::MenuItem("Console"))
		{
			pOpen_console = true; // Window can close

			consoleVisible = !consoleVisible;
		}

		if (ImGui::MenuItem("Exit"))
		{
			return UPDATE_STOP;
		}
		ImGui::EndMenuBar();

	}

	ImGui::End();

	if (configVisible) MenuConfig();
	if (aboutVisible) MenuAbout();
	if (consoleVisible) MenuConsole();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleMenus::CleanUp()
{
	//LOG("Destroying ModuleMenus");
	my_log.AddLog("Destroying Editor");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleMenus::MenuAbout()
{
	if (pOpen_about)
	{
		if (ImGui::Begin("About", &pOpen_about))
		{
			ImGui::SetWindowSize(ImVec2(600.0f, 620.0f));

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

			//---------------------------------------- MATHGEOLIB
			ImGui::Bullet();
			if (ImGui::Button("MathGeoLib"))
			{
				OpenLink("https://github.com/juj/MathGeoLib");
			}
			ImGui::SameLine();
			ImGui::Text("v. 1.15");

			//---------------------------------------- MATHGEOLIB
			ImGui::Bullet();
			if (ImGui::Button("Assimp"))
			{
				OpenLink("https://github.com/assimp/assimp");
			}
			ImGui::SameLine();
			ImGui::Text("v. 3.1.1");

			//---------------------------------------- MATHGEOLIB
			ImGui::Bullet();
			if (ImGui::Button("DevIL"))
			{
				OpenLink("https://github.com/DentonW/DevIL");
			}
			ImGui::SameLine();
			ImGui::Text("v. 1.8.0");

			//---------------------------------------- MATHGEOLIB
			ImGui::Bullet();
			if (ImGui::Button("PhysFS"))
			{
				OpenLink("https://github.com/icculus/physfs");
			}
			ImGui::SameLine();
			ImGui::Text("v. 3.0.2");

			ImGui::Text(""); //Intro

			//License
			ImGui::Text("License:");
			ImGui::Text("");
			ImGui::Text("MIT License");
			ImGui::Text("");
			ImGui::Text("Copyright (c) 2022 Erik Martin Garzon & David Boces Obis");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this softwareand associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\n\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.\n\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");
		}
		ImGui::End();

		if (pOpen_about == NULL) aboutVisible = !aboutVisible; // Window is closed so function "MenuAbout()" stops being called
	}

}

void ModuleMenus::MenuConfig()
{
	if (pOpen_config)
	{
		if (ImGui::Begin("Configuration", &pOpen_config))
		{
			ImGui::SetWindowSize(ImVec2(450.0f, 300.0f));
			ImGui::TextColored(ImVec4(155, 155, 0, 255), "OPTIONS");

			if (ImGui::CollapsingHeader("Application"))
			{
				//Header
				ImGui::LabelText( "Ginuh Engine", "App Name:");
				ImGui::LabelText( "UPC CITM", "Organisation:");
				ImGui::SliderInt("Max FPS", &App->limitFPS, 1, 165);
				
				


				ImGui::Text("Limit Framerate: ");

				char title[25];
				sprintf_s(title, 25, "Framerate %1.f", fpsLog[fpsLog.size() - 1]);
				ImGui::PlotHistogram("##framerate", &fpsLog[0],fpsLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
				sprintf_s(title, 25, "Milliseconds %0.f", timeLog[timeLog.size() - 1]);
				ImGui::PlotHistogram("##milliseconds", &timeLog[0], timeLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));



			}
			if (ImGui::CollapsingHeader("Window"))
			{
				if (ImGui::Checkbox("FullScreen", &fullscreen))
				{
					if (fullscreen)
					{
						SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN); //FULLSCREEN ENABLED
						my_log.AddLog("FullScreen Enabled");

					}
					else
					{
						SDL_SetWindowFullscreen(App->window->window, !SDL_WINDOW_FULLSCREEN); //FULLSCREEN DISABLED
						my_log.AddLog("FullScreen Disabled");

					}
				}
				if (!fullscreen) //RESIZABLE, BORDER AND SIZE OPTIONS ONLY APPEARS IF IT IS IN WINDOW MODE
				{
					if (ImGui::Checkbox("Resizable", &resizable))
					{
						if (resizable)
						{
							SDL_SetWindowResizable(App->window->window, SDL_TRUE); //RESIZABLE ENABLED
							my_log.AddLog("Resizable Mode Enabled");

						}
						else
						{
							SDL_SetWindowResizable(App->window->window, SDL_FALSE);//RESIZABLE DISABLED
							my_log.AddLog("Resizable Mode Disabled");

						}
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Borderless", &borderless))
					{
						if (borderless)
						{
							SDL_SetWindowBordered(App->window->window, SDL_FALSE); //BORDERLESS ENABLED
							my_log.AddLog("Borderless Mode Enabled");

						}
						else
						{
							SDL_SetWindowBordered(App->window->window, SDL_TRUE); //BORDERLESS DISABLED
							my_log.AddLog("Borderless Mode Disabled");

						}
					}

					if (ImGui::SliderInt("Width", &screen_width, 720, 1920) ||
						ImGui::SliderInt("Height", &screen_height, 560, 1080))
					{
						SDL_SetWindowSize(App->window->window, screen_width, screen_height);
						my_log.AddLog("Window Size Change");

					}


				}
			}
			if (ImGui::CollapsingHeader("Visual"))
			{
				if (ImGui::Checkbox("Vsync", &vsync))
				{
					if (vsync)
					{
						SDL_GL_SetSwapInterval(1); //VSYNC ENABLED
						my_log.AddLog("Vsync Enabled");

					}
					else
					{
						SDL_GL_SetSwapInterval(0); //VSYNC DISABLED
						my_log.AddLog("Vsync Disabled");

					}
				}

				if (ImGui::SliderFloat("Brightness", &screen_brightness, 0.300f, 1.000f))
				{
					SDL_SetWindowBrightness(App->window->window, screen_brightness);
					my_log.AddLog("Brightness Change");

				}

				if (ImGui::Checkbox("Lights", &lights))
				{

					if (lights)
					{
						glEnable(GL_LIGHTING); //LIGHTS ENABLED
						my_log.AddLog("Lights Enabled");

					}
					else
					{
						glDisable(GL_LIGHTING); //LIGHTS DISABLED
						my_log.AddLog("Lights Disabled");
					}

				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Depth Test", &depth_test))
				{
					if (depth_test)
					{
						glEnable(GL_DEPTH_TEST); //DEPTH TEST ENABLED
						my_log.AddLog("Depth Test Enabled");

					}
					else
					{
						glDisable(GL_DEPTH_TEST); //DEPTH TEST DISABLED
						my_log.AddLog("Depth Test Disabled");

					}
				}
				if (ImGui::Checkbox("Cull Face", &cull_face))
				{
					if (cull_face)
					{
						glEnable(GL_CULL_FACE); //CULL FACE ENABLED
						my_log.AddLog("Cull Face Enabled");

					}
					else
					{
						glDisable(GL_CULL_FACE); //CULL FACE  DISABLED
						my_log.AddLog("Cull Face Disabled");

					}
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Color Material", &color_material))
				{
					if (color_material)
					{
						glEnable(GL_COLOR_MATERIAL); //COLOR MATERIAL ENABLED
						my_log.AddLog("Color Material Enabled");

					}
					else
					{
						glDisable(GL_COLOR_MATERIAL); //COLOR MATERIAL  DISABLED
						my_log.AddLog("Color Material Disabled");

					}
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Texture 2D", &texture2D))
				{
					if (texture2D)
					{
						glEnable(GL_TEXTURE_2D); //TEXTURE 2D ENABLED
						my_log.AddLog("Texture 2D Enabled");

					}
					else
					{
						glDisable(GL_TEXTURE_2D); //TEXTURE 2D DISABLED
						my_log.AddLog("Texture 2D Disabled");

					}
				}
				if (ImGui::Checkbox("Wireframe View", &wireframe_view))
				{
					if (wireframe_view)
					{
						App->scene_intro->wireframe = true; //WIREFRAME VIEW ENABLED
						my_log.AddLog("Wireframe View Enabled");

					}
					else
					{
						App->scene_intro->wireframe = false; //WIREFRAME VIEW DISABLED
						my_log.AddLog("Wireframe View Disabled");

					}

					// Updated in "ModuleSceneIntro -> Update()"
				}

			}
			if (ImGui::CollapsingHeader("Hardware"))
			{

				//Header
				if (ImGui::Checkbox("Active", &hardwareActive))
				{
					if (hardwareActive)
					{
						//Aqui ha de fer algo pero encara no se exactament que
					}
					else
					{
						//Demanar al miquel a veure que vol dir aques checkbox
					}

				}

				ImGui::Text("-----");

				//SDL Version
				SDL_version version;
				SDL_GetVersion(&version);

				ImGui::Text("SDL Version:");
				ImGui::SameLine();
				ImGui::TextColored({ 255,255,0,20 }, "%u.%u.%u", version.major, version.minor, version.patch);

				ImGui::Text("-----");

				//CPU

				//Cache

				int SDL_GetCPUCount(void); //Aquest retorna el numero de nuclis del cpu.
				int SDL_GetCPUCacheLineSize(void); //Aquest retorna el tamany de la primera linea de cache de la CPU


				ImGui::Text("CPUs:");
				ImGui::SameLine();
				ImGui::TextColored({ 255,255,0,20 }, "%d", SDL_GetCPUCount());

				//Ram del sistema
				int SDL_GetSystemRAM(void); //Aquesta retorna la cantitat de ram de la que disposem

				ImGui::Text("System RAM:");
				ImGui::SameLine();
				ImGui::TextColored({ 255,255,0,20 }, "%dGb", SDL_GetSystemRAM() / 1000);

				//Caps--------------------------------
				ImGui::Text("Caps:");

				//Initiation of parameters

				SDL_bool SDL_Has3DNow(void);
				SDL_bool SDL_HasAltiVec(void);
				SDL_bool SDL_HasAVX(void);
				SDL_bool SDL_HasAVX2(void);
				SDL_bool SDL_HasMMX(void);
				SDL_bool SDL_HasRDTSC(void);
				SDL_bool SDL_HasSSE(void);
				SDL_bool SDL_HasSSE2(void);
				SDL_bool SDL_HasSSE3(void);
				SDL_bool SDL_HasSSE41(void);
				SDL_bool SDL_HasSSE42(void);

				//3dNow


				if (SDL_Has3DNow())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "3DNow!");
				}

				//AltiVec

				if (SDL_HasAltiVec())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "AltiVec");
				}


				//AVX
				if (SDL_HasAVX())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "AVX");
				}

				//AVX2
				if (SDL_HasAVX2())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "AVX2");
				}
				//MMX
				if (SDL_HasMMX())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "MMX");
				}
				//RDTSC
				if (SDL_HasRDTSC())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "RDTSC");
				}
				//SSE
				if (SDL_HasSSE())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "SSE");
				}
				//SSE2
				if (SDL_HasSSE2())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "SSE2");
				}
				//SSE3
				if (SDL_HasSSE3())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "SSE3");
				}
				//SSE41
				if (SDL_HasSSE41())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "SSE41");
				}
				//SSE42
				if (SDL_HasSSE42())
				{
					ImGui::SameLine();
					ImGui::TextColored({ 255,255,0,20 }, "SSE42");
				}

				ImGui::Text("-----");

				//GPU
				const char* SDL_GetCurrentVideoDriver(void);
				ImGui::Text("Brand:");
				ImGui::SameLine();
				ImGui::TextColored({ 255,255,0,20 }, "%s", SDL_GetCurrentVideoDriver());

			}
		}

		ImGui::End();

		if (pOpen_config == NULL) configVisible = !configVisible; // Window is closed so function "MenuConfig()" stops being called
	}
}

void ModuleMenus::MenuConsole()
{
	if (pOpen_console)
	{
		my_log.Draw("Console", &pOpen_console);
	}
	if (pOpen_console == NULL)
	{
		consoleVisible = !consoleVisible; // Window is closed so function "MenuConsole()" stops being called
	}
}

void ModuleMenus::OpenLink(const char* url)
{
	ShellExecuteA(0, "open", url, 0, 0, 0);

}

void ModuleMenus::PushLog(std::vector<float>* Log, float toPush) //Function to keep actualizing the ms and fps Log
{

	if (Log->size() > 100)
	{
		Log->erase(Log->begin());
		Log->push_back(toPush);
	}
	else Log->push_back(toPush);


}
