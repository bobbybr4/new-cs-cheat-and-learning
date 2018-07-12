#pragma once

enum class ResolveMode : int {
	OFF = 0,
	FORCE,
	DELTA,
	STEADY,
	TICKMODULO,
	ALL
};

class CResolveInfo {
	friend class CResolver;
protected:
	std::deque<CTickRecord> m_sRecords;
	bool	m_bEnemyShot; //priority
	bool	m_bLowerBodyYawChanged;
	bool	m_bBacktrackThisTick;
};

#define Resolver CResolver::GetInstance()

enum ResolverMode_t {
	RESOLVERMODE_OFF,
	RESOLVERMODE_PREDICT,
	RESOLVERMODE_NOSPREAD
};

class CResolver {
	IMPLEMENT_SINGLETON(CResolver);
private:
	void Resolve_Nospread(CBaseEntity * pEntity);
	void Resolve_Predict(CBaseEntity* pEntity);
	void Resolve_Smart(CBaseEntity * pEntity);
public:
	void Resolve(CBaseEntity* pEntity);
	void Store(CBaseEntity * Entity);
};

class CPredictable
{
public:
	float m_flLowerBodyYawTarget = 0.0f;
	float m_flSimulationTime = 0.0f;

	CPredictable(float lby, float sim);
};

const inline float GetDelta(float a, float b) {
	return abs(Math::ClampYaw(a - b));
}

const inline float LBYDelta(const CTickRecord& v) {
	return v.m_angEyeAngles.y - v.m_flLowerBodyYawTarget;
}

const inline bool IsDifferent(float a, float b, float tolerance = 10.f) {
	return (GetDelta(a, b) > tolerance);
}
