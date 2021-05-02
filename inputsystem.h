#pragma once

class Input {
public:
	struct key_t {
		bool down;
		bool pressed;
		int  tick;
		int  oldtick;
	};

	std::array< key_t, 256 > m_keys;
	vec2_t					 m_mouse;
	vec2_t					 m_delta;
	int m_scroll;
	std::string				 m_buffer;

public:

	__forceinline void updateMouse() {
		POINT p;
		if (!GetCursorPos(&p))
			return;

		ScreenToClient(g_csgo.m_window, &p);

		m_delta = m_mouse - vec2_t(
			static_cast<float>(p.x),
			static_cast<float>(p.y));

		m_mouse = {
			static_cast<float>(p.x),
			static_cast<float>(p.y) };
	}

	__forceinline void update() {
		// iterate all keys.
		for (int i{}; i <= 254; ++i) {
			key_t* key = &m_keys[i];

			key->pressed = false;

			if (key->down && key->tick > key->oldtick) {
				key->oldtick = key->tick;
				key->pressed = true;
			}
		}
	}

	// mouse within coords.
	__forceinline bool IsCursorInBounds(int x, int y, int x2, int y2) const {
		return m_mouse.x > x && m_mouse.y > y && m_mouse.x < x2&& m_mouse.y < y2;
	}

	// mouse within rectangle.
	__forceinline bool IsCursorInRect(Rect area) const {
		return IsCursorInBounds(area.x, area.y, area.x + area.w, area.y + area.h);
	}

	__forceinline void SetDown(int vk) {
		key_t* key = &m_keys[vk];

		key->down = true;
		key->tick = g_winapi.GetTickCount();
	}

	__forceinline void SetUp(int vk) {
		key_t* key = &m_keys[vk];
		key->down = false;
	}

	// key is being held.
	__forceinline bool GetKeyState(int vk) {
		if (vk == -1)
			return false;

		return m_keys[vk].down;
	}

	// key was pressed.
	__forceinline bool GetKeyPress(int vk) {
		if (vk == -1)
			return false;

		key_t* key = &m_keys[vk];
		return key->pressed;
	}
};

extern Input g_input;