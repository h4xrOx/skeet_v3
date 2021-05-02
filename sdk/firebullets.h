#pragma once

enum WeaponSound_t {
	EMPTY,
	SINGLE,
	SINGLE_ACCURATE,
	SINGLE_NPC,
	WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	NEARLYEMPTY,
	FAST_RELOAD,

	// Add new shoot sound types here

	NUM_SHOOT_SOUND_TYPES,
};

class C_TEFireBullets
{
public:
	char pad[12];
	int		m_iPlayer; //12
	int m_nItemDefIndex;
	vec3_t	m_vecOrigin;
	ang_t	m_vecAngles;
	int		m_iWeaponID;
	int		m_iMode;
	int		m_iSeed;
	float	m_flSpread;
	float			m_fInaccuracy;
	float			m_flRecoilIndex;
	float			m_fSpread;
#if defined( WEAPON_FIRE_BULLETS_ACCURACY_FISHTAIL_FEATURE )
	float			m_fAccuracyFishtail;
#endif
	WeaponSound_t	m_iSoundType;
};