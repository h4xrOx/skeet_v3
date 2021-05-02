#pragma once
#include "../../includes.h"

namespace InputHelper {
	inline bool KeyState[ 256 ];
	inline bool PrevKeyState[ 256 ];

	inline int Scroll;

	void Update( );
	bool Pressed( int key );
	bool Down( int key );
	bool Released( int key );
	bool Hovered( const vec2_t &pos, const vec2_t&size );
}