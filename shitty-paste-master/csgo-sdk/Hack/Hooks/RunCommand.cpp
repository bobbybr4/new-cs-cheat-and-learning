#include "../../csgo-sdk.h"

RunCommandFn oRunCommand;

void SetUpVelocity(CBaseEntity* player);

void __fastcall Hooks::RunCommand(void* ecx, void* edx, CBaseEntity* pEntity, CUserCmd* pCmd, IMoveHelper* moveHelper)
{
	oRunCommand(ecx, edx, pEntity, pCmd, moveHelper);

	SetUpVelocity(pEntity);
}

void SetUpVelocity(CBaseEntity* player)
{
	float flYawDelta = GetDelta(player->GetLowerBodyYawTarget(), player->GetEyeAngles().y);

	INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();
	float flLatency = nci->GetAvgLatency(FLOW_OUTGOING);

	if (player->GetFlags() & FL_ONGROUND)
	{
		if (((Interfaces->Globals->curtime + flLatency) > G::LBY[player->GetIndex()]) || !G::LBY[player->GetIndex()])
		{
			if (flYawDelta > 35.0f)
			{
				if (player->GetVelocity().Length2D() > 0.5f && (flLatency < 0.22000001f))
				{
					G::LBY[player->GetIndex()] = Interfaces->Globals->curtime + (0.22000001f);
				}
				else if (player->GetVelocity().Length2D() < 0.5f && (flLatency < 1.1f))
				{
					G::LBY[player->GetIndex()] = Interfaces->Globals->curtime + (1.1f);
				}
				else
				{
					G::LBY[player->GetIndex()] = Interfaces->Globals->curtime;
				}
			}
		}
	}
}