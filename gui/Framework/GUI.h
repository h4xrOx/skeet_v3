#pragma once
#include <stack>

#include "../Helpers/InputHelper.h"
#include "../../includes.h"

/* Prototypes */
struct MultiItem_t {
	std::string name;
	std::string value;
};

/* Context structs */
struct DropdownInfo_t {
	size_t HashedID;
	bool Open = false;
	bool OutAnimation = false;

	std::map<size_t, float> ArrowAnimation;
	std::map<size_t, float> ArrowTimer;

	std::map<size_t, float> CrossAnimation;
	std::map<size_t, float> CrossTimer;

	float Animation;

	std::vector<std::string> Elements;
	std::string Option;

	float Size = 0.f;
	vec2_t Pos = { 0, 0 };
};

struct MultiDropdownInfo_t {
	size_t HashedID;
	bool Open = false;
	bool OutAnimation = false;

	float Animation;

	std::map<size_t, float> _ArrowAnimation;
	std::map<size_t, float> _ArrowTimer;

	std::map<size_t, float> _CrossAnimation;
	std::map<size_t, float> _CrossTimer;

	std::vector<MultiItem_t> Elements;
	std::string Option;

	float Size = 0.f;
	vec2_t Pos = { 0, 0 };
};

struct HotkeyInfo_t {
	size_t id;
	bool open = false;

	vec2_t Pos = { 0, 0 };
};

struct ColorPickerInfo_t {
	size_t HashedID;
	bool Open = false;
	bool OutAnimation = false;

	float Animation;

	std::string Value;
	Color DefaultColor{ 255, 255, 255 };

	bool PickingAlpha = false;
	bool PickingColor = false;
	bool PickingHue = false;

	float Size = 0.f;
	vec2_t Pos = { 0, 0 };
};

struct SliderInfo_t {
	std::map<size_t, float> ValueAnimation;
	std::map<size_t, float> ValueTimer;

	std::map<size_t, float> PreviewAnimation;
	std::map<size_t, float> PreviousAmount;

	std::map<size_t, float> LastChangeTime;

	std::map<size_t, std::pair<float, bool>> ShouldChangeValue;
};

/* "Object" structs */
struct MenuContext_t {
	vec2_t pos = { 100, 100 };
	vec2_t size = { 600, 445 };

	vec2_t NextGroupPos;
	std::stack< vec2_t > CursorPosStack;

	std::vector< std::pair<std::string, std::string> > Tabs;
	int ActiveTab = 0;

	std::map<int, std::vector< std::pair<std::string, std::string> >> SubTabs;
	std::map<int, int> ActiveSubTab;

	DropdownInfo_t DropdownInfo;
	MultiDropdownInfo_t MultiDropdownInfo;
	ColorPickerInfo_t ColorPickerInfo;
	HotkeyInfo_t HotkeyInfo;
	SliderInfo_t SliderInfo;

	vec2_t ParentPos;
	vec2_t ParentSize;

	std::string parent;
	size_t FocusedID = 0;

	bool open = false;
	bool dragging = false;
	bool setup = false;
	bool typing = false;

	float animation = 0.f;
	float timer = 0.f;
};

namespace GUI {
	inline MenuContext_t* ctx = new MenuContext_t();

	// modify this if you'd like to change the spacing between each object
	inline int ObjectPadding( ) {
		constexpr int spacing{ 10 };

		return spacing + 1;
	};	

	vec2_t PopCursorPos( );
	vec2_t GetLastCursorPos( );
	void PushCursorPos( const vec2_t &cursor_pos );

	size_t Hash( const std::string &name );
	float Approach( float a, float b, float multiplier );
	float MapNumber( float input, float input_min, float input_max, float output_min, float output_max );
	std::vector<std::string> SplitStr( const std::string &str, char separator );
}

#include "form/form.h"
#include "group/group.h"
#include "checkbox/checkbox.h"
#include "button/button.h"
#include "label/label.h"
#include "slider/slider.h"
#include "dropdown/dropdown.h"
#include "colorpicker/colorpicker.h"
#include "hotkey/hotkey.h"
#include "multidropdown/multidropdown.h"
#include "Listbox/Listbox.h"