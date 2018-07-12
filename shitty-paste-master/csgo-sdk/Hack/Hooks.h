#pragma once

using CreateMoveFn = bool(__stdcall*)(float, CUserCmd*);
extern CreateMoveFn oCreateMove;

using FrameStageNotifyFn = void(__stdcall*)(ClientFrameStage_t);
extern FrameStageNotifyFn oFrameStageNotify;

using PaintTraverseFn = void(__fastcall*)(void*, void*, unsigned int, bool, bool);
extern PaintTraverseFn oPaintTraverse;

using OverrideViewFn = void(__stdcall*)(CViewSetup*);
extern OverrideViewFn oOverrideView;

using InPredictionFn = bool(__stdcall*)();
extern InPredictionFn oInPrediction;

using EndSceneFn = long(__stdcall*)(IDirect3DDevice9* device);
extern EndSceneFn oEndScene;

using ResetFn = long(__stdcall*)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
extern ResetFn oReset;

typedef void(__thiscall* TraceRayFn)(void*, Ray_t const&, unsigned int, CTraceFilter*, trace_t&);
extern TraceRayFn oTraceRay;

typedef void(__fastcall* RunCommandFn)(void*, void*, CBaseEntity*, CUserCmd*, IMoveHelper*);
extern RunCommandFn oRunCommand;

typedef float(__fastcall* GetLastTimeStampFn)(void* ecx);
extern GetLastTimeStampFn oGetLastTimeStamp;

typedef bool(__thiscall* IsPlayingTimeDemoFn)(void*);
extern IsPlayingTimeDemoFn oIsPlayingTimeDemo;

typedef bool(__thiscall *IsConnectedFn)(PVOID);
extern IsConnectedFn oIsConnected;

using DrawModelExecuteFn = void*(__stdcall*)(void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
extern DrawModelExecuteFn oDrawModelExecute;

typedef HRESULT(__stdcall* PresentFn)(IDirect3DDevice9*, RECT*, CONST RECT*, HWND, CONST RGNDATA*);
extern PresentFn oPresent;

typedef bool(__thiscall *FireEventFn)(void*, IGameEvent*);
extern FireEventFn oFireEvent;

extern WNDPROC oWndProc;
extern RecvVarProxyFn oLowerBodyYawTargetProxy;

typedef void(__thiscall* FireBullets_t)(C_TEFireBullets*, DataUpdateType_t);
extern FireBullets_t oFireBullet;

typedef void(__thiscall* DrawHuso_t)(void*, DrawModelResults_t *pResults, const DrawModelInfo_t& info,
	matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags);
extern DrawHuso_t oDrawHuso;

typedef MDLHandle_t(__thiscall *FindMdlFn)(void*, char*);
extern FindMdlFn oFindMDL;

namespace Hooks {
	extern void __stdcall DETOUR_FireBulletPostDataUpdate(DataUpdateType_t updateType);
	extern bool __stdcall CreateMove(float flInputSampleTime, CUserCmd* cmd);
	extern void __stdcall FrameStageNotify(ClientFrameStage_t stage);
	extern void __fastcall PaintTraverse(void* ecx, void* edx, unsigned int panel, bool forceRepaint, bool allowForce);
	extern void __stdcall OverrideView(CViewSetup* vsView);
	extern void __stdcall DrawModelExecute(void* context, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	extern void __fastcall DrawHuso(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags);
	extern void __fastcall TraceRay(void* thisptr, void* ecx, Ray_t const& ray, unsigned int mask, CTraceFilter* filter, trace_t& trace);
	extern void __fastcall RunCommand(void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* moveHelper);
	extern float __fastcall GetLastTimeStamp(void* ecx, void*);
	extern bool __stdcall IsConnected();
	extern bool __fastcall IsPlayingTimeDemo(void* thisptr, void* edx);
	extern bool __fastcall FireEvent(void*, void*, IGameEvent* pEvent);
	extern MDLHandle_t __fastcall FindMDL(void* ecx, void* edx, char* FilePath);
	extern HRESULT __stdcall EndScene(IDirect3DDevice9* pDevice);
	extern HRESULT __stdcall Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	extern HRESULT __stdcall Present(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	extern LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	extern void LowerBodyYawTargetProxy(const CRecvProxyData* pData, void* pStruct, void* pOut);

};
