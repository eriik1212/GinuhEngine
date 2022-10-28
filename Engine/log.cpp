#pragma once
#include "Globals.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);
}

void ConsoleInfo::AddConsoleLog(const char file[], int line, const char* logText, ...)
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