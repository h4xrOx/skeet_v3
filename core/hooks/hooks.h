#pragma once

class Hooks {
public:
	void init( );

public:
	// prototypes.
	using PaintTraverse_t              = void( __thiscall* )( void*, VPANEL, bool, bool );
	using DoPostScreenSpaceEffects_t   = bool( __thiscall* )( void*, CViewSetup* );
	using CreateMove_t                 = bool( __thiscall* )( void*, float, CUserCmd* );
	using LevelInitPostEntity_t        = void( __thiscall* )( void* );
	using LevelShutdown_t              = void( __thiscall* )( void* );
	using LevelInitPreEntity_t         = void( __thiscall* )( void*, const char* );
	using IN_KeyEvent_t                = int( __thiscall* )( void*, int, int, const char* );
	using FrameStageNotify_t           = void( __thiscall* )( void*, Stage_t );
	using UpdateClientSideAnimation_t  = void( __thiscall* )( void* );
    using GetActiveWeapon_t            = Weapon*( __thiscall * )( void* );
	using DoExtraBoneProcessing_t      = void( __thiscall* )( void*, int, int, int, int, int, int );
	using CalcView_t                   = void( __thiscall* )( void*, vec3_t&, vec3_t&, float&, float&, float& );
	using Weapon_ShootPosition_t       = vec3_t( __thiscall* )( void*, vec3_t* );
	using BuildTransformations_t	   = void(__thiscall*)(void*, int, int, int, int, int, int);
	using StandardBlendingRules_t	   = void(__thiscall*)(void*, int, int, int, int, int);
	using CalcViewModelView_t          = void( __thiscall* )( void*, vec3_t&, ang_t& );
	using InPrediction_t               = bool( __thiscall* )( void* );
	using OverrideView_t               = void( __thiscall* )( void*, CViewSetup* );
	using LockCursor_t                 = void( __thiscall* )( void* );
	using RunCommand_t                 = void( __thiscall* )( void*, Entity*, CUserCmd*, IMoveHelper* );
	using SetupMove_t				   = void(__thiscall*)(void*, Player*, CUserCmd*, IMoveHelper*, CMoveData*);
	using ProcessPacket_t              = void( __thiscall* )( void*, void*, bool );
	using SendDatagram_t               = int( __thiscall* )( void*, void* );
	using SendNetMsg_t				   = bool(__thiscall*)(void*, INetMessage&, bool, bool);
	// using CanPacket_t                = bool( __thiscall* )( void* );
	using PlaySound_t                  = void( __thiscall* )( void*, const char* );
	using GetScreenSize_t              = void( __thiscall* )( void*, int&, int& );
	using Push3DView_t                 = void( __thiscall* )( void*, CViewSetup&, int, void*, void* );
	using SceneEnd_t                   = void( __thiscall* )( void* );
	using DrawModelExecute_t           = void( __thiscall* )( void*, uintptr_t, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t* );
	using ComputeShadowDepthTextures_t = void( __thiscall* )( void*, const CViewSetup&, bool );
	using GetInt_t                     = int( __thiscall* )( void* );
	using GetFloat_t				   = float(__thiscall*)(void*);
	using IsConnected_t                = bool( __thiscall* )( void* );
	using IsHLTV_t                     = bool( __thiscall* )( void* );
	using OnEntityCreated_t            = void( __thiscall* )( void*, Entity* );
	using OnEntityDeleted_t            = void( __thiscall* )( void*, Entity* );
	using RenderSmokeOverlay_t         = void( __thiscall* )( void*, bool );
	using ShouldDrawFog_t              = bool( __thiscall* )( void* );
	using ShouldDrawParticles_t        = bool( __thiscall* )( void* );
	using Render2DEffectsPostHUD_t     = void( __thiscall* )( void*, const CViewSetup& );
	using OnRenderStart_t              = void( __thiscall* )( void* );
    using RenderView_t                 = void( __thiscall* )( void*, const CViewSetup &, const CViewSetup &, int, int );
	using GetMatchSession_t            = CMatchSessionOnlineHost*( __thiscall* )( void* );
	using OnScreenSizeChanged_t        = void( __thiscall* )( void*, int, int );
	using OverrideConfig_t             = bool( __thiscall* )( void*, MaterialSystem_Config_t*, bool );
	using PostDataUpdate_t             = void( __thiscall* )( void*, DataUpdateType_t );
	using WriteUsercmdDeltaToBuffer_t = bool(__thiscall*)(void*, int, void*, int, int, bool);

public:
	void                     PaintTraverse( VPANEL panel, bool repaint, bool force );
	bool                     DoPostScreenSpaceEffects( CViewSetup* setup );
	bool					 CreateMove( float input_sample_time, CUserCmd* cmd );
	void                     LevelInitPostEntity( );
	void                     LevelShutdown( );
	//int                      IN_KeyEvent( int event, int key, const char* bind );
	void                     LevelInitPreEntity( const char* map );
	void                     FrameStageNotify( Stage_t stage );
	bool					 WriteUsercmdDeltaToBuffer(int m_nSlot, void* m_pBuffer, int m_nFrom, int m_nTo, bool m_bNewCmd);
	void                     UpdateClientSideAnimation( );
	void					 CalcView( vec3_t& eyeOrigin, vec3_t& eyeAngles, float& zNear, float& zFar, float& fov );
	vec3_t					 Weapon_ShootPosition(vec3_t* ang );
	bool                     InPrediction( );
	void				     SetupMove(Player* ent, CUserCmd* cmd, IMoveHelper* movehelper, CMoveData* movedata);
	bool                     ShouldDrawParticles( );
	bool                     ShouldDrawFog( );
	void                     OverrideView( CViewSetup* view );
	void                     LockCursor( );
	void                     PlaySound( const char* name );
	void                     OnScreenSizeChanged( int oldwidth, int oldheight );
	void                     RunCommand( Entity* ent, CUserCmd* cmd, IMoveHelper* movehelper );
	int                      SendDatagram( void* data );
	void                     ProcessPacket( void* packet, bool header );
	bool					 SendNetMsg(INetMessage& msg, bool bForceReliable, bool bVoice);
	//void                     GetScreenSize( int& w, int& h );
	void                     SceneEnd( );
	void                     DrawModelExecute( uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone );
	void                     ComputeShadowDepthTextures( const CViewSetup& view, bool unk );
	int                      DebugSpreadGetInt( );
	int                      CsmShadowGetInt( );
	void                     DoExtraBoneProcessing( int a2, int a3, int a4, int a5, int a6, int a7 );
	void                     BuildTransformations(int a2, int a3, int a4, int a5, int a6, int a7);
	void                     StandardBlendingRules(int a2, int a3, int a4, int a5, int a6);
	bool                     IsConnected( );
	bool                     IsHLTV( );
	void					 FireEvents();
	void                     RenderSmokeOverlay( bool unk );
	void                     OnRenderStart( );
    void                     RenderView( const CViewSetup &view, const CViewSetup &hud_view, int clear_flags, int what_to_draw );
	void                     Render2DEffectsPostHUD( const CViewSetup& setup );
	CMatchSessionOnlineHost* GetMatchSession( );
	bool                     OverrideConfig( MaterialSystem_Config_t* config, bool update );
	static void __stdcall    PostDataUpdate( C_TEFireBullets* thisptr, DataUpdateType_t type );
	static void	__stdcall	 PostDataUpdate_h(DataUpdateType_t type);

	static HRESULT WINAPI Present(IDirect3DDevice9* pDevice, RECT* pRect1, const RECT* pRect2, HWND hWnd, const RGNDATA* pRGNData);
	static HRESULT WINAPI Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParameters);
	static LRESULT WINAPI WndProc( HWND wnd, uint32_t msg, WPARAM wp, LPARAM lp );

public:
	// vmts.
	VMT m_panel;
	VMT m_player;
	VMT m_client_mode;
	VMT m_client;
	VMT m_engine;
	VMT m_prediction;
	VMT m_surface;
	VMT m_render;
	VMT m_net_channel;
	VMT m_render_view;
	VMT m_model_render;
	VMT m_shadow_mgr;
	VMT m_view_render;
	VMT m_match_framework;
	VMT m_material_system;
	VMT m_fire_bullets;
	VMT m_device;

	// cvars
	VMT m_debug_spread;
	VMT m_cl_csm_shadows;

	// wndproc old ptr.
	WNDPROC m_old_wndproc;

	// old player create fn.
	DoExtraBoneProcessing_t     m_DoExtraBoneProcessing;
	UpdateClientSideAnimation_t m_UpdateClientSideAnimation;
    GetActiveWeapon_t           m_GetActiveWeapon;

	// netvar proxies.
	RecvVarProxy_t m_Pitch_original;
    RecvVarProxy_t m_Force_original;
};

extern Hooks                g_hooks;