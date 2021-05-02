#pragma once

class Sphere;

enum HitscanMode : int {
	NORMAL = 0,
	LETHAL = 1,
	LETHAL2 = 3,
	PREFER = 4,
	PREFER_SAFEPOINT = 5,
};

struct AnimationBackup_t {
	vec3_t           m_origin, m_abs_origin;
	vec3_t           m_velocity, m_abs_velocity;
	int              m_flags, m_eflags;
	float            m_duck, m_body;
	C_AnimationLayer m_layers[ 13 ];
};

struct HitscanData_t {
	float  m_damage;
	vec3_t m_pos;
	int m_hitbox;
	bool m_safepoint;
	int m_mode;

	__forceinline HitscanData_t() : m_damage{ 0.f }, m_pos{ }, m_hitbox{ }, m_safepoint{}, m_mode{} {}
};

struct HitscanBox_t {
	int         m_index;
	HitscanMode m_mode;
	bool m_safepoint;

	__forceinline bool operator==( const HitscanBox_t& c ) const {
		return m_index == c.m_index && m_mode == c.m_mode && m_safepoint == c.m_safepoint;
	}
};

class AimPlayer {
public:
	using hitboxcan_t = stdpp::unique_vector< HitscanBox_t >;

public:
	// essential data.
	Player* m_player;
	float	  m_spawn;

	// aimbot data.
	hitboxcan_t m_hitboxes;

	// resolve data.
	int       m_shots;
	int       m_missed_shots;
	float     m_delta;
	float	  m_last_resolve;
	bool      m_extending;

	float m_abs_angles;

	BoneArray* m_matrix;

public:

	void OnNetUpdate( Player* player );
	void OnRoundStart( Player* player );
	void SetupHitboxes( LagComp::LagRecord_t* record, bool history );
	bool SetupHitboxPoints( LagComp::LagRecord_t* record, BoneArray* bones, int index, std::vector< vec3_t >& points );
	bool GetBestAimPosition( vec3_t& aim, float& damage, int& hitbox, LagComp::LagRecord_t* record, float& min_damage );

public:
	void reset( ) {
		m_player = nullptr;
		m_spawn = 0.f;
		m_shots = 0;
		m_missed_shots = 0;

		m_hitboxes.clear( );
	}
};

class Aimbot {
private:
	struct target_t {
		Player* m_player;
		AimPlayer* m_data;
	};

	struct knife_target_t {
		target_t  m_target;
	};

	struct table_t {
		uint8_t swing[ 2 ][ 2 ][ 2 ]; // [ first ][ armor ][ back ]
		uint8_t stab[ 2 ][ 2 ];		  // [ armor ][ back ]
	};

	const table_t m_knife_dmg{ { { { 25, 90 }, { 21, 76 } }, { { 40, 90 }, { 34, 76 } } }, { { 65, 180 }, { 55, 153 } } };

	std::array< ang_t, 12 > m_knife_ang{
		ang_t{ 0.f, 0.f, 0.f }, ang_t{ 0.f, -90.f, 0.f }, ang_t{ 0.f, 90.f, 0.f }, ang_t{ 0.f, 180.f, 0.f },
		ang_t{ -80.f, 0.f, 0.f }, ang_t{ -80.f, -90.f, 0.f }, ang_t{ -80.f, 90.f, 0.f }, ang_t{ -80.f, 180.f, 0.f },
		ang_t{ 80.f, 0.f, 0.f }, ang_t{ 80.f, -90.f, 0.f }, ang_t{ 80.f, 90.f, 0.f }, ang_t{ 80.f, 180.f, 0.f }
	};

public:
	std::array< AimPlayer, 64 > m_players;
	std::vector< AimPlayer* >   m_targets;

	AimPlayer* m_old_target;

	// target selection stuff.
	float m_best_dist;
	float m_best_fov;
	float m_best_damage;
	int   m_best_hp;
	float m_best_lag;
	float m_best_height;

	// found target stuff.
	Player* m_target;
	ang_t      m_angle;
	vec3_t     m_aim;
	float      m_damage;
	LagComp::LagRecord_t* m_record;

	int m_hitbox;
	bool m_stop;
	bool m_override_damage;
	bool m_force_body;
	bool m_shoot_next_tick;
	bool m_force_safepoint;
	BoneArray* m_current_matrix;

	std::vector<Sphere> m_current_sphere;
public:
	__forceinline void reset( ) {
		// reset aimbot data.
		init( );

		// reset all players data.
		for( auto& p : m_players )
			p.reset( );
	}

	__forceinline bool IsValidTarget( Player* player ) {
		if( !player )
			return false;

		if( !player->IsPlayer( ) )
			return false;

		if( !player->alive( ) )
			return false;

		if( player->m_bIsLocalPlayer( ) )
			return false;

		if( !player->enemy( g_cl.m_local ) )
			return false;

		return true;
	}

	bool CanHit(vec3_t start, vec3_t end, LagComp::LagRecord_t* record, int box, bool in_shot = false, BoneArray* bones = nullptr);

public:
	// aimbot.
	void init( );
	void StripAttack( );
	void think( );
	void find( );
	bool CheckHitchance( Player* player, const ang_t& angle, LagComp::LagRecord_t* record, int hitbox );
	bool SelectTarget( LagComp::LagRecord_t* record, const vec3_t& aim, float damage );
	void apply( );
	void NoSpread( );

	// knifebot.
	void knife( );
	bool CanKnife( LagComp::LagRecord_t* record, ang_t angle, bool& stab );
	bool KnifeTrace( vec3_t dir, bool stab, CGameTrace* trace );
	bool KnifeIsBehind( LagComp::LagRecord_t* record );
};

extern Aimbot g_aimbot;