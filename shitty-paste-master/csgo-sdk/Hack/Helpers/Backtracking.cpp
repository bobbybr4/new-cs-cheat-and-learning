#include "../../csgo-sdk.h"

/*CBacktracking::CBacktracking()
{
	Backtracking->InitializeLagRecord();
}*/

bool CBacktracking::bTickIsValid(int tick) {
	static auto cl_interp_ratio = Interfaces->Cvar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = Interfaces->Cvar->FindVar("cl_updaterate");

	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();

	if (g_pSettings->RagebotPositionAdjustment->SelectedIndex)
		tick += TIME_TO_TICKS(m_flLerpTime);

	return (fabsf(TICKS_TO_TIME(Interfaces->Globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.20f);
}

bool IsValidTick(float simTime)
{
	INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();

	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(Lagcompensation->GetLerpTime());

	int predCmdArrivTick = Interfaces->Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(Lagcompensation->GetLerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(Lagcompensation->GetLerpTime())));

	return abs(flCorrect) <= 0.2f;
}


void CBacktracking::LogEntity(CBaseEntity* player)
{
	int m_iIndex = player->GetIndex();

	LagRecord* m_LagRecords = this->m_LagRecord[m_iIndex];
	
	if (!player || !player->IsAlive()) // invalidate his simtime
	{
		for (auto i = 0; i < 11; ++i)
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}

	float m_flSimulationTime = player->GetSimulationTime();

	//if (!IsValidTick(m_flSimulationTime))
		//return;
	
	int m_iHighestIndex = 1;
	float m_flHighestSimulationTime = 0.0f;

	for (auto i = 0; i < 11; ++i)
	{
		if (m_LagRecords[i].m_fSimulationTime > m_flSimulationTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;

		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;

		if (m_LagRecords[i].m_fSimulationTime == m_flSimulationTime)
			return;

		if (m_LagRecords[i].m_fSimulationTime > m_flHighestSimulationTime)
		{
			m_iHighestIndex = i;
			m_flHighestSimulationTime = m_LagRecords[i].m_fSimulationTime;
		}
	}

	m_iHighestIndex++;
	m_iHighestIndex = m_iHighestIndex % 11;

	m_LagRecords[m_iHighestIndex].m_bIsFixed = false;
	m_LagRecords[m_iHighestIndex].m_iTargetID = m_iIndex;

	m_LagRecords[m_iHighestIndex].headSpot = player->GetBonePosition(8);

	m_LagRecords[m_iHighestIndex].m_vAbsOrigin = player->GetAbsOrigin();
	m_LagRecords[m_iHighestIndex].m_vecVelocity = player->GetVelocity();
	m_LagRecords[m_iHighestIndex].m_fSimulationTime = player->GetSimulationTime();
	m_LagRecords[m_iHighestIndex].m_vecAngles = player->GetAbsAngles();
	m_LagRecords[m_iHighestIndex].m_flCycle = player->GetCycle();
	m_LagRecords[m_iHighestIndex].m_nSequence = player->GetSequence();
	m_LagRecords[m_iHighestIndex].flags = player->GetFlags();
	m_LagRecords[m_iHighestIndex].m_flLowerBodyYawTarget = player->GetLowerBodyYawTarget();

	m_LagRecords[m_iHighestIndex].m_flPoseParameter = player->GetPoseParameters();
}	

void CBacktracking::LogCurrentEntity(CBaseEntity* player)
{
	int m_iIndex = player->GetIndex();

	this->m_PrevRecords[m_iIndex].m_vAbsOrigin = player->GetAbsOrigin();
	this->m_PrevRecords[m_iIndex].m_vecVelocity = player->GetVelocity();
	this->m_PrevRecords[m_iIndex].m_fSimulationTime = player->GetSimulationTime();
	this->m_PrevRecords[m_iIndex].m_vecAngles = player->GetAbsAngles();
	this->m_PrevRecords[m_iIndex].m_flCycle = player->GetCycle();
	this->m_PrevRecords[m_iIndex].m_nSequence = player->GetSequence();
	this->m_PrevRecords[m_iIndex].flags = player->GetFlags();
	this->m_PrevRecords[m_iIndex].m_flLowerBodyYawTarget = player->GetLowerBodyYawTarget();
	this->m_PrevRecords[m_iIndex].m_flPoseParameter = player->GetPoseParameters();
}

void CBacktracking::SetEntity(CBaseEntity* player, LagRecord record)
{
	/* putmeinthedirtwhereibelong */

	if (!player || record.m_fSimulationTime == 0.0f)
	{
		record.m_bIsFixed = false;
		return;
	}

	this->LogCurrentEntity(player);

	player->SetAbsOrigin(record.m_vAbsOrigin);
	*(Vector*)((DWORD)player + offsets->m_vecVelocity) = record.m_vecVelocity;
	*(float*)((DWORD)player + offsets->m_flSimulationTime) = record.m_fSimulationTime;
	player->SetAbsAngles(record.m_vecAngles);
	*(float*)((DWORD)player + offsets->m_flCycle) = record.m_flCycle;
	//*(int*)((DWORD)player + offsets->m_nSequence) = record.m_nSequence;
	*(int*)((DWORD)player + offsets->m_fFlags) = record.flags;
	//*(float*)((DWORD)player + offsets->m_flLowerBodyYawTarget) = record.m_flLowerBodyYawTarget;

	player->SetPoseParameters(record.m_flPoseParameter);

	record.m_iWantedTick = record.m_fSimulationTime;
	record.m_bIsFixed = true;
}

void CBacktracking::SetCurrentEntity(CBaseEntity* player)
{
	int m_iIndex = player->GetIndex(); // [m_iIndex]

	player->SetAbsOrigin(m_PrevRecords[m_iIndex].m_vAbsOrigin);
	*(Vector*)((DWORD)player + offsets->m_vecVelocity) = m_PrevRecords[m_iIndex].m_vecVelocity;
	*(float*)((DWORD)player + offsets->m_flSimulationTime) = m_PrevRecords[m_iIndex].m_fSimulationTime;
	player->SetAbsAngles(m_PrevRecords[m_iIndex].m_vecAngles);
	*(float*)((DWORD)player + offsets->m_flCycle) = m_PrevRecords[m_iIndex].m_flCycle;
	//*(int*)((DWORD)player + offsets->m_nSequence) = m_PrevRecords[m_iIndex].m_nSequence;
	*(int*)((DWORD)player + offsets->m_fFlags) = m_PrevRecords[m_iIndex].flags;
	//*(float*)((DWORD)player + offsets->m_flLowerBodyYawTarget) = m_PrevRecords[m_iIndex].m_flLowerBodyYawTarget;

	player->SetPoseParameters(m_PrevRecords[m_iIndex].m_flPoseParameter);

	m_PrevRecords[m_iIndex].m_iWantedTick = m_PrevRecords[m_iIndex].m_fSimulationTime;
}

float GetLatency()
{
	INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();
	if (nci)
	{
		return nci->GetAvgLatency(FLOW_INCOMING);
	}
	return 0.0f;
}

int GetChokedAmt(CBaseEntity* pEntity)
{
	float flSimulationTime = pEntity->GetSimulationTime();
	float flSimDiff = Interfaces->Globals->curtime - flSimulationTime;
	float latency = GetLatency();
	return TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
}

int CBacktracking::GetCorrectionTick(CBaseEntity* player, int history)
{
	int m_iIndex = player->GetIndex();

	LagRecord* m_LagRecords = this->m_LagRecord[m_iIndex];

	LagRecord recentLR = m_LagRecords[history], prevLR;

	if (history == 0)
	{
		prevLR = m_LagRecords[8];
	}
	else
	{
		prevLR = m_LagRecords[history - 1];
	}

	if (recentLR.m_fSimulationTime == 0.0f)
	{
		printf("StartLagCompensation: invalid simtime for entity %i.\n", m_iIndex);
		return -1;
	}

	INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();
	
	int predicted_cmd_arrival = G::UserCmd->tick_count + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING)); // cred 2 kidua
	
	int lerp_ticks = TIME_TO_TICKS(Lagcompensation->GetLerpTime());
	int tick_count = TIME_TO_TICKS(recentLR.m_fSimulationTime) +lerp_ticks;

	ConVar *sv_max_usercmd_future_ticks = Interfaces->Cvar->FindVar("sv_max_usercmd_future_ticks");

	float correction = clamp(Lagcompensation->GetLerpTime() +  nci->GetLatency(FLOW_OUTGOING), 0.0f, 1.0f) - TICKS_TO_TIME(predicted_cmd_arrival + TIME_TO_TICKS(Lagcompensation->GetLerpTime())) - tick_count;

	if (fabs(correction) > 0.2f)
	{
		printf("StartLagCompensation: delta too big (%.3f)\n", correction); // haha! you got me! i'm big paster!
		return -1;
	}

	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).LengthSqr() > 4096.0f)
	{
		for (auto i = 0; i < GetChokedAmt(player); ++i)
		{
			//g_pPlayerSimulation->Think(); // fuck it i spent too much time on this
		}

		recentLR.m_bIsBreakingLagComp = true;

		return -1;
	}
	else
	{
		SetEntity(player, recentLR);
	}

	return tick_count;
}

void CBacktracking::InitializeLagRecord()
{
	for (int i = 0; i <= 32; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			m_LagRecord[i][j].m_fSimulationTime = 0.0f;
		}
	}
}

int CBacktracking::FixTickCount(CBaseEntity* player)
{
	int m_iIndex = player->GetIndex();

	LagRecord *m_LagRecords = this->m_LagRecord[m_iIndex];
	LagRecord recentLR = m_LagRecords[14];

	if (recentLR.m_fSimulationTime == 0.0f)
	{
		return TIME_TO_TICKS(player->GetSimulationTime() + Lagcompensation->GetLerpTime()) + 1;
	}

	int lerp_ticks = TIME_TO_TICKS(Lagcompensation->GetLerpTime());
	int tick_count = TIME_TO_TICKS(recentLR.m_fSimulationTime)+lerp_ticks; 

	return tick_count + 1;
}

void CBacktracking::Think(ClientFrameStage_t stage)
{
	if (!Interfaces->EngineClient->IsConnected() || !Interfaces->EngineClient->IsInGame())
		return;

	if (!G::LocalPlayer)
		return;

	if (g_pSettings->RagebotPositionAdjustment->SelectedIndex == 2)
	{
		if (stage == FRAME_RENDER_START)
		{
			for (auto i = 0; i < 64; ++i)
			{
				CBaseEntity* player = Interfaces->ClientEntityList->GetClientEntity(i);

				if (!player)
					continue;

				if (player->IsDormant())
					continue;

				if (player->GetImmune())
					continue;

				if (!player->IsAlive())
					continue;

				if (!player->IsEnemy() && g_pSettings->RagebotFriendly->Checked)
					continue;

				LogEntity(player);
			}
		}
	}
}