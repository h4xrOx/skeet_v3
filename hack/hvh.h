#pragma once

class AdaptiveAngle {
public:
	float m_yaw;
	float m_dist;

public:
	// ctor.
	__forceinline AdaptiveAngle( float yaw, float penalty = 0.f ) {
		// set yaw.
		m_yaw = math::NormalizedAngle( yaw );

		// init distance.
		m_dist = 0.f;

		// remove penalty.
		m_dist -= penalty;
	}
};

enum AntiAimMode : size_t {
	STAND = 0,
	WALK,
	AIR,
};

class HVH {
public:
	int    m_pitch;
	int    m_yaw;
	int    m_yaw_fake;
	float  m_jitter_range;
	float  m_rot_range;
	float  m_rot_speed;
	float  m_rand_update;
	int    m_dir;
	size_t m_base_angle;
	float  m_auto_time;

	bool   m_step_switch;
	int    m_random_lag;
	float  m_next_random_update;
	float  m_random_angle;
	float  m_direction;
	float  m_real;
	float  m_auto;
	float  m_auto_dist;
	float  m_auto_last;
	float  m_view;

	bool m_fake_duck;
	bool m_use_real_around_fake;
	bool m_jitter_update;

public:
	void AntiAim( );
	void IdealPitch( );
	void AntiAimPitch( );
	void PrepareFake( );
	float ChooseReal( );
	void AdjustYaw( );
	void AtTarget( );
	void UpdateJitter( );
	void SendPacket( );
	void FakeDuck( );
};

extern HVH g_hvh;