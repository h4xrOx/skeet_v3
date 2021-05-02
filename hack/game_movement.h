#pragma once

class GameMovement {
public:
	void Extrapolate( Player* pEntity, vec3_t& vecOrigin, vec3_t& vecVelocity, int& fFlags, bool bOnGround );
};

extern GameMovement g_game_movement;