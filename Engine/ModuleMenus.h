#ifndef __ModuleMenus_H__
#define __ModuleMenus_H__

#include <vector>

#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

class ConsoleInfo
{
public:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

	void    Clear() { Buf.clear(); LineOffsets.clear(); }

	void AddConsoleLog(const char* logText, ...)
	{
		static  char tmp_string[4096];
		static char tmp_string2[4096];
		static va_list  ap;

		// Construct the string from variable arguments
		va_start(ap, logText);
		vsprintf_s(tmp_string, 4096, logText, ap);
		va_end(ap);
		sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", __FILE__, __LINE__, tmp_string);
		OutputDebugString(tmp_string2);

		int old_size = Buf.size();
		va_list args;
		va_start(args, logText);
		Buf.appendfv(logText, args);
		Buf.appendfv("\n", args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
		{

			if (Buf[old_size] == '\n')
			{
				LineOffsets.push_back(old_size);

			}
		}

		ScrollToBottom = true;

	}

	void    DrawConsole(const char* title, bool* p_opened = NULL)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 200));
		ImGui::Begin(title, p_opened);
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);
		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

		if (Filter.IsActive())
		{
			const char* buf_begin = Buf.begin();
			const char* line = buf_begin;
			for (int line_no = 0; line != NULL; line_no++)
			{
				const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
				if (Filter.PassFilter(line, line_end))
					ImGui::TextUnformatted(line, line_end);
				line = line_end && line_end[1] ? line_end + 1 : NULL;
			}
		}
		else
		{
			ImGui::TextUnformatted(Buf.begin());
		}

		if (ScrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::End();
	}
};

class ModuleMenus : public Module
{
public:
	ModuleMenus(Application* app, bool start_enabled = true);
	~ModuleMenus();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool pOpen_config = true,
		pOpen_about = true,
		pOpen_console = true,
		pOpen_hierarchy = true,
		pOpen_inspector = true;


	bool vsync = VSYNC;

	ConsoleInfo info;
    
private:
	bool aboutVisible = true;
	void MenuAbout();

	bool configVisible = true;
	void MenuConfig();

	bool consoleVisible = true;
	void MenuConsole();

	bool hierarchyVisible = true;
	void MenuHierarchy();

	bool inspectorVisible = true;
	void MenuInspector();

	
	


	//--------------------------------------------- OPTIONS
	//window
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool fullDesk = WIN_FULLSCREEN_DESKTOP;
	int screenWidth = SCREEN_WIDTH;
	int screenHeight = SCREEN_HEIGHT;

	//visual

	bool lights = true;
	bool depthTest = true;
	bool cullFace = true;
	bool colorMaterial = true;
	bool texture2D = true;
	bool wireframeView = false;
	float screenBrightness = 1;

	bool hardwareActive = true;


	void OpenLink(const char* url);
	void PushLog(std::vector<float>* Log, float toPush);
	

	//FPS

	std::vector<float> fpsLog;
	std::vector<float> timeLog;

	
	


};

#endif
