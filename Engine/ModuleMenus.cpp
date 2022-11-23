#include "Glew/include/glew.h"

#include "Primitive.h"
#include "Application.h"
#include "ModuleMenus.h"

//#include "ModuleMenuGameObject.h"

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

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

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

	// --------------------------------------------------------------------------- GENERAL WINDOW (CONFIG, ABOUT, CONSOLE, EXIT)
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);

	//ImGui::Begin("GinuhEngine", 0, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				App->files_manager->LoadFile("Assets/Primitives/cube.fbx");
				
				
			}
			if (ImGui::MenuItem("Plane"))
			{
				App->files_manager->LoadFile("Assets/Primitives/plane.fbx");
				
				
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				App->files_manager->LoadFile("Assets/Primitives/cylinder.fbx");
				
				
			}
			if (ImGui::MenuItem("Cone"))
			{
				App->files_manager->LoadFile("Assets/Primitives/cone.fbx");
				
			}
			if (ImGui::MenuItem("Sphere"))
			{
				App->files_manager->LoadFile("Assets/Primitives/sphere.fbx");
				
				
			}	
			if (ImGui::MenuItem("Torus"))
			{
				App->files_manager->LoadFile("Assets/Primitives/torus.fbx");
				
			}	
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
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

			if (ImGui::MenuItem("Hierarchy"))
			{
				pOpen_hierarchy = true; // Window can close

				hierarchyVisible = !hierarchyVisible;
			}

			if (ImGui::MenuItem("Inspector"))
			{
				pOpen_inspector = true; // Window can close

				inspectorVisible = !inspectorVisible;
			}


			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Exit"))
		{
			return UPDATE_STOP;
		}

		ImGui::EndMainMenuBar();

	}

	ImGui::PopStyleVar(3);

	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	if (configVisible) MenuConfig();
	if (aboutVisible) MenuAbout();
	if (consoleVisible) MenuConsole();
	if (hierarchyVisible) MenuHierarchy();
	if (inspectorVisible) MenuInspector();

	// --------------------------------------------------------------------------- WINDOW SCENE
	ImGui::Begin("Scene", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::BeginChild("##ID", ImVec2(SDL_GetWindowSurface(App->window->window)->w, SDL_GetWindowSurface(App->window->window)->h));
	
	ImVec2 wsize = ImGui::GetWindowSize();

	ImGui::Image((ImTextureID)App->renderer3D->textColorBuff, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::EndChild();
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleMenus::CleanUp()
{
	//LOG("Destroying ModuleMenus");
	App->menus->info.AddConsoleLog( "Destroying Module Menus");

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

			//---------------------------------------- ASSIMP
			ImGui::Bullet();
			if (ImGui::Button("Assimp"))
			{
				OpenLink("https://github.com/assimp/assimp");
			}
			ImGui::SameLine();
			ImGui::Text("v. 3.1.1");

			//---------------------------------------- DEVIL
			ImGui::Bullet();
			if (ImGui::Button("DevIL"))
			{
				OpenLink("https://github.com/DentonW/DevIL");
			}
			ImGui::SameLine();
			ImGui::Text("v. 1.8.0");

			//---------------------------------------- PHYSFS
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
			//ImGui::SetWindowSize(ImVec2(450.0f, 300.0f));
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
						SDL_SetWindowSize(App->window->window, 1920, 1080);
						App->menus->info.AddConsoleLog( "FullScreen Enabled");

					}
					else
					{
						
						SDL_SetWindowFullscreen(App->window->window, 0); //FULLSCREEN DISABLED
						SDL_SetWindowSize(App->window->window, SCREEN_WIDTH, SCREEN_HEIGHT);
						App->menus->info.AddConsoleLog("FullScreen Disabled");

					}
				}
				if (!fullscreen) //RESIZABLE, BORDER AND SIZE OPTIONS ONLY APPEARS IF IT IS IN WINDOW MODE
				{
					if (ImGui::Checkbox("Resizable", &resizable))
					{
						if (resizable)
						{
							SDL_SetWindowResizable(App->window->window, SDL_TRUE); //RESIZABLE ENABLED
							App->menus->info.AddConsoleLog("Resizable Mode Enabled");

						}
						else
						{
							SDL_SetWindowResizable(App->window->window, SDL_FALSE);//RESIZABLE DISABLED
							App->menus->info.AddConsoleLog("Resizable Mode Disabled");

						}
					}
					ImGui::SameLine();
					if (ImGui::Checkbox("Borderless", &borderless))
					{
						if (borderless)
						{
							SDL_SetWindowBordered(App->window->window, SDL_FALSE); //BORDERLESS ENABLED
							App->menus->info.AddConsoleLog("Borderless Mode Enabled");

						}
						else
						{
							SDL_SetWindowBordered(App->window->window, SDL_TRUE); //BORDERLESS DISABLED
							App->menus->info.AddConsoleLog("Borderless Mode Disabled");

						}
					}

					ImGui::Text("Width: ");
					ImGui::SameLine();
					ImGui::TextColored({ 0,255,0,255 }, "%d", SDL_GetWindowSurface(App->window->window)->w);

					ImGui::Text("Height: ");
					ImGui::SameLine();
					ImGui::TextColored({ 0,255,0,255 }, "%d", SDL_GetWindowSurface(App->window->window)->h);

				}
			}
			if (ImGui::CollapsingHeader("Visual"))
			{
				if (ImGui::Checkbox("Vsync", &vsync))
				{
					if (vsync)
					{
						SDL_GL_SetSwapInterval(1); //VSYNC ENABLED
						App->menus->info.AddConsoleLog("Vsync Enabled");

					}
					else
					{
						SDL_GL_SetSwapInterval(0); //VSYNC DISABLED
						App->menus->info.AddConsoleLog("Vsync Disabled");

					}
				}

				if (ImGui::SliderFloat("Brightness", &screenBrightness, 0.300f, 1.000f))
				{
					SDL_SetWindowBrightness(App->window->window, screenBrightness);
					App->menus->info.AddConsoleLog("Brightness Change");

				}

				if (ImGui::Checkbox("Lights", &lights))
				{

					if (lights)
					{
						glEnable(GL_LIGHTING); //LIGHTS ENABLED
						App->menus->info.AddConsoleLog("Lights Enabled");

					}
					else
					{
						glDisable(GL_LIGHTING); //LIGHTS DISABLED
						App->menus->info.AddConsoleLog("Lights Disabled");
					}

				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Depth Test", &depthTest))
				{
					if (depthTest)
					{
						glEnable(GL_DEPTH_TEST); //DEPTH TEST ENABLED
						App->menus->info.AddConsoleLog("Depth Test Enabled");

					}
					else
					{
						glDisable(GL_DEPTH_TEST); //DEPTH TEST DISABLED
						App->menus->info.AddConsoleLog("Depth Test Disabled");

					}
				}
				if (ImGui::Checkbox("Cull Face", &cullFace))
				{
					if (cullFace)
					{
						glEnable(GL_CULL_FACE); //CULL FACE ENABLED
						App->menus->info.AddConsoleLog("Cull Face Enabled");

					}
					else
					{
						glDisable(GL_CULL_FACE); //CULL FACE  DISABLED
						App->menus->info.AddConsoleLog("Cull Face Disabled");

					}
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Color Material", &colorMaterial))
				{
					if (colorMaterial)
					{
						glEnable(GL_COLOR_MATERIAL); //COLOR MATERIAL ENABLED
						App->menus->info.AddConsoleLog("Color Material Enabled");

					}
					else
					{
						glDisable(GL_COLOR_MATERIAL); //COLOR MATERIAL  DISABLED
						App->menus->info.AddConsoleLog("Color Material Disabled");

					}
				}
				ImGui::SameLine();
				if (ImGui::Checkbox("Texture 2D", &texture2D))
				{
					if (texture2D)
					{
						App->scene_intro->textureEnabled = true;
						//glEnable(GL_TEXTURE_2D); //TEXTURE 2D ENABLED
						App->menus->info.AddConsoleLog("Texture 2D Enabled");

					}
					else
					{
						App->scene_intro->textureEnabled = false;
						//glDisable(GL_TEXTURE_2D); //TEXTURE 2D DISABLED
						App->menus->info.AddConsoleLog("Texture 2D Disabled");

					}
				}
				if (ImGui::Checkbox("Wireframe View", &wireframeView))
				{
					if (wireframeView)
					{
						App->scene_intro->wireframe = true; //WIREFRAME VIEW ENABLED
						App->menus->info.AddConsoleLog("Wireframe View Enabled");

					}
					else
					{
						App->scene_intro->wireframe = false; //WIREFRAME VIEW DISABLED
						App->menus->info.AddConsoleLog("Wireframe View Disabled");

					}

					// Updated in "ModuleSceneIntro -> Update()"
				}

			}
			if (ImGui::CollapsingHeader("Hardware"))
			{

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
		info.DrawConsole("Console", &pOpen_console);
	}
	if (pOpen_console == NULL)
	{
		consoleVisible = !consoleVisible; // Window is closed so function "MenuConsole()" stops being called
	}
}


void ModuleMenus::MenuHierarchy()
{
	if (pOpen_hierarchy)
	{
		if (ImGui::Begin("Hierarchy", &pOpen_hierarchy, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("GameObjects: \n");

			if (ImGui::Button("Create Empty Game Object to Scene"))
				GameObject* newGO = new GameObject(App->scene_intro->gameObjects[0], "Empty");

			// We manage the "GameObject[0]" as the scene, so all others GameObjects are its children
			// We "only" print the Root, and it "automatically" print its children
			PrintGameObjects(App->scene_intro->gameObjects[0]);
		}
		ImGui::End();
	}
	if (pOpen_hierarchy == NULL) hierarchyVisible = !hierarchyVisible; // Window is closed so function "MenuHierarchy()" stops being called

}

void ModuleMenus::MenuInspector()
{
	if (pOpen_inspector)
	{
		ImGui::Begin("Inspector", &pOpen_inspector, ImGuiWindowFlags_AlwaysAutoResize);
		if (App->scene_intro->gameobject_selected != NULL)
		{
			strcpy(newName, App->scene_intro->gameobject_selected->name.c_str());
			if (ImGui::InputText("##Name", &newName[0], sizeof(newName)))
			{
				if (newName[0] != '\0')
					App->scene_intro->gameobject_selected->name = newName;
			}
			ImGui::SameLine();

			ImGui::Checkbox("Active", &App->scene_intro->gameobject_selected->active);

			for (size_t i = 0; i < App->scene_intro->gameobject_selected->GetComponents().size(); i++)
			{
				App->scene_intro->gameobject_selected->GetComponentByNum(i)->PrintGui();
			}
		}

		ImGui::End();
	}
	if (pOpen_inspector == NULL) inspectorVisible = !inspectorVisible; // Window is closed so function "MenuInspector()" stops being called
}

void ModuleMenus::PrintGameObjects(GameObject* go)
{
	ImGuiTreeNodeFlags TreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (go == App->scene_intro->gameobject_selected)
	{
		TreeFlags |= ImGuiTreeNodeFlags_Selected;

	}

	if (go->GetChildren().empty())
	{
		//BeginDrag(App->scene_intro->gameobject_selected);
		//BeginDrop(go);

		TreeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::TreeNodeEx(go->name.c_str(), TreeFlags);

		OptionsPanelGO(go);

		if (ImGui::IsItemHovered() && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN))
		{
			App->scene_intro->gameobject_selected = go;
		}
	}
	else
	{

		if (ImGui::TreeNodeEx(go->name.c_str(), TreeFlags))
		{
			//BeginDrag(App->scene_intro->gameobject_selected);
			BeginDrop(go);

			OptionsPanelGO(go);

			if (ImGui::IsItemHovered() && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN))
			{
				App->scene_intro->gameobject_selected = go;

			}
			for (size_t i = 0; i < go->GetChildren().size(); i++)
			{
				PrintGameObjects(go->GetChild(i));
			}
			ImGui::TreePop();
		}
	}

	BeginDrag(App->scene_intro->gameobject_selected);
	BeginDrop(go);
}

void ModuleMenus::BeginDrag(GameObject* GO)
{
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("GAME_OBJECT", GO, sizeof(GameObject*));

		relocatedGO = GO;

		ImGui::Text("Changing Hierarchy...");

		ImGui::EndDragDropSource();
	}
}

void ModuleMenus::BeginDrop(GameObject* GO)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAME_OBJECT"))
		{
			GameObject* dropGO = (GameObject*)payload->Data;

			relocatedGO->RelocateGO(GO);

			relocatedGO = nullptr;
		}

		ImGui::EndDragDropTarget();
	}
}

void ModuleMenus::OptionsPanelGO(GameObject* GO)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (App->scene_intro->gameObjects[0] != GO)
		{
			ImGui::Text("Edit OPTIONS for \"%s\"", App->scene_intro->gameobject_selected->name.c_str());
			if (ImGui::Button("Create Empty GO"))
				GameObject* newGO = new GameObject(App->scene_intro->gameobject_selected, "Empty");

			if (ImGui::Button("Delete GO"))
				App->scene_intro->gameobject_selected->~GameObject(); 

			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal) && App->scene_intro->gameObjects[0] != GO)
		ImGui::SetTooltip("Right-click to edit Game Object");
}

void ModuleMenus::OpenLink(const char* url)
{
	ShellExecuteA(0, "open", url, 0, 0, 0);

}

void ModuleMenus::PushLog(vector<float>* Log, float toPush) //Function to keep actualizing the ms and fps Log
{

	if (Log->size() > 100)
	{
		Log->erase(Log->begin());
		Log->push_back(toPush);
	}
	else Log->push_back(toPush);


}

