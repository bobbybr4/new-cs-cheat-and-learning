#pragma once

namespace G
{
	extern CBaseEntity*					LocalPlayer;
	extern CUserCmd*					UserCmd;
	extern HMODULE						Dll;
	extern HWND							Window;
	extern bool							UsingBulletHandler;
	extern std::vector<CBaseEntity*>	EnemyList;
	extern std::vector<CBaseEntity*>	TeamList;
	extern std::string					DebugString;
	extern bool							InAntiAim;
	extern bool							Aimbotting;
	extern bool							SendPacket;
	extern char*						DllPath;
	extern QAngle						VisualAngle;
	extern QAngle						RealAngle;
	extern QAngle						FakeAngle;
	extern int							ShotsFired[256];
	extern bool							ShouldAA;
	extern float						LBY[256];
}
