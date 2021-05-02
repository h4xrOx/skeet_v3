#pragma once

class EnginePrediction {
public:
	struct Variables_t {
		float m_flFrametime;
		float m_flCurtime;
		float m_flVelocityModifier;

		vec3_t m_vecVelocity;
		vec3_t m_vecOrigin;
		int m_fFlags;
	} m_stored_variables;

	struct Misc_t {
		int m_iRunCommandTickbase;
		bool m_bOverrideModifier;
	} Misc;

	struct viewmodel_t {
		float m_flViewmodelCycle;
		float m_flViewmodelAnimTime;
	} StoredViewmodel;

public:
	void UpdatePrediction( );
	void Predict( );
	void Restore( );
	
	void UpdateViewmodelData( );
	void CorrectViewmodelData( );
};

extern EnginePrediction g_inputpred;