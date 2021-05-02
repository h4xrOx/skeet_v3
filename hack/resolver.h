#pragma once

class ShotRecord;

struct resolverInfo_t {
	resolverInfo_t() = default;

	struct missInfo_t {
		missInfo_t() {
			this->m_freestand = 0;
			this->m_first_shot = 0;
			this->m_static = 0;
			this->m_jitter = 0;
		}

		int m_freestand;
		int m_first_shot;
		int m_static;
		int m_jitter;
	};

	struct hitInfo_t {
		hitInfo_t() {
			this->m_freestand = 0;
			this->m_first_shot = 0;
			this->m_static = 0;
			this->m_jitter = 0;
		}

		int m_freestand;
		int m_first_shot;
		int m_static;
		int m_jitter;
	};

	bool m_jitter{};
	bool m_use_freestand{};
	
	missInfo_t m_misses{};
	hitInfo_t m_hits{};
};

class Resolver {
public:
	enum Modes : size_t {
		RESOLVE_NONE = 0,
		RESOLVE_MOVE,
		RESOLVE_STAND,
	};

public:

	void ResolveAngles( Player* player, LagComp::LagRecord_t* record);
	void ResolveStand( AimPlayer* data, LagComp::LagRecord_t* record );

	float ResolveShot( AimPlayer* data, LagComp::LagRecord_t* record );

public:
	std::array< vec3_t, 64 > m_impacts;
	//std::unordered_map<CBaseHandle, resolverInfo_t> m_resolver_info;
};

extern Resolver g_resolver;