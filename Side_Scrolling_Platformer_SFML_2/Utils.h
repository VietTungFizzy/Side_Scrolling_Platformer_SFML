#pragma once
#define RUNNING_WINDOW
#include<iostream>
#include<string>

namespace Utils
{ 
#ifdef RUNNING_WINDOW
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<Shlwapi.h>
	
	inline std::string GetWorkingDirection()
	{
		HMODULE _module = GetModuleHandleA(nullptr);
		if (_module)
		{
			char path[MAX_PATH];
			GetModuleFileName(_module, path, MAX_PATH);
			PathRemoveFileSpecA(path);
			strcat_s(path, "\\");
			return std::string(path);
		}
		else
		{
			return "";
		}
	}
#elif defined RUNNING_LINUX
	#include<unistd.h>
	inline std::string GetWorkingDirection()
	{
		char path[1024];
		if (getcwd(path, sizeof(path)) != nullptr)
		{
			return std::string(cwd) + std::string("/");
		}
		return "";
	}
#endif // RUNNING_WINDOW
}