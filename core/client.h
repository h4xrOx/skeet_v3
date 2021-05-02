#pragma once

class Sequence {
public:
	float m_time;
	int   m_state;
	int   m_seq;

public:
	__forceinline Sequence( ) : m_time{ }, m_state{ }, m_seq{ } {};
	__forceinline Sequence( float time, int state, int seq ) : m_time{ time }, m_state{ state }, m_seq{ seq } {};
};

class NetPos {
public:
	float  m_time;
	vec3_t m_pos;

public:
	__forceinline NetPos( ) : m_time{ }, m_pos{ } {};
	__forceinline NetPos( float time, vec3_t pos ) : m_time{ time }, m_pos{ pos } {};
};

class Client {
public:
	// hack thread.
	static ulong_t __stdcall init( void* arg );

	void StartMove( CUserCmd* cmd );
	void EndMove( CUserCmd* cmd );
	void DoMove( );
	void DrawHUD( );
	void UpdateAnimations( );
	void KillFeed( );
	void ClanTag( );
	void UnlockHiddenConvars( );
	void MouseDelta( );

	void OnPaint( );
	void OnMapload( );
	void OnTick( CUserCmd* cmd );

	// debugprint function.
	void print( const std::string text, ... );

	// check if we are able to fire this tick.
	bool CanFireWeapon( float curtime );
	bool IsFiring( float curtime );
	void UpdateRevolverCock( );
	void UpdateIncomingSequences( );
	void BuildSeedTable();

public:
	// local player variables.
	Player* m_local;
	bool     m_processing;
	int      m_flags;
	vec3_t	 m_shoot_pos;
	bool     m_player_fire;
	bool	 m_shot;
	bool	 m_old_shot;
	bool     m_pressing_move;
	bool     m_negate_desync;

	// active weapon variables.
	Weapon* m_weapon;
	int         m_weapon_id;
	WeaponInfo* m_weapon_info;
	int         m_weapon_type;
	bool        m_weapon_fire;

	// revolver variables.
	int	 m_revolver_cock;
	int	 m_revolver_query;
	bool m_revolver_fire;

	// general game varaibles.
	bool     m_round_end;
	int      m_round_flags;
	Stage_t	 m_stage;
	int	     m_max_lag;
	int	     m_wanted_choke;
	bool	 m_should_lag;
	int      m_lag;
	int	     m_old_lag;
	bool* m_packet;
	bool* m_final_packet;
	bool	 m_old_packet;
	float	 m_lerp;
	float    m_latency;
	int      m_latency_ticks;
	int      m_server_tick;
	int      m_arrival_tick;
	int      m_goal_shift;
	int      m_width, m_height;
	bool	 m_in_glow;
	bool	 m_update_night;
	bool	 m_update_sound;

	// usercommand variables.
	CUserCmd* m_cmd;
	int	      m_tick;
	int	      m_rate;
	int		  m_shot_command_number;
	int		  m_shot_tickbase;
	int	      m_buttons;
	int       m_old_buttons;
	ang_t     m_view_angles;
	ang_t	  m_strafe_angles;
	vec3_t	  m_forward_dir;

	penetration::PenetrationOutput_t m_pen_data;

	std::deque< Sequence > m_sequences;
	std::deque< NetPos >   m_net_pos;

	// animation variables.
	ang_t            m_angle;
	ang_t            m_rotation;
	ang_t            m_radar;
	float            m_speed;
	float            m_anim_time;
	float            m_anim_frame;
	bool             m_animate;
	bool             m_update;
	bool             m_update_fake;
	bool             m_ground;
	bool             m_lagcomp;
	ang_t            m_real_abs_ang;

	float            m_real_poses[ 24 ];
	float            m_fake_poses[ 24 ];
	float            m_backup_poses[ 24 ];

	C_AnimationLayer m_real_layers[ 13 ];
	C_AnimationLayer m_fake_layers[ 13 ];
	C_AnimationLayer m_backup_layers[ 13 ];

	// other variables.
	CCSGOPlayerAnimState*    m_fake_state;
	CCSGOPlayerAnimState*    m_fake_state_allocated;
	ulong_t                  m_ent_handle;
	float                    m_spawn_time;
	BoneArray                m_real_matrix[ 128 ];
	BoneArray                m_fake_matrix[ 128 ];
	std::vector<std::string> m_hitsounds;

	// hack username.
	std::string m_user;
};

extern Client g_cl;