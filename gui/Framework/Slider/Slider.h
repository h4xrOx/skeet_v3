#pragma once

namespace GUI::Controls {
	bool Slider( const std::string& name, const std::string& var_name, float min, float max, const std::string& display = "%.1f" );
	//int Slider( const std::string &name, int *var_name, int min, int max, const std::string &display = "%.0f" );
}
