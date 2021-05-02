#pragma once

#define MAX_WEAPONS	48

enum Hitboxes_t : int {
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_R_THIGH,
	HITBOX_L_THIGH,
	HITBOX_R_CALF,
	HITBOX_L_CALF,
	HITBOX_R_FOOT,
	HITBOX_L_FOOT,
	HITBOX_R_HAND,
	HITBOX_L_HAND,
	HITBOX_R_UPPER_ARM,
	HITBOX_R_FOREARM,
	HITBOX_L_UPPER_ARM,
	HITBOX_L_FOREARM,
	HITBOX_MAX
};

enum PoseParam_t {
	STRAFE_YAW,
	STAND_POSE,
	LEAN_YAW,
	SPEED,
	LADDER_YAW,
	LADDER_SPEED,
	JUMP_FALL,
	MOVE_YAW,
	MOVE_BLEND_CROUCH,
	MOVE_BLEND_WALK,
	MOVE_BLEND_RUN,
	BODY_YAW,
	BODY_PITCH,
	AIM_BLEND_STAND_IDLE,
	AIM_BLEND_STAND_WALK,
	AIM_BLEND_STAND_RUN,
	AIM_BLEND_COURCH_IDLE,
	AIM_BLEND_CROUCH_WALK,
	DEATH_YAW
};

enum RenderFlags_t : uint32_t {
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	STUDIO_TRANSPARENCY = 0x80000000,
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000
};

enum BoneFlags_t : int {
	BONE_USED_BY_ANYTHING = 0x0007FF00,
	BONE_USED_BY_HITBOX = 0x00000100, // bone (or child) is used by a hit box
	BONE_USED_BY_ATTACHMENT = 0x00000200, // bone (or child) is used by an attachment point
	BONE_USED_BY_VERTEX_MASK = 0x0003FC00,
	BONE_USED_BY_VERTEX_LOD0 = 0x00000400, // bone (or child) is used by the toplevel model via skinned vertex
	BONE_USED_BY_VERTEX_LOD1 = 0x00000800,
	BONE_USED_BY_VERTEX_LOD2 = 0x00001000,
	BONE_USED_BY_VERTEX_LOD3 = 0x00002000,
	BONE_USED_BY_VERTEX_LOD4 = 0x00004000,
	BONE_USED_BY_VERTEX_LOD5 = 0x00008000,
	BONE_USED_BY_VERTEX_LOD6 = 0x00010000,
	BONE_USED_BY_VERTEX_LOD7 = 0x00020000,
	BONE_USED_BY_BONE_MERGE = 0x00040000
};

enum CSWeaponType : int {
	WEAPONTYPE_EQUIPMENT = 0, //not sure about this naming
	WEAPONTYPE_KNIFE = 1,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_HEAVY,
	WEAPONTYPE_UNKNOWN
};

enum Teams_t : int {
	TEAM_NOTEAM = 0,
	TEAM_SPECTATOR,
	TEAM_TERRORISTS,
	TEAM_COUNTERTERRORISTS
};

enum Effects_t : int {
	EF_BONEMERGE = 0x001,	// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004,	// player flashlight
	EF_NOINTERP = 0x008,	// don't interpolate the next frame
	EF_NOSHADOW = 0x010,	// Don't cast no shadow
	EF_NODRAW = 0x020,	// don't draw entity
	EF_NORECEIVESHADOW = 0x040,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
	EF_ITEM_BLINK = 0x100,	// blink an item so that the user notices it.
	EF_PARENT_ANIMATES = 0x200,	// always assume that the parent entity is animating
	EF_MAX_BITS = 10
};

enum InvalidatePhysicsBits_t : int {
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,
	BOUNDS_CHANGED = 0x10,
	SEQUENCE_CHANGED = 0x20
};

enum DataUpdateType_t : int {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum LifeStates_t : int {
	LIFE_ALIVE = 0,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

enum PlayerFlags_t : int {
	FL_ONGROUND = ( 1 << 0 ),
	FL_DUCKING = ( 1 << 1 ),
	FL_WATERJUMP = ( 1 << 3 ),
	FL_ONTRAIN = ( 1 << 4 ),
	FL_INRAIN = ( 1 << 5 ),
	FL_FROZEN = ( 1 << 6 ),
	FL_ATCONTROLS = ( 1 << 7 ),
	FL_CLIENT = ( 1 << 8 ),
	FL_FAKECLIENT = ( 1 << 9 ),
	FL_INWATER = ( 1 << 10 ),
};

enum MoveType_t : int {
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
};

enum Weapons_t : int {
	DEAGLE = 1,
	ELITE = 2,
	FIVESEVEN = 3,
	GLOCK = 4,
	AK47 = 7,
	AUG = 8,
	AWP = 9,
	FAMAS = 10,
	G3SG1 = 11,
	GALIL = 13,
	M249 = 14,
	M4A4 = 16,
	MAC10 = 17,
	P90 = 19,
	MP5_SD = 23,
	UMP45 = 24,
	XM1014 = 25,
	BIZON = 26,
	MAG7 = 27,
	NEGEV = 28,
	SAWEDOFF = 29,
	TEC9 = 30,
	ZEUS = 31,
	P2000 = 32,
	MP7 = 33,
	MP9 = 34,
	NOVA = 35,
	P250 = 36,
	SCAR20 = 38,
	SG553 = 39,
	SSG08 = 40,
	KNIFE_CT = 42,
	FLASHBANG = 43,
	HEGRENADE = 44,
	SMOKE = 45,
	MOLOTOV = 46,
	DECOY = 47,
	FIREBOMB = 48,
	C4 = 49,
	MUSICKIT = 58,
	KNIFE_T = 59,
	M4A1S = 60,
	USPS = 61,
	TRADEUPCONTRACT = 62,
	CZ75A = 63,
	REVOLVER = 64,
	KNIFE_BAYONET = 500,
	KNIFE_CSS = 503,
	KNIFE_FLIP = 505,
	KNIFE_GUT = 506,
	KNIFE_KARAMBIT = 507,
	KNIFE_M9_BAYONET = 508,
	KNIFE_HUNTSMAN = 509,
	KNIFE_FALCHION = 512,
	KNIFE_BOWIE = 514,
	KNIFE_BUTTERFLY = 515,
	KNIFE_SHADOW_DAGGERS = 516,
	KNIFE_CORD = 517,
	KNIFE_CANIS = 518,
	KNIFE_URSUS = 519,
	KNIFE_GYPSY_JACKKNIFE = 520,
	KNIFE_OUTDOOR = 521,
	KNIFE_STILETTO = 522,
	KNIFE_WIDOWMAKER = 523,
	KNIFE_SKELETON = 525,
};

struct RenderableInstance_t {
	uint8_t m_alpha;
	__forceinline RenderableInstance_t( ) : m_alpha{ 255ui8 } {}
};

class IHandleEntity {
public:
	virtual ~IHandleEntity( ) {}
	virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
	virtual const ulong_t& GetRefEHandle( ) const = 0;
};

class IClientUnknown : public IHandleEntity {
public:
	virtual void* GetCollideable( ) = 0;
	virtual void* GetClientNetworkable( ) = 0;
	virtual void* GetClientRenderable( ) = 0;
	virtual void* GetIClientEntity( ) = 0;
	virtual void* GetBaseEntity( ) = 0;
	virtual void* GetClientThinkable( ) = 0;
	virtual void* GetClientAlphaProperty( ) = 0;
};

class Entity {
public:
	// helper methods.
	template< typename t >
	__forceinline t& get( size_t offset ) {
		return *( t* )( ( uintptr_t )this + offset );
	}

	template< typename t >
	__forceinline void set( size_t offset, const t& val ) {
		*( t* )( ( uintptr_t )this + offset ) = val;
	}

	template< typename t >
	__forceinline t as( ) {
		return ( t )this;
	}

public:
	// netvars / etc.
	__forceinline vec3_t& m_vecOrigin( ) {
		return get< vec3_t >( g_entoffsets.m_vecOrigin );
	}

	__forceinline vec3_t& m_vecOldOrigin( ) {
		return get< vec3_t >( g_entoffsets.m_vecOldOrigin );
	}

	__forceinline vec3_t& m_vecVelocity( ) {
		return get< vec3_t >( g_entoffsets.m_vecVelocity );
	}

	__forceinline vec3_t& m_vecMins( ) {
		return get< vec3_t >( g_entoffsets.m_vecMins );
	}

	__forceinline vec3_t& m_vecMaxs( ) {
		return get< vec3_t >( g_entoffsets.m_vecMaxs );
	}

	__forceinline int& m_iTeamNum( ) {
		return get< int >( g_entoffsets.m_iTeamNum );
	}

	__forceinline int& m_nSequence( ) {
		return get< int >( g_entoffsets.m_nSequence );
	}

	__forceinline float& m_flCycle( ) {
		return get< float >( g_entoffsets.m_flCycle );
	}

	__forceinline float& m_flAnimTime( ) {
		return get< float >( g_entoffsets.m_flAnimTime );
	}

	__forceinline float& m_flC4Blow( ) {
		return get< float >( g_entoffsets.m_flC4Blow );
	}

	__forceinline bool& m_bBombTicking( ) {
		return get< bool >( g_entoffsets.m_bBombTicking );
	}

	__forceinline int& m_fEffects( ) {
		// todo; netvar.
		return get< int >( g_entoffsets.m_fEffects );
	}

	__forceinline int& m_nModelIndex( ) {
		return get< int >( g_entoffsets.m_nModelIndex );
	}

	__forceinline bool& m_bReadyToDraw( ) {
		return get< bool >( g_entoffsets.m_bReadyToDraw );
	}

public:
	// virtual indices
	enum indices : size_t {
		WORLDSPACECENTER = 78,
		GETMAXHEALTH = 122,
		ISPLAYER = 158,
		ISBASECOMBATWEAPON = 166,
	};

public:
	// virtuals.
	// renderable table.
	__forceinline void* renderable( ) {
		return ( void* )( ( uintptr_t )this + 0x4 );
	}

	__forceinline vec3_t& GetRenderOrigin( ) {
		return util::get_method< vec3_t& ( __thiscall* )( void* ) >( renderable( ), 1 )( renderable( ) );
	}

	__forceinline ang_t& GetRenderAngles( ) {
		return util::get_method< ang_t& ( __thiscall* )( void* ) >( renderable( ), 2 )( renderable( ) );
	}

	__forceinline const model_t* GetModel( ) {
		return util::get_method< const model_t* ( __thiscall* )( void* ) >( renderable( ), 8 )( renderable( ) );
	}

	__forceinline void DrawModel( int flags = STUDIO_RENDER, const RenderableInstance_t& instance = { } ) {
		return util::get_method< void( __thiscall* )( void*, int, const RenderableInstance_t& )>( renderable( ), 9 )( renderable( ), flags, instance );
	}

	__forceinline bool SetupBones( matrix3x4_t* out, int max, int mask, float time ) {
		return util::get_method< bool( __thiscall* )( void*, matrix3x4_t*, int, int, float )>( renderable( ), 13 )( renderable( ), out, max, mask, time );
	}

	// networkable table.
	__forceinline void* networkable( ) {
		return ( void* )( ( uintptr_t )this + 0x8 );
	}

	__forceinline IClientUnknown* GetIClientUnknown( ) {
		return util::get_method< IClientUnknown* ( __thiscall* )( void* ) >( networkable( ), 0 )( networkable( ) );
	}

	__forceinline void Release( ) {
		return util::get_method< void( __thiscall* )( void* ) >( networkable( ), 1 )( networkable( ) );
	}

	__forceinline ClientClass* GetClientClass( ) {
		return util::get_method< ClientClass* ( __thiscall* )( void* ) >( networkable( ), 2 )( networkable( ) );
	}

	__forceinline void OnDataChanged( DataUpdateType_t type ) {
		return util::get_method< void( __thiscall* )( void*, DataUpdateType_t ) >( networkable( ), 5 )( networkable( ), type );
	}

	__forceinline void PreDataUpdate( DataUpdateType_t type ) {
		return util::get_method< void( __thiscall* )( void*, DataUpdateType_t ) >( networkable( ), 6 )( networkable( ), type );
	}

	__forceinline void PostDataUpdate( DataUpdateType_t type ) {
		return util::get_method< void( __thiscall* )( void*, DataUpdateType_t ) >( networkable( ), 7 )( networkable( ), type );
	}

	__forceinline bool dormant( ) {
		return util::get_method< bool( __thiscall* )( void* ) >( networkable( ), 9 )( networkable( ) );
	}

	__forceinline int index( ) {
		return util::get_method< int( __thiscall* )( void* ) >( networkable( ), 10 )( networkable( ) );
	}

	__forceinline void SetDestroyedOnRecreateEntities( ) {
		return util::get_method< void( __thiscall* )( void* ) >( networkable( ), 13 )( networkable( ) );
	}

	// normal table.
	__forceinline const vec3_t& GetAbsOrigin( ) {
		return util::get_method< const vec3_t& ( __thiscall* )( void* ) >( this, 10 )( this );
	}

	__forceinline const ang_t& GetAbsAngles( ) {
		return util::get_method< const ang_t& ( __thiscall* )( void* ) >( this, 11 )( this );
	}

	__forceinline bool IsPlayer( ) {
		return GetClientClass( )->m_ClassID == 40;
	}

	__forceinline bool IsBaseCombatWeapon( ) {
		return util::get_method< bool( __thiscall* )( void* ) >( this, ISBASECOMBATWEAPON )( this );
	}

	__forceinline std::string GetBombsiteName( ) {
		std::string out;

		// note - dex; bomb_target + 0x150 has a char array for site name... not sure how much memory gets allocated for it.
		out.resize( 32u );

		std::memcpy( &out[ 0 ], ( const void* )( ( uintptr_t )this + 0x150 ), 32u );

		return out;
	}

	__forceinline void InvalidatePhysicsRecursive( InvalidatePhysicsBits_t bits ) {
		using InvalidatePhysicsRecursive_t = void( __thiscall* )( decltype( this ), InvalidatePhysicsBits_t );
		g_csgo.InvalidatePhysicsRecursive.as< InvalidatePhysicsRecursive_t >( )( this, bits );
	}

	__forceinline void SetAbsAngles( const ang_t& angles ) {
		using SetAbsAngles_t = void( __thiscall* )( decltype( this ), const ang_t& );
		g_csgo.SetAbsAngles.as< SetAbsAngles_t >( )( this, angles );
	}

	__forceinline void SetAbsOrigin( const vec3_t& origin ) {
		using SetAbsOrigin_t = void( __thiscall* )( decltype( this ), const vec3_t& );
		g_csgo.SetAbsOrigin.as< SetAbsOrigin_t >( )( this, origin );
	}

	__forceinline void SetAbsVelocity( const vec3_t& velocity ) {
		using SetAbsVelocity_t = void( __thiscall* )( decltype( this ), const vec3_t& );
		g_csgo.SetAbsVelocity.as< SetAbsVelocity_t >( )( this, velocity );
	}

	__forceinline void AddEffect( int effects ) {
		m_fEffects( ) |= effects;
	}

	__forceinline int get_class_id( ) {
		ClientClass* cc{ GetClientClass( ) };

		return ( cc ) ? cc->m_ClassID : -1;
	}

	__forceinline bool is( hash32_t hash ) {
		return g_netvars.GetClientID( hash ) == get_class_id( );
	}
};

/*class CCSGOPlayerAnimState {
public:
	PAD( 0x1C );				// 0x0000
	Player* m_outer;			// 0x001C
	PAD( 0x40 );				// 0x0020
	Player* m_player;			// 0x0060
	PAD( 0x8 );					// 0x0064
	float   m_time;				// 0x006C
	int     m_frame;			// 0x0070
	float   m_update_delta;		// 0x0074
	float   m_eye_yaw;			// 0x0078
	float   m_eye_pitch;		// 0x007C
	float   m_goal_feet_yaw;	// 0x0080
	float   m_cur_feet_yaw;		// 0x0084
	PAD( 0x1C );				// 0x0088
	float   m_dip_cycle;        // 0x00A4
	float   m_dip_adj;          // 0x00A8
	PAD( 0x40 );				// 0x00A8
	float   m_speed;			// 0x00EC
	float   m_fall_velocity;    // 0x00F0
	PAD( 0x14 );				// 0x00F0
	bool    m_ground;			// 0x0108
	bool    m_land;             // 0x0109
	PAD( 0x2 );					// 0x010A
	PAD( 0x21C );				// 0x010C
	bool    m_dip_air;			// 0x0328
	PAD( 0x12 );				// 0x0329
	float   m_min_pitch;        // 0x033C
	PAD( 0x4 );					// 0x0340
};*/ // size: 0x344

class CCSGOPlayerAnimState {
public:

	char pad[ 3 ];
	char unknown;
	char pad2[ 91 ];
	Player* m_baseEntity;
	void* m_activeWeapon;
	void* m_lastActiveWeapon;
	float m_flLastClientSideAnimationUpdateTime;
	int m_iLastClientSideAnimationUpdateFramecount;
	float m_flAnimUpdateDelta;
	float m_flEyeYaw;
	float m_flEyePitch;
	float m_flGoalFeetYaw;
	float m_flCurrentFeetYaw;
	float m_flCurrentTorsoYaw;
	float m_flLeanVelocity;
	float m_flLeanAmount;
	char pad4[ 4 ];
	float m_flFeetCycle;
	float m_flFeetYawRate;
	float m_flUnknown2;
	float m_flDuckAmount;
	float m_flLandingDuckAdditiveSomething;
	float unknown3;
	vec3_t m_vecOrigin;
	vec3_t m_vecLastOrigin;
	vec3_t m_vecVelocity;
	vec3_t m_vecMoveSpeed;
	vec3_t m_vecMove;
	float m_flSpeed;
	float m_flSpeedUp;
	float m_flSpeedNormalized;
	float m_flFeetSpeedForwardsOrSideways;
	float m_flFeetSpeedForwardsOrSidewaysFraction;
	float m_flTimeSinceStartedMoving;
	float m_flTimeSinceStoppedMoving;
	bool m_bOnGround;
	unsigned char m_szInHitGroundAnimation;
	char pad7[ 10 ];
	float m_flLastOriginZ;
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation;
	float m_flStopToFullRunningFraction;
	bool m_bJust_Landed;
	bool m_bJust_LeftGround;
	char pad_0120[2];
	float m_flDuckRate;
	bool m_bOnLadder;
	char pad_0128[3];
	float m_flLadderCycle;
	float m_flLadderWeight;
	bool m_bNotRunning;
	char pad_0135[3];
	bool m_bInBalanceAdjust;
	char pad_0141[3];
	void* m_Modifiers;
	int gap148[1];
	float m_flTimeOfLastInjury;
	float m_flLastSetupLeanCurtime;
	vec3_t m_vecLastSetupLeanVelocity;
	vec3_t m_vecSetupLeanVelocityDelta;
	vec3_t m_vecSetupLeanVelocityInterpolated;
	float m_flLeanWeight;
	int m_iUnknownIntArr2[2];
	bool m_bFlashed;
	char m_bFlashedPad[3];
	float m_flStrafeWeight;
	int m_iUnknownint3;
	float m_flStrafeCycle;
	int m_iStrafeSequence;
	bool m_bStrafing;
	char m_bStrafingPad[3];
	float m_flTotalStrafeTime;
	int m_iUnknownInt4;
	bool m_bUnknownBool__;
	bool m_bIsAccelerating;
	char pad_01AE[2];
	void* m_arrPoseParameters[20];
	int m_iUnknownClientInt__;
	float m_flVelocityUnknown;
	int m_iMoveState;
	float m_flMovePlaybackRate;
	float m_flUnknownFL0;
	float m_flUnknownFL;
	float m_flUnknownFL1;
	float m_flMinYawServer;
	float m_flMaxYawServer;
	float m_flMaximumPitchServer;
	float m_flMinimumPitchServer;
	int m_iUnknownInt;
	char pad_02D0[84];
	float m_flEyePosZ;
	bool m_bIsDucked;
	char pad_0329[3];
	float m_flUnknownCap1;
	float m_flMinYaw;
	float m_flMaxYaw;
	float m_flMinPitch;
	float m_flMaxPitch;
	int m_iAnimsetVersion;
}; // size: 0x344

class CStudioHdr {
public:
	class mstudioposeparamdesc_t {
	public:
		int					sznameindex;
		__forceinline char* const name( void ) const { return ( ( char* )this ) + sznameindex; }
		int					flags;	// ????
		float				start;	// starting value
		float				end;	// ending value
		float				loop;	// looping range, 0 for no looping, 360 for rotations, etc.
	};

	studiohdr_t* m_pStudioHdr;
	void* m_pVModel;
};

class C_AnimationLayer {
public:
	float   m_anim_time;			// 0x0000
	float   m_fade_out_time;		// 0x0004
	int     m_flags;				// 0x0008
	int     m_activty;				// 0x000C
	int     m_priority;				// 0x0010
	int     m_order;				// 0x0014
	int     m_sequence;				// 0x0018
	float   m_prev_cycle;			// 0x001C
	float   m_weight;				// 0x0020
	float   m_weight_delta_rate;	// 0x0024
	float   m_playback_rate;		// 0x0028
	float   m_cycle;				// 0x002C
	Entity* m_owner;				// 0x0030
	int     m_bits;					// 0x0034
}; // size: 0x0038

class CBoneAccessor {
public:
	void* m_pAnimating;
	BoneArray* m_pBones;
	int        m_ReadableBones;
	int        m_WritableBones;
};

class CBoneCache {
public:
	BoneArray* m_pCachedBones;
	PAD( 0x8 );
	int        m_CachedBoneCount;
};

class Ragdoll : public Entity {
public:
	__forceinline Player* GetPlayer( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Player* >( m_hPlayer( ) );
	}

	__forceinline EHANDLE& m_hPlayer( ) {
		return get< EHANDLE >( g_entoffsets.m_hPlayer );
	}

	__forceinline float& m_flDeathYaw( ) {
		return get< float >( g_entoffsets.m_flDeathYaw );
	}

	__forceinline float& m_flAbsYaw( ) {
		return get< float >( g_entoffsets.m_flAbsYaw );
	}
};

class Player : public Entity {
public:
	// netvars / etc.
	__forceinline vec3_t& m_vecAbsVelocity( ) {
		return get< vec3_t >( g_entoffsets.m_vecAbsVelocity );
	}

	__forceinline int& m_lifeState( ) {
		return get< int >( g_entoffsets.m_lifeState );
	}

	__forceinline int& m_fFlags( ) {
		return get< int >( g_entoffsets.m_fFlags );
	}

	__forceinline int& m_MoveType( ) {
		return get< int >( g_entoffsets.m_MoveType );
	}

	__forceinline int& m_iHealth( ) {
		return get< int >( g_entoffsets.m_iHealth );
	}

	__forceinline int& m_iAccount( ) {
		return get< int >( g_entoffsets.m_iAccount );
	}

	__forceinline int& m_hViewModel( ) {
		return get< int >( g_entoffsets.m_hViewModel );
	}

	__forceinline bool& m_bHasDefuser( ) {
		return get< bool >( g_entoffsets.m_bHasDefuser );
	}

	__forceinline int& m_nHitboxSet( ) {
		return get< int >( g_entoffsets.m_nHitboxSet );
	}

	__forceinline ang_t& m_angAbsRotation( ) {
		return get< ang_t >( g_entoffsets.m_angAbsRotation );
	}

	__forceinline ang_t& m_angRotation( ) {
		return get< ang_t >( g_entoffsets.m_angRotation );
	}

	__forceinline ang_t& m_angNetworkAngles( ) {
		return get< ang_t >( g_entoffsets.m_angNetworkAngles );
	}

	__forceinline bool m_bIsLocalPlayer( ) {
		// .text:101E0078 674     84 C0				   test    al, al          ; Logical Compare
		// .text:101E007A 674     74 17				   jz      short loc_101E0093; Jump if Zero( ZF = 1 )
		// .text:101E007C 674     8A 83 F8 35 00 00	   mov     al, [ ebx + 35F8h ]
		return get< bool >( g_csgo.IsLocalPlayer );
	}

	__forceinline CCSGOPlayerAnimState* m_PlayerAnimState( ) {
		// .text:1037A5B8 00C     E8 E3 40 E6 FF         call    C_BasePlayer__Spawn ; Call Procedure
		// .text:1037A5BD 00C     80 BE E1 39 00 00 00   cmp     byte ptr[ esi + 39E1h ], 0; Compare Two Operands
		// .text:1037A5C4 00C     74 48                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		// .text:1037A5C6 00C     8B 8E 74 38 00 00      mov     ecx, [ esi + 3874h ]; this
		// .text:1037A5CC 00C     85 C9                  test    ecx, ecx; Logical Compare
		// .text:1037A5CE 00C     74 3E                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		return get< CCSGOPlayerAnimState* >( g_csgo.PlayerAnimState );
	}

	__forceinline void SetPlayerAnimState( CCSGOPlayerAnimState* state ) {
		// .text:1037A5B8 00C     E8 E3 40 E6 FF         call    C_BasePlayer__Spawn ; Call Procedure
		// .text:1037A5BD 00C     80 BE E1 39 00 00 00   cmp     byte ptr[ esi + 39E1h ], 0; Compare Two Operands
		// .text:1037A5C4 00C     74 48                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		// .text:1037A5C6 00C     8B 8E 74 38 00 00      mov     ecx, [ esi + 3874h ]; this
		// .text:1037A5CC 00C     85 C9                  test    ecx, ecx; Logical Compare
		// .text:1037A5CE 00C     74 3E                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		set< CCSGOPlayerAnimState* >( g_csgo.PlayerAnimState, state );
	}

	__forceinline CStudioHdr* m_studioHdr( ) {
		// .text:1017E902 08C    8B 86 3C 29 00 00    mov     eax, [ esi + 293Ch ]
		// .text:1017E908 08C    89 44 24 10          mov[ esp + 88h + var_78 ], eax
		return get< CStudioHdr* >( g_csgo.studioHdr );
	}

	__forceinline ulong_t& m_iMostRecentModelBoneCounter( ) {
		// .text:101AC9A9 000    89 81 80 26 00 00    mov[ ecx + 2680h ], eax
		return get< ulong_t >( g_csgo.MostRecentModelBoneCounter );
	}

	__forceinline float& m_flLastBoneSetupTime( ) {
		// .text:101AC99F 000    C7 81 14 29 00 00 FF FF+    mov     dword ptr [ecx+2914h], 0FF7FFFFFh;
		return get< float >( g_csgo.LastBoneSetupTime );
	}

	__forceinline int& m_nTickBase( ) {
		return get< int >( g_entoffsets.m_nTickBase );
	}

	__forceinline float& m_flNextAttack( ) {
		return get< float >( g_entoffsets.m_flNextAttack );
	}

	__forceinline float& m_flDuckAmount( ) {
		return get< float >( g_entoffsets.m_flDuckAmount );
	}

	__forceinline float& m_flSimulationTime( ) {
		return get< float >( g_entoffsets.m_flSimulationTime );
	}

	__forceinline float& m_flOldSimulationTime( ) {
		return get< float >( g_entoffsets.m_flOldSimulationTime );
	}

	__forceinline matrix3x4_t& m_pCoordFrame( ) {
		return get< matrix3x4_t >( g_entoffsets.m_pCoordFrame );
	}

	__forceinline float& m_flLowerBodyYawTarget( ) {
		return get< float >( g_entoffsets.m_flLowerBodyYawTarget );
	}

	__forceinline float& m_fImmuneToGunGameDamageTime( ) {
		return get< float >( g_entoffsets.m_fImmuneToGunGameDamageTime );
	}

	__forceinline bool& m_bGunGameImmunity( ) {
		return get< bool >( g_entoffsets.m_bGunGameImmunity );
	}

	__forceinline bool& m_bHasHelmet( ) {
		return get< bool >( g_entoffsets.m_bHasHelmet );
	}

	__forceinline bool& m_bClientSideAnimation( ) {
		return get< bool >( g_entoffsets.m_bClientSideAnimation );
	}

	__forceinline bool& m_bHasHeavyArmor( ) {
		return get< bool >( g_entoffsets.m_bHasHeavyArmor );
	}

	__forceinline bool& m_bIsScoped( ) {
		return get< bool >( g_entoffsets.m_bIsScoped );
	}

	__forceinline bool& m_bDucking( ) {
		return get< bool >( g_entoffsets.m_bDucking );
	}

	__forceinline int& m_nNextThinkTick( ) {
		return get<int>( g_entoffsets.m_nNextThinkTick );
	}

	__forceinline bool& m_bSpotted( ) {
		return get< bool >( g_entoffsets.m_bSpotted );
	}

	__forceinline int& m_iOcclusionFlags( ) {
		return get<int>( g_entoffsets.m_iOcclusionFlags );
	}

	__forceinline int& m_iOcclusionFramecount( ) {
		return get<int>( g_entoffsets.m_iOcclusionFramecount );
	}

	__forceinline int& m_iObserverMode( ) {
		return get< int >( g_entoffsets.m_iObserverMode );
	}

	__forceinline int& m_ArmorValue( ) {
		return get< int >( g_entoffsets.m_ArmorValue );
	}

	__forceinline float& m_flMaxspeed( ) {
		return get< float >( g_entoffsets.m_flMaxspeed );
	}

	__forceinline float& m_surfaceFriction( ) {
		return get< float >( g_entoffsets.m_surfaceFriction );
	}

	__forceinline float& m_flFlashBangTime( ) {
		return get< float >( g_entoffsets.m_flFlashBangTime );
	}

	__forceinline ang_t& m_angEyeAngles( ) {
		return get< ang_t >( g_entoffsets.m_angEyeAngles );
	}

	__forceinline ang_t& m_aimPunchAngle( ) {
		return get< ang_t >( g_entoffsets.m_aimPunchAngle );
	}

	__forceinline ang_t& m_viewPunchAngle( ) {
		return get< ang_t >( g_entoffsets.m_viewPunchAngle );
	}

	__forceinline ang_t& m_aimPunchAngleVel( ) {
		return get< ang_t >( g_entoffsets.m_aimPunchAngleVel );
	}

	__forceinline vec3_t& m_vecViewOffset( ) {
		return get< vec3_t >( g_entoffsets.m_vecViewOffset );
	}

	__forceinline CUserCmd& m_PlayerCommand( ) {
		return get< CUserCmd >( g_entoffsets.m_PlayerCommand );
	}

	__forceinline CUserCmd*& m_pCurrentCommand( ) {
		return get< CUserCmd* >( g_entoffsets.m_pCurrentCommand );
	}

	__forceinline int& m_iEFlags( ) {
		return get< int >( g_entoffsets.m_iEFlags );
	}

	__forceinline float* m_flPoseParameter( ) {
		return ( float* )( ( uintptr_t )this + g_entoffsets.m_flPoseParameter );
	}

	__forceinline uint32_t& GetReadableBones( ) {
		return get< uint32_t >( g_entoffsets.ReadableBones );
	}

	__forceinline uint32_t& GetWriteableBones( ) {
		return get< uint32_t >( g_entoffsets.WriteableBones );
	}

	__forceinline matrix3x4_t*& GetBoneArrayForWrite( ) {
		return get< matrix3x4_t* >( g_entoffsets.BoneArrayForWrite );
	}

	__forceinline CBaseHandle* m_hMyWearables( ) {
		return ( CBaseHandle* )( ( uintptr_t )this + g_entoffsets.m_hMyWearables );
	}

	__forceinline CBoneCache& m_BoneCache( ) {
		// TODO; sig
		return get< CBoneCache >( g_entoffsets.m_BoneCache );
	}

	__forceinline matrix3x4_t**& m_BoneCache2( ) {
		// TODO; sig
		return get< matrix3x4_t** >( g_entoffsets.m_BoneCache );
	}

	__forceinline EHANDLE& m_hObserverTarget( ) {
		return get< EHANDLE >( g_entoffsets.m_hObserverTarget );
	}

	__forceinline EHANDLE& m_hActiveWeapon( ) {
		return get< EHANDLE >( g_entoffsets.m_hActiveWeapon );
	}

	__forceinline EHANDLE& m_hGroundEntity( ) {
		return get< EHANDLE >( g_entoffsets.m_hGroundEntity );
	}

	__forceinline CBaseHandle* m_hMyWeapons( ) {
		return ( CBaseHandle* )( ( uintptr_t )this + g_entoffsets.m_hMyWeapons );
	}

	__forceinline C_AnimationLayer* m_AnimOverlay( ) {
		// .text:1017EAB1 08C    8B 47 1C                mov     eax, [edi+1Ch]
		// .text:1017EAB4 08C    8D 0C D5 00 00 00 00    lea     ecx, ds:0[ edx * 8 ]; Load Effective Address
		// .text:1017EABB 08C    2B CA                   sub     ecx, edx; Integer Subtraction
		// .text:1017EABD 08C    8B 80 70 29 00 00       mov     eax, [ eax + 2970h ]
		// .text:1017EAC3 08C    8D 34 C8                lea     esi, [ eax + ecx * 8 ]; Load Effective Address
		// .text:1017EAC6
		return get< C_AnimationLayer* >( g_csgo.AnimOverlay );
	}

	__forceinline float& m_flSpawnTime( ) {
		// .text:10381AB3 00C    F3 0F 10 49 10             movss   xmm1, dword ptr [ecx+10h] ; Move Scalar Single-FP
		// .text:10381AB8 00C    F3 0F 5C 88 90 A2 00 00    subss   xmm1, dword ptr[ eax + 0A290h ]; Scalar Single - FP Subtract
		return get< float >( g_csgo.SpawnTime );
	}	

	__forceinline CBoneAccessor& m_BoneAccessor( ) {
		// .text:101A9253 1C4    C7 81 A0 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 26A0h ], 0FFF00h
		// .text:101A925D 1C4    C7 81 9C 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 269Ch ], 0FFF00h
		// .text:101A9267 1C4    8B 10                            mov     edx, [ eax ]
		// .text:101A9269 1C4    8D 81 94 26 00 00                lea     eax, [ ecx + 2694h ]; Load Effective Address
		// .text:101A926F 1C4    50                               push    eax
		return get< CBoneAccessor >( g_csgo.BoneAccessor );
	}

	__forceinline float& m_flVelocityModifier( ) {
		// .text:10381AB3 00C    F3 0F 10 49 10             movss   xmm1, dword ptr [ecx+10h] ; Move Scalar Single-FP
		// .text:10381AB8 00C    F3 0F 5C 88 90 A2 00 00    subss   xmm1, dword ptr[ eax + 0A290h ]; Scalar Single - FP Subtract
		return get< float >( 0xA38C );
	}

	__forceinline float GetMaxBodyRotation( ) {
		auto animstate = uintptr_t( this->m_PlayerAnimState( ) );

		float
			rate = 180,
			duckammount = *( float* )( animstate + 0xA4 ),
			speedfraction = std::max( 0.f, std::min( *( float* )( animstate + 0xF8 ), 1.f ) ),
			speedfactor = std::max( 0.f, std::min( 1.f, *( float* )( animstate + 0xFC ) ) );

		float
			unk1 = ( ( *( float* )( animstate + 0x11C ) * -0.30000001 ) - 0.19999999 ) * speedfraction,
			unk2 = unk1 + 1.f;

		if( duckammount > 0 )
			unk2 += ( ( duckammount * speedfactor ) * ( 0.5f - unk2 ) );

		return *( float* )( animstate + 0x334 ) * unk2;
	};

public:
	enum indices : size_t {
		GETREFEHANDLE = 2,
		TESTHITBOXES = 52,
		THINK = 138,
		BUILDTRANSFORMATIONS = 189,
		DOEXTRABONEPROCESSING = 197,
		STANDARDBLENDINGRULES = 205,
		UPDATECLIENTSIDEANIMATION = 223, // 55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36
		GETACTIVEWEAPON = 267,
		WEAPONSHOOTPOSITION = 282,
		CALCVIEW = 276,
		PRETHINK = 317,
		RUNPOSTTHINK = 319,
		GETFOV = 326,
		UPDATECOLLISIONBOUNDS = 329 // 56 57 8B F9 8B 0D ? ? ? ? F6 87 ? ? ? ? ?
	};

public:
	// virtuals.
	__forceinline ulong_t GetRefEHandle( ) {
		if( !this->GetIClientUnknown( ) )
			return NULL;

		return this->GetIClientUnknown( )->GetRefEHandle( );
	}

	__forceinline void BuildTransformations( CStudioHdr* hdr, vec3_t* pos, quaternion_t* q, const matrix3x4a_t& transform, int mask, uint8_t* computed ) {
		using BuildTransformations_t = void( __thiscall* )( decltype( this ), CStudioHdr*, vec3_t*, quaternion_t*, matrix3x4a_t const&, int, uint8_t* );
		return util::get_method< BuildTransformations_t >( this, BUILDTRANSFORMATIONS )( this, hdr, pos, q, transform, mask, computed );
	}

	__forceinline void StandardBlendingRules( CStudioHdr* hdr, vec3_t* pos, quaternion_t* q, float time, int mask ) {
		using StandardBlendingRules_t = void( __thiscall* )( decltype( this ), CStudioHdr*, vec3_t*, quaternion_t*, float, int );
		return util::get_method< StandardBlendingRules_t >( this, STANDARDBLENDINGRULES )( this, hdr, pos, q, time, mask );
	}

	__forceinline float GetFOV( ) {
		return util::get_method< float( __thiscall* )( decltype( this ) ) >( this, GETFOV )( this );
	}

	__forceinline const vec3_t& WorldSpaceCenter( ) {
		return util::get_method< const vec3_t& ( __thiscall* )( void* ) >( this, WORLDSPACECENTER )( this );
	}

	__forceinline void EyePosition( vec3_t* pos ) {
		util::get_method< float* ( __thiscall* )( Player*, vec3_t* ) >( this, 168 )( this, pos );
	}

	__forceinline vec3_t Weapon_ShootPosition( ) {
		if( g_hvh.m_fake_duck && this->index( ) == g_csgo.m_engine->GetLocalPlayer( ) ) {
			vec3_t origin = m_vecOrigin( );

			vec3_t vDuckHullMin = g_csgo.m_game_movement->GetPlayerMins( true );
			vec3_t vStandHullMin = g_csgo.m_game_movement->GetPlayerMins( false );

			float fMore = ( vDuckHullMin.z - vStandHullMin.z );

			vec3_t vecDuckViewOffset = g_csgo.m_game_movement->GetPlayerViewOffset( true );
			vec3_t vecStandViewOffset = g_csgo.m_game_movement->GetPlayerViewOffset( false );
			float duckFraction = m_flDuckAmount( );

			float tempz = ( ( vecDuckViewOffset.z - fMore ) * duckFraction ) +
				( vecStandViewOffset.z * ( 1 - duckFraction ) );

			origin.z += tempz;

			return origin;
		}

		vec3_t dummy;
		EyePosition( &dummy );

		if( *reinterpret_cast < int32_t* > ( uintptr_t( this ) + 0x3AB4 ) ) {
			auto animstate = this->m_PlayerAnimState( );
			if( animstate ) {
				ModifyEyePosition( animstate, &dummy );
			}
		}

		return dummy;
	}

	__forceinline void UpdateClientSideAnimation( ) {
		return util::get_method< void( __thiscall* )( decltype( this ) ) >( this, UPDATECLIENTSIDEANIMATION )( this );
	}

	__forceinline void UpdateCollisionBounds( ) {
		return util::get_method< void( __thiscall* )( decltype( this ) ) >( this, UPDATECOLLISIONBOUNDS )( this );
	}

	// misc funcs.
	__forceinline CStudioHdr* GetModelPtr( ) {
		using LockStudioHdr_t = void( __thiscall* )( decltype( this ) );

		if( !m_studioHdr( ) )
			g_csgo.LockStudioHdr.as< LockStudioHdr_t >( )( this );

		return m_studioHdr( );
	}

	__forceinline void PreThink( ) {
		return util::get_method< void( __thiscall* )( decltype( this ) ) >( this, PRETHINK )( this );
	}

	__forceinline void Think( ) {
		return util::get_method< void( __thiscall* )( decltype( this ) ) >( this, THINK )( this );
	}

	__forceinline void RunPostThink( ) {
		return util::get_method< void( __thiscall* )( decltype( this ) ) >( this, RUNPOSTTHINK )( this );
	}

	__forceinline void RunPreThink( ) {
		static auto fn = pattern::find( g_csgo.m_client_dll, XOR( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) ).as<bool( __thiscall* )( void*, int )>( );

		if( fn( this, 0 ) )
			PreThink( );
	}

	__forceinline void RunThink( ) {
		static auto fn = pattern::find( g_csgo.m_client_dll, XOR( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6" ) ).as<void( __thiscall* )( int )>( );

		if( m_nNextThinkTick( ) != -1 && m_nNextThinkTick( ) > 0
			&& m_nNextThinkTick( ) < game::TIME_TO_TICKS( g_csgo.m_globals->m_curtime ) ) {
			m_nNextThinkTick( ) = -1;
			fn( 0 );
			Think( );
		}
	}

	__forceinline Weapon* GetActiveWeapon( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >( m_hActiveWeapon( ) );
	}

	__forceinline Entity* GetObserverTarget( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle( m_hObserverTarget( ) );
	}

	__forceinline Entity* GetGroundEntity( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle( m_hGroundEntity( ) );
	}

	__forceinline void SetAnimLayers( C_AnimationLayer* layers ) {
		std::memcpy( m_AnimOverlay( ), layers, sizeof( C_AnimationLayer ) * 13 );
	}

	__forceinline void GetAnimLayers( C_AnimationLayer* layers ) {
		std::memcpy( layers, m_AnimOverlay( ), sizeof( C_AnimationLayer ) * 13 );
	}

	__forceinline void SetPoseParameters( float* poses ) {
		std::memcpy( m_flPoseParameter( ), poses, sizeof( float ) * 24 );
	}

	__forceinline void GetPoseParameters( float* poses ) {
		std::memcpy( poses, m_flPoseParameter( ), sizeof( float ) * 24 );
	}

	__forceinline bool ComputeHitboxSurroundingBox( vec3_t* mins, vec3_t* maxs ) {
		using ComputeHitboxSurroundingBox_t = bool( __thiscall* )( void*, vec3_t*, vec3_t* );

		return g_csgo.ComputeHitboxSurroundingBox.as< ComputeHitboxSurroundingBox_t >( )( this, mins, maxs );
	}

	__forceinline int GetSequenceActivity( int sequence ) {
		using GetSequenceActivity_t = int( __fastcall* )( CStudioHdr*, int );

		return g_csgo.GetSequenceActivity.as< GetSequenceActivity_t >( )( GetModelPtr( ), sequence );
	}

	__forceinline bool HasC4( ) {
		using HasC4_t = bool( __thiscall* )( decltype( this ) );
		return g_csgo.HasC4.as< HasC4_t >( )( this );
	}

	__forceinline void InvalidateBoneCache( ) {
		CBoneAccessor* accessor = &m_BoneAccessor( );
		if( !accessor )
			return;

		accessor->m_WritableBones = 0;
		accessor->m_ReadableBones = 0;

		m_iMostRecentModelBoneCounter( ) = 0;
		m_flLastBoneSetupTime( ) = std::numeric_limits< float >::lowest( );
	}

	__forceinline bool alive( ) {
		//return m_lifeState( ) == LIFE_ALIVE;
		return util::get_method< bool(__thiscall*)(decltype(this)) >(this, 157)(this);
	}

	__forceinline bool enemy( Player* from ) {
		if( m_iTeamNum( ) != from->m_iTeamNum( ) )
			return true;

		else if( g_csgo.mp_teammates_are_enemies->GetInt( ) )
			return true;

		return false;
	}

	void ModifyEyePosition( CCSGOPlayerAnimState* state, vec3_t* pos );
};

class WeaponInfo {
public:

	virtual ~WeaponInfo( ) = default;

	//char pad_0000[4]; //0x0000
	char* consoleName;				// 0x0004
	char		pad_0008[ 8 ];				// 0x0008
	void* m_pWeaponDef;  //0x0010
	int			iMaxClip1;					// 0x0014
	int			iMaxClip2;					// 0x0018
	int			iDefaultClip1;				// 0x001C
	int			iDefaultClip2;				// 0x0020
	char		pad_0024[ 8 ];				// 0x0024
	char* szWorldModel;				// 0x002C
	char* szViewModel;				// 0x0030
	char* szDroppedModel;				// 0x0034
	char		pad_0038[ 4 ];				// 0x0038
	char* N0000023E;					// 0x003C
	char		pad_0040[ 56 ];				// 0x0040
	char* szEmptySound;				// 0x0078
	char		pad_007C[ 4 ];				// 0x007C
	char* szBulletType;				// 0x0080
	char		pad_0084[ 4 ];				// 0x0084
	char* szHudName;					// 0x0088
	char* szWeaponName;				// 0x008C
	char		pad_0090[ 60 ];				// 0x0090
	int 		WeaponType;					// 0x00C8
	int			iWeaponPrice;				// 0x00CC
	int			iKillAward;					// 0x00D0
	char* szAnimationPrefix;			// 0x00D4
	float		flCycleTime;				// 0x00D8
	float		flCycleTimeAlt;				// 0x00DC
	float		flTimeToIdle;				// 0x00E0
	float		flIdleInterval;				// 0x00E4
	bool		bFullAuto;					// 0x00E8
	char		pad_0x00E5[ 3 ];			// 0x00E9
	int			iDamage;					// 0x00EC
	float		flArmorRatio;				// 0x00F0
	int			iBullets;					// 0x00F4
	float		flPenetration;				// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;					// 0x0104
	float		flRangeModifier;			// 0x0108
	float		flThrowVelocity;			// 0x010C
	char		pad_0x010C[ 16 ];			// 0x0110
	bool		bHasSilencer;				// 0x011C
	char		pad_0x0119[ 3 ];			// 0x011D
	char* pSilencerModel;				// 0x0120
	int			iCrosshairMinDistance;		// 0x0124
	float		flMaxPlayerSpeed;	        // 0x0128
	float		flMaxPlayerSpeedAlt;		// 0x012C
	char		pad_0x0130[ 4 ];		    // 0x0130
	float		flSpread;					// 0x0134
	float		flSpreadAlt;				// 0x0138
	float		flInaccuracyCrouch;			// 0x013C
	float		flInaccuracyCrouchAlt;		// 0x0140
	float		flInaccuracyStand;			// 0x0144
	float		flInaccuracyStandAlt;		// 0x0148
	float		flInaccuracyJumpInitial;	// 0x014C
	float		flInaccuracyJump;			// 0x0150
	float		flInaccuracyJumpAlt;		// 0x0154
	float		flInaccuracyLand;			// 0x0158
	float		flInaccuracyLandAlt;		// 0x015C
	float		flInaccuracyLadder;			// 0x0160
	float		flInaccuracyLadderAlt;		// 0x0164
	float		flInaccuracyFire;			// 0x0168
	float		flInaccuracyFireAlt;		// 0x016C
	float		flInaccuracyMove;			// 0x0170
	float		flInaccuracyMoveAlt;		// 0x0174
	float		flInaccuracyReload;			// 0x0178
	int			iRecoilSeed;				// 0x017C
	float		flRecoilAngle;				// 0x0180
	float		flRecoilAngleAlt;			// 0x0184
	float		flRecoilAngleVariance;		// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;			// 0x0190
	float		flRecoilMagnitudeAlt;		// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;		// 0x01A0
	float		flRecoveryTimeStand;		// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;	// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;			// 0x01B8
	bool		bHideViewModelZoomed;		// 0x01B9
	char		pad_0x01B5[ 2 ];			// 0x01BA
	char		iZoomLevels[ 3 ];			// 0x01BC
	int			iZoomFOV[ 2 ];				// 0x01C0
	float		fZoomTime[ 3 ];				// 0x01C4
	char* szWeaponClass;				// 0x01D4
	float		flAddonScale;				// 0x01D8
	char		pad_0x01DC[ 4 ];			// 0x01DC
	char* szEjectBrassEffect;			// 0x01E0
	char* szTracerEffect;				// 0x01E4
	int			iTracerFrequency;			// 0x01E8
	int			iTracerFrequencyAlt;		// 0x01EC
	char* szMuzzleFlashEffect_1stPerson;	// 0x01F0
	char		pad_0x01F4[ 4 ];				// 0x01F4
	char* szMuzzleFlashEffect_3rdPerson;	// 0x01F8
	char		pad_0x01FC[ 4 ];			// 0x01FC
	char* szMuzzleSmokeEffect;		// 0x0200
	float		flHeatPerShot;				// 0x0204
	char* szZoomInSound;				// 0x0208
	char* szZoomOutSound;				// 0x020C
	float		flInaccuracyPitchShift;		// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;			// 0x0218
	char		pad_0x0218[ 8 ];			// 0x0220
	char* pWrongTeamMsg;				// 0x0224
	bool		bHasBurstMode;				// 0x0228
	char		pad_0x0225[ 3 ];			// 0x0229
	bool		bIsRevolver;				// 0x022C
	bool		bCannotShootUnderwater;		// 0x0230
};

class IRefCounted {
private:
	volatile long refCount;

public:
	virtual void destructor( char bDelete ) = 0;
	virtual bool OnFinalRelease( ) = 0;

	void unreference( ) {
		if( InterlockedDecrement( &refCount ) == 0 && OnFinalRelease( ) ) {
			destructor( 1 );
		}
	}
};

class Weapon : public Entity {
public:
	using ref_vec_t = CUtlVector< IRefCounted* >;

	// netvars / etc.
	__forceinline ref_vec_t& m_CustomMaterials( ) {
		return get< ref_vec_t >( g_entoffsets.m_CustomMaterials );
	}

	__forceinline ref_vec_t& m_CustomMaterials2( ) {
		return get< ref_vec_t >( g_entoffsets.m_CustomMaterials2 );
	}

	__forceinline ref_vec_t& m_VisualsDataProcessors( ) {
		return get< ref_vec_t >( g_entoffsets.m_VisualsDataProcessors );
	}

	__forceinline bool& m_bCustomMaterialInitialized( ) {
		return get< bool >( g_entoffsets.m_bCustomMaterialInitialized );
	}

	__forceinline short& m_iItemDefinitionIndex( ) {
		return get< short >( g_entoffsets.m_iItemDefinitionIndex );
	}

	__forceinline int& m_iClip1( ) {
		return get< int >( g_entoffsets.m_iClip1 );
	}

	__forceinline int& m_iPrimaryReserveAmmoCount( ) {
		return get< int >( g_entoffsets.m_iPrimaryReserveAmmoCount );
	}

	__forceinline int& m_Activity( ) {
		return get< int >( g_entoffsets.m_Activity );
	}

	__forceinline float& m_fFireDuration( ) {
		return get< float >( g_entoffsets.m_fFireDuration );
	}

	__forceinline int& m_iBurstShotsRemaining( ) {
		return get< int >( g_entoffsets.m_iBurstShotsRemaining );
	}

	__forceinline float& m_flNextPrimaryAttack( ) {
		return get< float >( g_entoffsets.m_flNextPrimaryAttack );
	}

	__forceinline float& m_flNextSecondaryAttack( ) {
		return get< float >( g_entoffsets.m_flNextSecondaryAttack );
	}

	__forceinline float& m_flThrowStrength( ) {
		return get< float >( g_entoffsets.m_flThrowStrength );
	}

	__forceinline float& m_fNextBurstShot( ) {
		return get< float >( g_entoffsets.m_fNextBurstShot );
	}

	__forceinline int& m_zoomLevel( ) {
		return get< int >( g_entoffsets.m_zoomLevel );
	}

	__forceinline float& m_flRecoilIndex( ) {
		return get< float >( g_entoffsets.m_flRecoilIndex );
	}

	__forceinline int& m_weaponMode( ) {
		return get< int >( g_entoffsets.m_weaponMode );
	}

	__forceinline int& m_nFallbackPaintKit( ) {
		return get< int >( g_entoffsets.m_nFallbackPaintKit );
	}

	__forceinline int& m_nFallbackStatTrak( ) {
		return get< int >( g_entoffsets.m_nFallbackStatTrak );
	}

	__forceinline int& m_nFallbackSeed( ) {
		return get< int >( g_entoffsets.m_nFallbackSeed );
	}

	__forceinline float& m_flFallbackWear( ) {
		return get< float >( g_entoffsets.m_flFallbackWear );
	}

	__forceinline int& m_iViewModelIndex( ) {
		return get< int >( g_entoffsets.m_iViewModelIndex );
	}

	__forceinline int& m_iWorldModelIndex( ) {
		return get< int >( g_entoffsets.m_iWorldModelIndex );
	}

	__forceinline int& m_iAccountID( ) {
		return get< int >( g_entoffsets.m_iAccountID );
	}

	__forceinline int& m_iItemIDHigh( ) {
		return get< int >( g_entoffsets.m_iItemIDHigh );
	}

	__forceinline int& m_iEntityQuality( ) {
		return get< int >( g_entoffsets.m_iEntityQuality );
	}

	__forceinline int& m_OriginalOwnerXuidLow( ) {
		return get< int >( g_entoffsets.m_OriginalOwnerXuidLow );
	}

	__forceinline int& m_OriginalOwnerXuidHigh( ) {
		return get< int >( g_entoffsets.m_OriginalOwnerXuidHigh );
	}

	__forceinline bool& m_bPinPulled( ) {
		return get< bool >( g_entoffsets.m_bPinPulled );
	}

	__forceinline float& m_fThrowTime( ) {
		return get< float >( g_entoffsets.m_fThrowTime );
	}

	__forceinline EHANDLE& m_hWeapon( ) {
		return get< EHANDLE >( g_entoffsets.m_hWeapon );
	}

	__forceinline EHANDLE& m_hWeaponWorldModel( ) {
		return get< EHANDLE >( g_entoffsets.m_hWeaponWorldModel );
	}

	__forceinline EHANDLE& m_hOwnerEntity( ) {
		return get< EHANDLE >( g_entoffsets.m_hOwnerEntity );
	}

	__forceinline float& m_flConstraintRadius( ) {
		return get< float >( g_entoffsets.m_flConstraintRadius );
	}

	__forceinline float& m_fLastShotTime( ) {
		return get< float >( g_entoffsets.m_fLastShotTime );
	}

	__forceinline bool IsGrenade( ) { return m_iItemDefinitionIndex( ) >= Weapons_t::FLASHBANG && m_iItemDefinitionIndex( ) <= Weapons_t::FIREBOMB; }

	__forceinline bool IsZoomable( bool extra_check = true ) {
		return m_iItemDefinitionIndex( ) == Weapons_t::SSG08
			|| m_iItemDefinitionIndex( ) == Weapons_t::SCAR20
			|| m_iItemDefinitionIndex( ) == Weapons_t::AWP
			|| m_iItemDefinitionIndex( ) == Weapons_t::G3SG1
			|| ( extra_check && ( m_iItemDefinitionIndex( ) == Weapons_t::SG553 || m_iItemDefinitionIndex( ) == Weapons_t::AUG ) );
	}

public:
	enum indices : size_t {
		SETMODELINDEX = 75,
		GETMAXCLIP1 = 367,
		GETSPREAD = 452,
		GETWPNDATA = 460, // C_WeaponCSBaseGun::GetCSWpnData
		GETINACCURACY = 482,
		UPDATEACCURACYPENALTY = 483,
	};

public:
	// virtuals.
	__forceinline int GetMaxClip1( ) {
		return util::get_method< int( __thiscall* )( void* ) >( this, GETMAXCLIP1 )( this );
	}

	__forceinline void SetGloveModelIndex( int index ) {
		return util::get_method< void( __thiscall* )( void*, int ) >( this, SETMODELINDEX )( this, index );
	}

	__forceinline WeaponInfo* GetWpnData( ) {
		return util::get_method< WeaponInfo* ( __thiscall* )( void* ) >( this, GETWPNDATA )( this );
	}

	__forceinline float GetInaccuracy( ) {
		return util::get_method< float( __thiscall* )( void* ) >( this, GETINACCURACY )( this );
	}

	__forceinline float GetSpread( ) {
		return util::get_method< float( __thiscall* )( void* ) >( this, GETSPREAD )( this );
	}

	__forceinline void UpdateAccuracyPenalty( ) {
		return util::get_method< void( __thiscall* )( void* ) >( this, UPDATEACCURACYPENALTY )( this );
	}

	// misc funcs.
	__forceinline Weapon* GetWeapon( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >( m_hWeapon( ) );
	}

	__forceinline Weapon* GetWeaponWorldModel( ) {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >( m_hWeaponWorldModel( ) );
	}

	__forceinline bool IsKnife( ) {
		return ( GetWpnData( )->WeaponType == WEAPONTYPE_KNIFE && m_iItemDefinitionIndex( ) != ZEUS );
	}

	__forceinline vec3_t CalculateSpread( int seed, float inaccuracy, float spread, bool revolver2 = false ) {
		WeaponInfo* wep_info;
		int        item_def_index;
		float      recoil_index, r1, r2, r3, r4, s1, c1, s2, c2;

		// if we have no bullets, we have no spread.
		wep_info = GetWpnData( );
		if( !wep_info || !wep_info->iBullets )
			return { };

		// get some data for later.
		item_def_index = m_iItemDefinitionIndex( );
		recoil_index = m_flRecoilIndex( );

		// generate needed floats.
		r1 = std::get<0>(g_csgo.m_computed_seeds[seed]);
		r2 = std::get<1>(g_csgo.m_computed_seeds[seed]);

		// todo - dex; need to make sure this is right for shotguns still.
		//             the 3rd arg to get_shotgun_spread is actually using the bullet iterator
		//             should also probably check for if the weapon is a shotgun, but it seems like GetShotgunSpread modifies some the r1 - r4 vars...
		// for( int i{}; i < wep_info->m_bullets; ++i )
		/*
			// shotgun shit, or first bullet
			if ( !bullet_i
			  || ((int (__thiscall *)(void ***))weapon_accuracy_shotgun_spread_patterns[13])(&weapon_accuracy_shotgun_spread_patterns) )
			{
			  r1 = RandomFloat(0, 0x3F800000);// rand 0.f, 1.f
			  r2 = RandomFloat(0, 0x40C90FDB);// rand 0.f, pi * 2.f
			  v47 = *(_DWORD *)v45;
			  r2_ = r2;
			  v48 = (*(int (__thiscall **)(int))(v47 + 48))(v45);

			  // not revolver?
			  if ( v48 != sub_101D9B10(&dword_14FA0DE0) || a7 != 1 )
			  {

				// not negev?
				v50 = (*(int (__thiscall **)(int))(*(_DWORD *)v45 + 48))(v45);
				if ( v50 != sub_101D9B10(&dword_14FA0DEC) || recoil_index >= 3.0 )
				{
				  r1_ = r1;
				}
				else
				{
				  r1__ = r1;
				  recoil_index_it1 = 3;
				  do
				  {
					--recoil_index_it1;
					r1__ = r1__ * r1__;
				  }
				  while ( (float)recoil_index_it1 > recoil_index );
				  r1_ = 1.0 - r1__;
				}
			  }
			  else
			  {
				r1_ = 1.0 - (float)(r1 * r1);
			  }

			  // some convar stuff / etc
			  if ( max_inaccuracy )
				r1_ = 1.0;

			  bullet_i = v87;
			  bullet_i1 = r1_ * v104;

			  if ( only_up_is_not_0 )
				v53 = 1.5707964;
			  else
				v53 = r2_;

			  r2_ = v53;
			}

			if ( ((int (__thiscall *)(void ***))weapon_accuracy_shotgun_spread_patterns[13])(&weapon_accuracy_shotgun_spread_patterns) )
			{
			  null0 = v54;
			  m_iItemDefinitionIndex = (**(int (***)(void))LODWORD(v88))();
			  get_shotgun_spread(
				m_iItemDefinitionIndex,
				null0,
				bullet_i + m_BulletsPerShot * (signed int)recoil_index,
				&r4,
				&r3);
			}
			else
			{
			  r3 = RandomFloat(0, 0x3F800000);// rand 0.f, 1.f
			  r4 = RandomFloat(0, 0x40C90FDB);// rand 0.f, pi * 2.f
			}
		*/

		if( /*wep_info->m_weapon_type == WEAPONTYPE_SHOTGUN &&*/ g_csgo.weapon_accuracy_shotgun_spread_patterns->GetInt( ) > 0 )
			g_csgo.GetShotgunSpread( item_def_index, 0, 0 /*bullet_i*/ + wep_info->iBullets * recoil_index, &r4, &r3 );

		else {
			r3 = std::get<0>(g_csgo.m_computed_seeds[seed]);
			r4 = std::get<1>(g_csgo.m_computed_seeds[seed]);
		}

		// revolver secondary spread.
		if( item_def_index == REVOLVER && revolver2 ) {
			r1 = 1.f - ( r1 * r1 );
			r3 = 1.f - ( r3 * r3 );
		}

		// negev spread.
		else if( item_def_index == NEGEV && recoil_index < 3.f ) {
			for( int i{ 3 }; i > recoil_index; --i ) {
				r1 *= r1;
				r3 *= r3;
			}

			r1 = 1.f - r1;
			r3 = 1.f - r3;
		}

		// get needed sine / cosine values.
		c1 = std::cos( r2 );
		c2 = std::cos( r4 );
		s1 = std::sin( r2 );
		s2 = std::sin( r4 );

		// calculate spread vector.
		return {
			( c1 * ( r1 * inaccuracy ) ) + ( c2 * ( r3 * spread ) ),
			( s1 * ( r1 * inaccuracy ) ) + ( s2 * ( r3 * spread ) ),
			0.f
		};
	}

	__forceinline vec3_t CalculateSpread( int seed, bool revolver2 = false ) {
		return CalculateSpread( seed, GetInaccuracy( ), GetSpread( ), revolver2 );
	}

	__forceinline std::string GetLocalizedName( ) {
		auto weapondata = this->GetWpnData( );
		if( !weapondata )
			return XOR( "ERROR" );

		return util::WideToMultiByte( g_csgo.m_localize->Find( weapondata->szHudName ) );
	}
};

class CTraceFilterSimple_game {
public:
	void* m_vmt;
	const Entity* m_pass_ent1;
	int             m_collision_group;
	ShouldHitFunc_t m_shouldhit_check_fn;

public:
	__forceinline CTraceFilterSimple_game( ) :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >( ) },
		m_pass_ent1{ },
		m_collision_group{ },
		m_shouldhit_check_fn{ } {}

	__forceinline CTraceFilterSimple_game( const Entity* pass_ent1, int collision_group = COLLISION_GROUP_NONE, ShouldHitFunc_t shouldhit_check_fn = nullptr ) :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >( ) },
		m_pass_ent1{ pass_ent1 },
		m_collision_group{ collision_group },
		m_shouldhit_check_fn{ shouldhit_check_fn } {}

	__forceinline bool ShouldHitEntity( Entity* entity, int contents_mask ) {
		// note - dex; game is dumb, this gets the real vmt.
		void* real_vmt = *( void** )m_vmt;

		return util::get_method< bool( __thiscall* )( void*, Entity*, int ) >( real_vmt, 0 )( real_vmt, entity, contents_mask );
	}

	// note - dex; don't really care about calling the virtuals for these two functions, they only set members in the class for us.
	__forceinline void SetPassEntity( Entity* pass_ent1 ) {
		m_pass_ent1 = pass_ent1;

		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 2 )( m_vmt, pass_ent1 );
	}

	__forceinline void SetCollisionGroup( int collision_group ) {
		m_collision_group = collision_group;

		// util::get_method< void (__thiscall *)( void *, int ) >( m_vmt, 3 )( m_vmt, collision_group );
	}
};

class CTraceFilterSkipTwoEntities_game {
public:
	void* m_vmt;
	const Entity* m_pass_ent1;
	int             m_collision_group;
	ShouldHitFunc_t m_shouldhit_check_fn;
	const Entity* m_pass_ent2;

public:
	__forceinline CTraceFilterSkipTwoEntities_game( ) :
		m_vmt{ g_csgo.CTraceFilterSkipTwoEntities_vmt.as< void* >( ) },
		m_pass_ent1{ },
		m_collision_group{ },
		m_shouldhit_check_fn{ },
		m_pass_ent2{ } {}

	__forceinline CTraceFilterSkipTwoEntities_game( const Entity* pass_ent1, const Entity* pass_ent2, int collision_group = COLLISION_GROUP_NONE, ShouldHitFunc_t shouldhit_check_fn = nullptr ) :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >( ) },
		m_pass_ent1{ pass_ent1 },
		m_collision_group{ collision_group },
		m_shouldhit_check_fn{ shouldhit_check_fn },
		m_pass_ent2{ pass_ent2 } {}

	__forceinline bool ShouldHitEntity( Entity* entity, int contents_mask ) {
		// note - dex; game is dumb, this gets the real vmt.
		void* real_vmt = *( void** )m_vmt;

		return util::get_method< bool( __thiscall* )( void*, Entity*, int ) >( m_vmt, 0 )( m_vmt, entity, contents_mask );
	}

	// note - dex; don't really care about calling the virtuals for these two functions, they only set members in the class for us.
	__forceinline void SetPassEntity( Entity* pass_ent1 ) {
		m_pass_ent1 = pass_ent1;

		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 2 )( m_vmt, pass_ent1 );
	}

	__forceinline void SetCollisionGroup( int collision_group ) {
		m_collision_group = collision_group;

		// util::get_method< void (__thiscall *)( void *, int ) >( m_vmt, 3 )( m_vmt, collision_group );
	}

	__forceinline void SetPassEntity2( Entity* pass_ent2 ) {
		m_pass_ent2 = pass_ent2;
		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 4 )( m_vmt, pass_ent2 );
	}
};