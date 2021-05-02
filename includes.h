#pragma once

#pragma warning( disable : 4307 ) // '*': integral constant overflow
#pragma warning( disable : 4244 ) // possible loss of data
#pragma warning( disable : 4800 ) // forcing value to bool 'true' or 'false'
#pragma warning( disable : 4838 ) // conversion from '::size_t' to 'int' requires a narrowing conversion

// You can define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING or _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS to acknowledge that you have received this warning.
#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

using ulong_t = unsigned long;


// windows / stl includes.
#include <Windows.h>
#include <cstdint>
#include <intrin.h>
#include <xmmintrin.h>
#include <array>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <functional>
#include <map>
#include <shlobj.h>
#include <filesystem>
#include <streambuf>
#include <optional>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Freetype.lib")
#pragma comment(lib, "Urlmon.lib")

#include <playsoundapi.h>
#include <mmeapi.h> 
#pragma comment(lib, "Winmm.lib") 

// our custom wrapper.
#include "util/unique_vector.h"
#include "util/contrib/tinyformat.h"

// other includes.
#include "util/hash.h"
#include "util/xorstr.h"
#include "util/pe.h"
#include "util/winapir.h"
#include "util/address.h"
#include "util/modules.h"
#include "util/pattern.h"
#include "util/util.h"
#include "util/vmt.h"
#include "util/stack.h"
#include "util/nt.h"
#include "util/x86.h"
#include "util/syscall.h"
#include "util/detours.h"

// hack includes.
#include "sdk/interfaces.h"
#include "sdk/sdk.h"
#include "core/csgo.h"
#include "hack/penetration.h"
#include "sdk/netvars.h"
#include "sdk/entoffsets.h"
#include "hack/hvh.h"
#include "sdk/entity.h"
#include "core/client.h"
#include "sdk/gamerules.h"
#include "core/hooks/hooks.h"
#include "core/hooks/detours.h"
#include "rendering/render.h"
#include "hack/pred.h"
#include "hack/lagcomp.h"
#include "hack/visuals.h"
#include "hack/movement.h"
#include "hack/game_movement.h"
#include "hack/aimbot.h"
#include "hack/animations.h"
#include "hack/tickbase_shift.h"
#include "hack/netdata.h"
#include "hack/chams.h"
#include "notify.h"
#include "hack/resolver.h"
#include "hack/grenades.h"
#include "hack/skins.h"
#include "hack/events.h"
#include "hack/hitsounds.h"
#include "hack/shots.h"

// gui includes.
#include "util/contrib/base64.h"
#include "inputsystem.h"
#include "config.h"

#include "Rendering/Renderer.h"

#include "gui/Helpers/Easing.h"
#include "gui/Framework/GUI.h"
#include "gui/Elements.h"