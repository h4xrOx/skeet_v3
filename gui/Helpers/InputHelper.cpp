#include "../../includes.h"

#include <Windows.h>

void InputHelper::Update( ) {
	static HWND window;

	if (!window) {
		window = FindWindowA(XOR("Valve001"), 0);
	}

	if (!(window && window == GetForegroundWindow()))
		return;

	for ( int i = 0; i < 256; i++ ) {
		PrevKeyState[ i ] = KeyState[ i ];
		KeyState[ i ] = GetAsyncKeyState( i );
	}
}

bool InputHelper::Down( int key ) {
	return KeyState[ key ] && PrevKeyState[ key ];
}

bool InputHelper::Pressed( int key ) {
	return KeyState[ key ] && !PrevKeyState[ key ];
}

bool InputHelper::Released( int key ) {
	return !KeyState[ key ] && PrevKeyState[ key ];
}

bool InputHelper::Hovered(const vec2_t& pos, const vec2_t& size) {
	return g_input.m_mouse > pos &&
		g_input.m_mouse < pos + size;
}