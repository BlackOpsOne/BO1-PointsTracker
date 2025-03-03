#include "settings.hpp"

#include <filesystem>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

namespace fs = std::filesystem;
std::map<std::string, std::string> setting_values;

namespace settings {
	// saves the settings to the settings file
	void SaveSettings(bool _default)
	{
		std::string path = "points-tracker.ini";
		std::ofstream file(path, std::ios::trunc);

		if (!file.is_open())
		{
			MessageBoxA(NULL, "Error opening \"points-tracker.ini\"", "BO1-PointsTracker", MB_OK | MB_ICONERROR);
			return;
		}

		// if we're saving the default settings, just write to the file
		if (_default)
		{
			file << "DisplayPointsLost=Yes\n";
		}
		else
		{
			for (const auto& [key, value] : setting_values)
			{
				file << key << "=" << value << "\n";
			}
		}

		file.close();
	}

	// loads settings from the settings file
	void LoadSettings()
	{
		std::string path = "points-tracker.ini";

		if (!fs::exists(path))
		{
			SaveSettings(true);
			LoadSettings();
			return;
		}

		std::ifstream file(path);

		if (!file.is_open())
		{
			MessageBoxA(NULL, "Error opening \"points-tracker.ini\"", "BO1-PointsTracker", MB_OK | MB_ICONERROR);
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#')
				continue;

			size_t delimiter = line.find('=');
			if (delimiter != std::string::npos)
			{
				std::string key = line.substr(0, delimiter);
				std::string value = line.substr(delimiter + 1);
				setting_values[key] = value;
			}
		}

		file.close();
	}

	std::string GetValue(std::string setting)
	{
		if (setting_values.find(setting) != setting_values.end())
		{
			return setting_values[setting];
		}
		return "";
	}
}