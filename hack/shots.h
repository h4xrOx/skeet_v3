#pragma once

class ShotRecord {
public:
	__forceinline ShotRecord() : m_target{}, m_record{}, m_time{}, m_lat{}, m_damage{}, m_pos{}, m_impacted{}, m_hurt{}, m_confirmed{}, m_hitbox{}, m_matrix{} {}

public:
	Player*    m_target;
	LagComp::LagRecord_t* m_record;
	float      m_time, m_lat, m_damage;
	vec3_t     m_pos;
	vec3_t	   m_server_pos;
	int		   m_hitbox;
	bool       m_impacted;
	bool	   m_hurt;
	bool	   m_confirmed;
	float	   m_weapon_range;
	BoneArray*  m_matrix;
};

class VisualImpactData_t {
public:
    vec3_t m_impact_pos, m_shoot_pos;
    int    m_tickbase;
    bool   m_ignore, m_hit_player;

public:
    __forceinline VisualImpactData_t( const vec3_t &impact_pos, const vec3_t &shoot_pos, int tickbase ) : 
        m_impact_pos{ impact_pos }, m_shoot_pos{ shoot_pos }, m_tickbase{ tickbase }, m_ignore{ false }, m_hit_player{ false } {}
};

class ImpactRecord {
public:
	__forceinline ImpactRecord() : m_shot{}, m_pos{}, m_tick{}, m_hit{} {}

public:
	ShotRecord* m_shot;
	int         m_tick;
	vec3_t      m_pos;
	bool		m_hit;
};

class HitRecord {
public:
	__forceinline HitRecord( ) : m_impact{}, m_group{ -1 }, m_damage{} {}

public:
	ImpactRecord* m_impact;
	int           m_group;
	float         m_damage;
};

class Sphere
{
public:
	Sphere(void) {};
	Sphere(const vec3_t& vecCenter, float flRadius, int hitgroup) { m_center = vecCenter; m_radius = flRadius; m_hitgroup = hitgroup; };

public:
	bool intersectsRay(const Ray& ray);
	bool intersectsRay(const Ray& ray, vec3_t& vecIntersection);
	bool IntersectInfiniteRayWithSphere(const vec3_t& vecRayOrigin, const vec3_t& vecRayDelta, const vec3_t& vecSphereCenter, float flRadius, float* pT1, float* pT2);
public:
	vec3_t m_center;
	float m_radius = 0.f;
	int m_hitgroup;
};

class Shots {
public:
	void OnShotFire( Player* target, float damage, int bullets, LagComp::LagRecord_t* record, int hitbox );
	void OnImpact( IGameEvent* evt );
	void OnHurt( IGameEvent* evt );
	void OnFire(IGameEvent* evt);
	void OnFrameStage();

	void SetupCapsule(const vec3_t& vecMin, const vec3_t& vecMax, float flRadius, int hitgroup, std::vector<Sphere>& m_cSpheres);
public:
	std::array< std::string, 11 > m_groups = {
	   XOR("body"),
	   XOR("head"),
	   XOR("chest"),
	   XOR("stomach"),
	   XOR("left arm"),
	   XOR("right arm"),
	   XOR("left leg"),
	   XOR("right leg"),
	   XOR("neck"),
	   XOR("unknown"),
	   XOR("gear")
	};

	std::deque< ShotRecord >          m_shots;
    std::vector< VisualImpactData_t > m_vis_impacts;
	std::deque< ImpactRecord >        m_impacts;
	std::deque< HitRecord >           m_hits;
};

extern Shots g_shots;