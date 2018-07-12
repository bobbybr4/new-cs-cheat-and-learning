#include "../csgo-sdk.h"

CBaseEntity*				G::LocalPlayer;
CUserCmd*					G::UserCmd;
HMODULE						G::Dll;
HWND						G::Window;
bool						G::UsingBulletHandler;
std::vector<CBaseEntity*>	G::EnemyList(32);
std::vector<CBaseEntity*>	G::TeamList(32);
std::string					G::DebugString;
bool						G::InAntiAim;
bool						G::Aimbotting;
bool						G::SendPacket;
char*						G::DllPath;
QAngle						G::VisualAngle;
int							G::ShotsFired[256];
QAngle						G::RealAngle;
QAngle						G::FakeAngle;
bool						G::ShouldAA = true;
float						G::LBY[256];