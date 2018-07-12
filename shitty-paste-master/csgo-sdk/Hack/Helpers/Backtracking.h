#pragma once

struct CTickRecord {
	CTickRecord() {}
	CTickRecord(CBaseEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYawTarget();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flCycle = ent->GetCycle();
		m_flSimulationTime = ent->GetSimulationTime();
		m_nSequence = ent->GetSequence();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		m_flPoseParameter = ent->GetPoseParameters();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		//m_flDuckSpeed = ent->GetDuckSpeed();
		//m_flDuckAmount = ent->GetDuckAmount();
	}

	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}

	bool operator>(const CTickRecord& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const CTickRecord& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const CTickRecord& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const CTickRecord& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const CTickRecord& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}

	//float m_flDuckAmount = 0.f;
	//float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
};

struct C_PlayerStored {
	CBaseCombatWeapon* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	QAngle EyeAngles = QAngle(0, 0, 0);
	CTickRecord ValidTick = CTickRecord();
};

struct CBacktrackInfo {
	CBacktrackInfo() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord()) {}

	bool shot = false;
	C_PlayerStored player = C_PlayerStored();
	int tick_count = 0;
	CTickRecord temporaryRecord = CTickRecord();


	CBaseEntity* Entity;
	std::deque<CTickRecord> TickRecords;
};

struct LagRecord
{
	Vector m_vecVelocity;
	Vector m_vAbsOrigin;
	QAngle m_vecAngles;
	Vector m_eyeAngles;
	int m_iTargetID;
	int m_nSequence;
	int m_iWantedTick;
	int m_iBacktrackedTick;
	float m_fSimulationTime;
	float m_flCycle;
	std::array<float, 24> m_flPoseParameter;
	float m_flLowerBodyYawTarget;
	unsigned char flags;
	bool m_bIsBreakingLagComp;
	bool m_bIsFixed;
	Vector headSpot;

	LagRecord()
	{
		m_iTargetID = -1;
		m_fSimulationTime = -1;
		m_iWantedTick = -1;
		m_bIsBreakingLagComp = false;
		m_bIsFixed = false;
		m_iBacktrackedTick = -1;
		m_vAbsOrigin = Vector(0, 0, 0);
		m_flCycle = -1;
		m_nSequence = -1;
		m_flLowerBodyYawTarget = 0;
		m_flPoseParameter[24] = 0;
		m_vecAngles = QAngle(0, 0, 0);
		flags = -1;
		m_vecVelocity = Vector(0, 0, 0);
		m_eyeAngles = Vector(0, 0, 0);
		headSpot = Vector(0, 0, 0);
	}

	LagRecord(const LagRecord &src)
	{
		m_flCycle = src.m_flCycle;
		m_nSequence = src.m_nSequence;
		m_iTargetID = src.m_iTargetID;
		m_vecAngles = src.m_vecAngles;
		m_fSimulationTime = src.m_fSimulationTime;
		m_iWantedTick = src.m_iWantedTick;
		m_bIsBreakingLagComp = src.m_bIsBreakingLagComp;
		m_bIsFixed = src.m_bIsFixed;
		m_vAbsOrigin = src.m_vAbsOrigin;
		m_iBacktrackedTick = src.m_iBacktrackedTick;
		flags = src.flags;
		m_flLowerBodyYawTarget = src.m_flLowerBodyYawTarget;
		m_flPoseParameter[24] = src.m_flPoseParameter[24];
		m_vecVelocity = src.m_vecVelocity;
		m_eyeAngles = src.m_eyeAngles;
		headSpot = src.headSpot;
	}
};

#define Backtracking CBacktracking::GetInstance()

class CBacktracking {
	friend class CLagcompensation;
	friend class CResolver;
	friend class CVisuals;

	IMPLEMENT_SINGLETON(CBacktracking); 
public:
		LagRecord m_LagRecord[64][11] = {};
		LagRecord m_PrevRecords[64] = {};
public:		
	void LogEntity(CBaseEntity* player);
	void LogCurrentEntity(CBaseEntity* player);
	void SetEntity(CBaseEntity* player, LagRecord record);
	void SetCurrentEntity(CBaseEntity* player);
	int GetCorrectionTick(CBaseEntity* player, int history);
	void InitializeLagRecord();
	int FixTickCount(CBaseEntity* player);
	void Think(ClientFrameStage_t stage);

	bool bTickIsValid(int tick);

private:
	std::array<CBacktrackInfo, 64> arr_infos;
};
