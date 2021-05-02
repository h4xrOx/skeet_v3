#pragma once

class Animations {
public:
	struct AnimationInfo_t {
		AnimationInfo_t( Player* pEntity, std::deque<LagComp::LagRecord_t> pRecords )
			: m_pEntity( pEntity ), m_pRecords( std::move( pRecords ) ), m_flLastSpawnTime( 0 ) {
		}

		void UpdateAnimations( LagComp::LagRecord_t* pRecord, LagComp::LagRecord_t* pPreviousRecord );

		Player* m_pEntity;
		std::deque<LagComp::LagRecord_t> m_pRecords;

		LagComp::LagRecord_t m_LatestRecord;
		LagComp::LagRecord_t m_PreviousRecord;

		float m_flLastSpawnTime{ };

		/* resolver info */
		// .first = static
		// .second = jitter (low & extended)
		std::pair<int, std::pair<int, int>> m_iMissedDueToResolver{ };
		int m_iMissedDueToSpread{ };

		float m_flPreviousAngle{ };

		float m_flBrute{ };
		float m_flLatestDelta{ };
		float m_flBestSide{ };
		float m_flFinalResolveAngle{ };

		float m_flOldYaw{ };

		float m_flLastPinPulled{ };

		int m_iResolverIndex{ };

		bool m_bWalking{ };
		bool m_bUsingMaxDesync{ };

		ang_t m_vecLastReliableAngle;
	};

	std::unordered_map<ulong_t, AnimationInfo_t> m_ulAnimationInfo;

	void UpdatePlayer( Player* pEntity );

	bool GenerateMatrix( Player* pEntity, BoneArray* pBoneToWorldOut, int boneMask, float currentTime );

	void UpdateLocalAnimations( );
	void UpdateFakeAnimations( );

	AnimationInfo_t* GetAnimationInfo( Player* pEntity );

	LagComp::LagRecord_t m_RealData;
	LagComp::LagRecord_t m_RealShotData;

	LagComp::LagRecord_t m_FakeData;
	LagComp::LagRecord_t m_FakeShotData;

	bool m_bEnableBones;
	bool m_bEnablePVS;
};

extern Animations g_anims;