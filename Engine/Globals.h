#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <string>
#include <filesystem>

#include "imgui/imgui.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

//void    ConsoleInfo::AddConsoleLog(const char file[], int line, const char* logText, ...);
class ConsoleInfo;

#define LOGC(logText, ...) info.AddConsoleLog(__FILE__, __LINE__, logText, __VA_ARGS__);

struct ConsoleInfo
{
public:
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets;        // Index to lines offset
	bool                ScrollToBottom;

	void    Clear() { Buf.clear(); LineOffsets.clear(); }

	void AddConsoleLog(const char file[], int line, const char* logText, ...)
	{
		static char tmp_string[4096];
		static char tmp_string2[4096];
		static va_list  ap;

		// Construct the string from variable arguments
		va_start(ap, logText);
		vsprintf_s(tmp_string, 4096, logText, ap);
		va_end(ap);
		sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
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

static ConsoleInfo info;

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "GINUH ENGINE"