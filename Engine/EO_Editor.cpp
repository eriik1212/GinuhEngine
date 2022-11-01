#include "EO_Editor.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "EW_Hierarchy.h"
#include "EW_Game.h"
#include "EW_Scene.h"
#include "EW_About.h"
#include "EW_Inspector.h"
#include "EW_Performance.h"
#include "EW_Project.h"
#include "EW_Console.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "IconsFontAwesome.h"

#include "ModuleFile.h"

EO_Editor::EO_Editor()
{
}

EO_Editor::~EO_Editor()
{
}

void EO_Editor::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		style.Colors[ImGuiCol_Border] = ImVec4(255, 1, 1, 255);
	}

	io.IniFilename = "Config/settings.ini";

	// Setup ImGui style
	

	// Setup font
	io.Fonts->AddFontDefault();
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 10.0f, &icons_config, icons_ranges);

	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create Editor Windows
	editor_windows[(uint)EW_TYPE::HIERARCHY] = new EW_Hierarchy();
	editor_windows[(uint)EW_TYPE::ABOUT] = new EW_About();
	editor_windows[(uint)EW_TYPE::GAME] = new EW_Game();
	editor_windows[(uint)EW_TYPE::SCENE] = new EW_Scene();
	editor_windows[(uint)EW_TYPE::PERFORMANCE] = new EW_Performance();
	editor_windows[(uint)EW_TYPE::INSPECTOR] = new EW_Inspector();
	editor_windows[(uint)EW_TYPE::CONSOLE] = new EW_Console();
	editor_windows[(uint)EW_TYPE::PROJECT] = new EW_Project();
}

void EO_Editor::PreUpdate()
{}

void EO_Editor::Update()
{}


void EO_Editor::PostUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window->window);
	ImGui::NewFrame();

	if (!SetMenuBar())
	{
		app->renderer3D->exit = true;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EO_Editor::CleanUp()
{
	for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
	{
		RELEASE(editor_windows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

bool EO_Editor::SetMenuBar()
{
	bool exit = false;

	// Top Bar
	ImGui::Begin("Yokai_Engine", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}
			if (ImGui::MenuItem("Open"))
			{

			}
			if (ImGui::MenuItem("Save"))
			{

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Wireframe", &app->renderer3D->wireframe);

			if (ImGui::Checkbox("Depth Test", &app->renderer3D->depth_test))
				(app->renderer3D->depth_test) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

			if (ImGui::Checkbox("Cull Face", &app->renderer3D->cull_face))
				(app->renderer3D->cull_face) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

			if (ImGui::Checkbox("Lighting", &app->renderer3D->lighting))
				(app->renderer3D->lighting) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

			if (ImGui::Checkbox("Color Material", &app->renderer3D->color_material))
				(app->renderer3D->color_material) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

			if (ImGui::Checkbox("Texture 2D", &app->renderer3D->texture_2d))
				(app->renderer3D->texture_2d) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
			{
				if (i != (uint)EW_TYPE::ABOUT) // about goes in help
				{
					ImGui::Checkbox(editor_windows[i]->window_name.c_str(), &editor_windows[i]->enabled);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::Checkbox("Vsync", &app->renderer3D->vsync);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Checkbox(editor_windows[(uint)EW_TYPE::ABOUT]->window_name.c_str(), &editor_windows[(uint)EW_TYPE::ABOUT]->enabled);
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Exit"))
		{
			exit = true; // close window
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

	// update enabled windows
	for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
	{
		if (editor_windows[i]->enabled)
		{
			editor_windows[i]->Update();
		}
		else if (i == (uint)EW_TYPE::SCENE)
		{
			dynamic_cast<EW_Scene*>(editor_windows[(uint)EW_TYPE::SCENE])->SetSceneCamera(false);
		}
		else if (i == (uint)EW_TYPE::GAME)
		{
			dynamic_cast<EW_Game*>(editor_windows[(uint)EW_TYPE::GAME])->SetCurrentGameCamera(false);
		}
	}

	return !exit;
}