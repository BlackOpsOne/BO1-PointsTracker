#include <string>

namespace settings {
	void LoadSettings();
	void SaveSettings(bool _default);
	std::string GetValue(std::string setting);
}