#pragma once

#define TIME_TO_TICKS( dt )        ( (int)( 0.5f + (float)(dt) / Interfaces->Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces->Globals->interval_per_tick *( t ) )
#define ROUND_TO_TICKS( t )		( Interfaces->Globals->interval_per_tick * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)


class VarMapEntry_t {
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t {
	VarMapping_t() {
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};


#define Lagcompensation CLagcompensation::GetInstance()

class CLagcompensation {
	friend class CResolver;
	friend class CBacktracking;

	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}

	IMPLEMENT_SINGLETON(CLagcompensation);
public:
	float GetLerpTime() const;

	void DisableInterpolation(CBaseEntity* pEntity) {
		VarMapping_t* map = GetVarMap(pEntity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++) {
			VarMapEntry_t *e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
		}
	}
};


#define LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ( 64.0f * 64.0f )
#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
// Allow 4 units of error ( about 1 / 8 bbox width )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )

#define LC_NONE                0
#define LC_ALIVE            (1<<0)

#define LC_ORIGIN_CHANGED    (1<<8)
#define LC_ANGLES_CHANGED    (1<<9)
#define LC_SIZE_CHANGED        (1<<10)
#define LC_ANIMATION_CHANGED (1<<11)

struct LagCompData
{
	Vector vecOrigin, vecViewoffset, vecVelocity, vecOldOrigin, vecOldViewoffset, vecOldVelocity;
	QAngle vecAngles, vecOldAngles;
	int iFlags, iOldFlags;
	float flSimulationTime, flAnimTime, flOldSimulationTime, flOldAnimTime;
	bool IsBackTracked;
};

class ResolverLagComp
{
public:
	// FrameStageNotify
	void BeginLagCompensation(CBaseEntity* pEntity);
	void FinishLagCompensation(CBaseEntity* pEntity);

	// Createmove Lag Comp
	void AdjustTickCountForCmd(CBaseEntity* pEntity, CUserCmd* pCmd);

private:
	// Lag Comp
	bool BacktrackPlayer(CBaseEntity* pLocalEntity, CBaseEntity* pEntity);
	void FailedBackTrack(CBaseEntity* pEntity);
	bool ChokedPackets(CBaseEntity* pEntity);

	// Lag Comp Data
	bool SaveNetvars(CBaseEntity* Entity);
	bool bTickIsValid(int tick);
	void SaveOldNetvars(CBaseEntity* Entity);
	void RestoreNetvars(CBaseEntity *Entity);
	float GetEstimatedServerTime(CBaseEntity* pLocal);
	float GetNetworkLatency();
	int GetEstimatedServerTickCount(float latency);

	// Anim Time
	//void RecvProxy_AnimTime(BaseEntity* pEntity);
	//float GetAnimTime(BaseEntity* pEntity);
};

extern std::map<int, LagCompData> PlayerLagCompData;
extern ResolverLagComp* LagCompResolver;