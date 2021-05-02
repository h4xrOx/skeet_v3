#pragma once

namespace GUI::Form {
	bool BeginWindow( std::string name );
	void EndWindow( );

	bool BeginTab( std::string icon, std::string name );
	bool BeginSubTab( std::string icon, std::string name );
}
