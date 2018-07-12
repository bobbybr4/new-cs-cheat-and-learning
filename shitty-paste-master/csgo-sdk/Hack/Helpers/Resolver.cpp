#include "../../csgo-sdk.h"

enum CCSGOAnimStatePoses
{
	ACT_CSGO_IDLE_TURN_BALANCEADJUST = 979,
};

void CResolver::Resolve_Nospread(CBaseEntity* pEntity) 
{
	auto record = Backtracking->arr_infos.at(pEntity->GetIndex());

	std::vector<CPredictable> m_predictable;

	float last_lby = 0.0f;
	float last_simtime = 0.0f;

	for (auto &r : record.TickRecords)
	{
		if (GetDelta(r.m_flLowerBodyYawTarget, last_lby) > 0 && GetDelta(r.m_flLowerBodyYawTarget, last_lby) < 35)
			continue;

		if (GetDelta(r.m_flLowerBodyYawTarget, r.m_angEyeAngles.y) > 0 && GetDelta(r.m_flLowerBodyYawTarget, r.m_angEyeAngles.y) < 35)
			continue;

		if (r.m_nSequence == 979) // ACT_CSGO_IDLE_TURN_BALANCEADJUST
		{
			m_predictable.push_back(CPredictable(r.m_flLowerBodyYawTarget, r.m_flSimulationTime));
		}

		last_lby = r.m_flLowerBodyYawTarget;
		last_simtime = r.m_flSimulationTime;
	}

	bool is_breaking_lby = false;

	for (auto c : m_predictable)
	{
		if (IsDifferent(c.m_flSimulationTime, pEntity->GetSimulationTime(), 0.170f) /* 170ms is good for now */)
			continue;

		if (last_simtime < c.m_flSimulationTime)
			continue;

		if (GetDelta(c.m_flLowerBodyYawTarget, pEntity->GetEyeAngles().y) < 90)
			continue;

		is_breaking_lby = true;
		last_lby = c.m_flLowerBodyYawTarget;
	}

	if (last_simtime < 0.2f)
	{
		switch (G::ShotsFired[pEntity->GetIndex()] % 3)
		{
		case 0:	pEntity->m_angEyeAngles().y = last_lby; break;
		case 1:	pEntity->m_angEyeAngles().y = last_lby + Math::RandomFloat(-35, 35); break;
		case 2:	pEntity->m_angEyeAngles().y += G::UserCmd->command_number % 2 ? -last_lby : last_lby; break;
		}
	}
	else
	{
		pEntity->m_angEyeAngles().y += G::ShotsFired[pEntity->GetIndex()] * 90.0f;
	}
	pEntity->m_angEyeAngles().NormalizeYaw();
}

CPredictable::CPredictable(float lby, float sim)
{
	m_flLowerBodyYawTarget = lby;
	m_flSimulationTime = sim;
}

void CResolver::Resolve_Predict(CBaseEntity* pEntity) {
	auto record = Backtracking->arr_infos.at(pEntity->GetIndex());

	std::vector<CPredictable> m_predictable;

	float last_lby = 0.0f;

	studiohdr_t* hdr = Interfaces->ModelInfo->GetStudioModel(pEntity->GetModel());

	for (auto &r : record.TickRecords)
	{
		if (hdr && hdr->pSeqdesc(r.m_nSequence)->activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST) // ACT_CSGO_IDLE_TURN_BALANCEADJUST
		{
			std::string animation = hdr->pSeqdesc(r.m_nSequence)->pszActivityName();
			std::cout << "Resolvable animation for index '" << pEntity->GetIndex() << "' found @ '" << animation << "'\n";
			m_predictable.push_back(CPredictable(r.m_angEyeAngles.y, r.m_flSimulationTime));
		}

		if (GetDelta(r.m_flLowerBodyYawTarget, last_lby) < 35)
			continue;

		if (GetDelta(r.m_flLowerBodyYawTarget, r.m_angEyeAngles.y) < 35)
			continue;

		last_lby = r.m_flLowerBodyYawTarget;
	}

	bool is_breaking_lby = false;

	for (auto c : m_predictable)
	{
		if (IsDifferent(c.m_flSimulationTime, pEntity->GetSimulationTime(), 0.170f) /* 170ms is good for now */)
			continue;

		if (GetDelta(c.m_flLowerBodyYawTarget, pEntity->GetEyeAngles().y) < 90)
			continue;

		is_breaking_lby = true;
		last_lby = c.m_flLowerBodyYawTarget;
	}

	if (is_breaking_lby)
	{
		float offset = 90.0f;

		if (GetDelta(last_lby, pEntity->GetEyeAngles().y) < 270.0f && GetDelta(last_lby, pEntity->GetEyeAngles().y) > 90.0f)
		{
			offset = 180.0f;
		}

		switch (G::ShotsFired[pEntity->GetIndex()] % 4)
		{
		case 0: pEntity->m_angEyeAngles().y += offset; break;
		case 1: pEntity->m_angEyeAngles().y += offset + 35.0f; break;
		case 2: pEntity->m_angEyeAngles().y -= offset + 35.0f; break;
		case 3: pEntity->m_angEyeAngles().y -= offset; break;
		}
	}
	else if (IsDifferent(last_lby, pEntity->GetEyeAngles().y))
	{
		switch (G::ShotsFired[pEntity->GetIndex()] % 3)
		{
		case 0: pEntity->m_angEyeAngles().y = last_lby; break;
		case 1: pEntity->m_angEyeAngles().y = last_lby + 35.0f; break;
		case 2: pEntity->m_angEyeAngles().y = last_lby - 35.0f; break;
		}
	}

	pEntity->m_angEyeAngles().NormalizeYaw();
}

void CResolver::Resolve(CBaseEntity * pEntity) {
	int resolvermode = g_pSettings->RagebotCorrections->SelectedIndex;


	if (pEntity->IsMovingOnGround())
		pEntity->m_angEyeAngles().y = pEntity->m_flLowerBodyYawTarget();
	else {
		switch (resolvermode) {
		case 0: return;
		case RESOLVERMODE_PREDICT:
			Resolve_Predict(pEntity);
			break;
		case RESOLVERMODE_NOSPREAD:
			Resolve_Nospread(pEntity);
			break;
		}
	}
}

void CResolver::Store(CBaseEntity * Entity) {
	if (!G::UserCmd || !Entity || Entity->GetIndex() < 1 || Entity->GetIndex() > 64)
		return;

	auto record = &Backtracking->arr_infos.at(Entity->GetIndex());

	record->Entity = Entity;

	if (record->TickRecords.size() > 1 && record->TickRecords.front().tickcount == G::UserCmd->tick_count)
		return;

	record->TickRecords.push_front(CTickRecord(Entity));

	static ConVar* cmd_rate = Interfaces->Cvar->FindVar("cl_cmdrate");
	
	while ((int)record->TickRecords.size() > 256) 
		record->TickRecords.pop_back();
}
