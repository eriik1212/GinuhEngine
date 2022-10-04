#ifndef __ModuleMenus_H__
#define __ModuleMenus_H__

#include <vector>

#include "Module.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

struct AppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear() { Buf.clear(); LineOffsets.clear(); }

    void    AddLog(const char* logText)
    {
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


        LOG(logText);
    }

    void    Draw(const char* title, bool* p_opened = NULL)
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
		pOpen_console = true;

	bool vsync = VSYNC;

  AppLog my_log;
private:
	bool aboutVisible = false;
	void MenuAbout();

	bool configVisible = false;
	void MenuConfig();

	bool consoleVisible = false;
	void MenuConsole();

	//--------------------------------------------- OPTIONS
	//window
	bool fullscreen = WIN_FULLSCREEN;
	bool resizable = WIN_RESIZABLE;
	bool borderless = WIN_BORDERLESS;
	bool full_desk = WIN_FULLSCREEN_DESKTOP;
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	//visual

	bool lights = true;
	bool depth_test = true;
	bool cull_face = true;
	bool color_material = true;
	bool texture2D = true;
	bool wireframe_view = false;
	float screen_brightness = 1;

	bool hardwareActive = true;

	void OpenLink(const char* url);
	void PushLog(std::vector<float>* Log, float toPush);

	//FPS

	std::vector<float> fpsLog;
	std::vector<float> timeLog;





};

#endif
