#include "../../csgo-sdk.h"

float CLagcompensation::GetLerpTime() const {
	static ConVar* cl_interp = Interfaces->Cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces->Cvar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces->Cvar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces->Cvar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces->Cvar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces->Cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces->Cvar->FindVar("sv_client_max_interp_ratio");

	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();

	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;

	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;

	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;

	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;

	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;

	else
		return v20;
}


std::map<int, LagCompData> PlayerLagCompData;
ResolverLagComp* LagCompResolver;

bool ResolverLagComp::SaveNetvars(CBaseEntity* Entity)
{
	PlayerLagCompData[Entity->GetIndex()].vecAngles = Entity->GetEyeAngles();
	PlayerLagCompData[Entity->GetIndex()].vecOrigin = Entity->GetOrigin();
	PlayerLagCompData[Entity->GetIndex()].vecViewoffset = Entity->GetViewOffset();
	PlayerLagCompData[Entity->GetIndex()].vecVelocity = Entity->GetVelocity();
	PlayerLagCompData[Entity->GetIndex()].iFlags = Entity->GetFlags();
	// Other stuff I was going to add, idk if I should.
	/*vecMinsPreScaled;
	vecMaxsPreScaled;
	sequence;
	cycle;
	weight;
	order;*/
	PlayerLagCompData[Entity->GetIndex()].flSimulationTime = Entity->GetSimulationTime();
	//PlayerLagCompData[Entity->GetIndex()].flAnimTime = PlayerLagCompData[Entity->GetIndex()].flAnimTime;
	// Was going 2 do animtime to get correct tick, might do later
	//RecvProxy_AnimTime(Entity);
	return true;
}

void ResolverLagComp::SaveOldNetvars(CBaseEntity* Entity)
{
	PlayerLagCompData[Entity->GetIndex()].vecOldAngles = PlayerLagCompData[Entity->GetIndex()].vecAngles;
	PlayerLagCompData[Entity->GetIndex()].vecOldOrigin = PlayerLagCompData[Entity->GetIndex()].vecOrigin;
	PlayerLagCompData[Entity->GetIndex()].vecOldViewoffset = PlayerLagCompData[Entity->GetIndex()].vecViewoffset;
	PlayerLagCompData[Entity->GetIndex()].vecOldVelocity = PlayerLagCompData[Entity->GetIndex()].vecVelocity;
	PlayerLagCompData[Entity->GetIndex()].iOldFlags = PlayerLagCompData[Entity->GetIndex()].iFlags;
	PlayerLagCompData[Entity->GetIndex()].flOldSimulationTime = PlayerLagCompData[Entity->GetIndex()].flSimulationTime;
	//PlayerLagCompData[Entity->GetIndex()].flOldAnimTime = PlayerLagCompData[Entity->GetIndex()].flAnimTime;
}

void ResolverLagComp::RestoreNetvars(CBaseEntity *Entity)
{
	Entity->GetViewOffset() = PlayerLagCompData[Entity->GetIndex()].vecOldViewoffset;
	Entity->GetVelocity() = PlayerLagCompData[Entity->GetIndex()].vecOldVelocity;
	Entity->GetOrigin() = PlayerLagCompData[Entity->GetIndex()].vecOldOrigin;
	*(int*)(DWORD(Entity) + offsets->m_fFlags) = PlayerLagCompData[Entity->GetIndex()].iOldFlags;
}

// Thank Polak (Aimware)
int ResolverLagComp::GetEstimatedServerTickCount(float latency)
{
	return (int)floorf((float)((float)(latency) / (float)((uintptr_t)&Interfaces->Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&Interfaces->Globals->tickcount);
}

float ResolverLagComp::GetEstimatedServerTime(CBaseEntity* pLocal)
{
	return pLocal->GetTickBase() * Interfaces->Globals->interval_per_tick;
}

// Thank Source SDK & Sharklaser
float ResolverLagComp::GetNetworkLatency()
{
	INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		//float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return IncomingLatency;// +OutgoingLatency;
	}
	return 0.0f;
}

bool ResolverLagComp::bTickIsValid(int tick)
{
	float latency = GetNetworkLatency(), m_flLerpTime = Interfaces->Cvar->FindVar("cl_interp_ratio")->GetFloat() / Interfaces->Cvar->FindVar("cl_updaterate")->GetFloat();
	const float SV_MAXUNLAG = 1.0f;
	int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
	tick += lerpTicks;
	float correct = std::clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
	int targettick = tick - lerpTicks;
	float deltaTime = correct - TICKS_TO_TIME(GetEstimatedServerTickCount(latency) - targettick);

	if (fabs(deltaTime) > 0.2f)
	{
		//targettick = Interfaces::Globals->tickcount - TIME_TO_TICKS(correct);
		return false;
	}
	return true;
}

void ResolverLagComp::AdjustTickCountForCmd(CBaseEntity* pEntity, CUserCmd* pCmd)
{
	if (PlayerLagCompData[pEntity->GetIndex()].IsBackTracked) {
		pCmd->tick_count = TIME_TO_TICKS(PlayerLagCompData[pEntity->GetIndex()].flOldSimulationTime);
		PlayerLagCompData[pEntity->GetIndex()].IsBackTracked = false;
	}
}

void ResolverLagComp::BeginLagCompensation(CBaseEntity* pEntity)
{
	if (!SaveNetvars(pEntity))
		return;

	if (abs(pEntity->GetVelocity().Length()) >= 0.50)
		SaveOldNetvars(pEntity);

	if (!BacktrackPlayer(G::LocalPlayer, pEntity))
		FailedBackTrack(pEntity);
}

bool ResolverLagComp::ChokedPackets(CBaseEntity* pEntity)
{
	if (TIME_TO_TICKS(fabs(pEntity->GetSimulationTime() - pEntity->GetOldSimulationTime())) >= 3 && abs(pEntity->GetVelocity().Length()) >= 0.50)
	{
		Vector LagPredictionOrigin{};
		LagPredictionOrigin -= pEntity->GetAbsOrigin();
		LagPredictionOrigin += pEntity->GetOrigin();
		pEntity->GetOrigin() = LagPredictionOrigin;
		return true;
	}
	return false;
}

//Restores player back to most recent server update if possible
bool ResolverLagComp::BacktrackPlayer(CBaseEntity* pLocalEntity, CBaseEntity* pEntity)
{
	int tickcount = Interfaces->Globals->tickcount;
	bool TickIsValid = bTickIsValid(tickcount);
	float flCurrentTime = (GetEstimatedServerTime(pLocalEntity));

	Vector delta = PlayerLagCompData[pEntity->GetIndex()].vecOrigin - PlayerLagCompData[pEntity->GetIndex()].vecOldOrigin;

	if (delta.Length2DSqr() < 4096 && abs(pEntity->GetVelocity().Length()) >= 0.50)
	{
		if (fabs(PlayerLagCompData[pEntity->GetIndex()].flSimulationTime - flCurrentTime) > 1.0f)
			return false;

		if (TickIsValid)
		{
			RestoreNetvars(pEntity);
			ChokedPackets(pEntity);
			PlayerLagCompData[pEntity->GetIndex()].IsBackTracked = true;
			return true;
		}
		return false;
	}
	else if (!ChokedPackets(pEntity))
		return false;
}

void ResolverLagComp::FailedBackTrack(CBaseEntity* pEntity)
{
	ChokedPackets(pEntity);
}

//Called on new frame to backtrack player if possible
void ResolverLagComp::FinishLagCompensation(CBaseEntity* pEntity)
{
	if (abs(pEntity->GetVelocity().Length()) >= 0.50)
	{
		pEntity->GetViewOffset() = PlayerLagCompData[pEntity->GetIndex()].vecViewoffset;
		pEntity->GetVelocity() = PlayerLagCompData[pEntity->GetIndex()].vecVelocity;
		pEntity->GetOrigin() = PlayerLagCompData[pEntity->GetIndex()].vecOrigin;
		*(int*)(DWORD(pEntity) + offsets->m_fFlags) = PlayerLagCompData[pEntity->GetIndex()].iFlags;
	}
}
