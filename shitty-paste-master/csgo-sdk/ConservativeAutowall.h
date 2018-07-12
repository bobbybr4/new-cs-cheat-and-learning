#pragma once
#include "csgo-sdk.h"
struct FireBulletDat
{
	Vector src;
	trace_t enter_trace;
	Vector direction;
	CTraceFilter filter;
	float trace_length;
	float trace_length_remaining;
	float current_damage;
	int penetrate_count;
};

class ConservativeAutowall
{
public:
	//Old Awall
	bool HandleBulletPenetration(WeaponInfo_t* pWeaponData, FireBulletDat& BulletData);
	bool SimulateFireBullet(CBaseEntity* pBaseEntity, FireBulletDat& BulletData);
	int PenetrateWall(CBaseEntity* pBaseEntity, const Vector& vecPoint);
}; extern ConservativeAutowall* g_pAutowall;