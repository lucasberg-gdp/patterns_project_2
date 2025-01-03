#pragma once

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>


#include <vector>
#include <string>

class WindowsUtils
{
public:
	static bool GetAllFilesFromFolder(std::string path, std::vector< std::string>& fileNames);
	static bool GetAllFoldersFromFolder(std::string path, std::vector< std::string>& fileNames);
	static bool LoadPirateAssets(std::vector< std::string>& fileNames);
};

