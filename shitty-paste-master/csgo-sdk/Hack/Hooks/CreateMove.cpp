#include "../../csgo-sdk.h"

void ClampUserCmd(CUserCmd* pCmd) {
	pCmd->forwardmove = std::clamp(pCmd->forwardmove, -450.f, 450.f);
	pCmd->sidemove = std::clamp(pCmd->sidemove, -450.f, 450.f);
	pCmd->upmove = std::clamp(pCmd->upmove, -320.f, 320.f);
	//pCmd->viewangles.Clamp();
}

CreateMoveFn oCreateMove;
bool __stdcall Hooks::CreateMove(float flInputSampleTime, CUserCmd* cmd) {
	uintptr_t* framePointer;
	__asm mov framePointer, ebp;
	QAngle org_angle = cmd->viewangles;

	G::RealAngle = cmd->viewangles;

	if (!cmd->command_number || !Interfaces->EngineClient->IsInGame() || !Interfaces->EngineClient->IsConnected())
		return oCreateMove(flInputSampleTime, cmd);

	G::LocalPlayer = Interfaces->ClientEntityList->GetClientEntity(Interfaces->EngineClient->GetLocalPlayer());
	G::UserCmd = cmd;
	G::SendPacket = true;

	if (!G::LocalPlayer)
		return oCreateMove(flInputSampleTime, cmd);

	Util::BuildEntityLists();

	Miscellaneous->Tick();
	QAngle wish_angle = cmd->viewangles;
	cmd->viewangles = org_angle;

/*	for (auto i = 0; i < 64; ++i)
	{
		CBaseEntity* player = Interfaces->ClientEntityList->GetClientEntity(i);

		if (!player || player == G::LocalPlayer)
			continue;

		if (player->IsDormant())
			continue;

		if (player->GetImmune())
			continue;

		if (!player->IsAlive())
			continue;

		if (!player->IsEnemy() && g_pSettings->RagebotFriendly->Checked)
			continue;

		if(g_pSettings->RagebotPositionAdjustment->SelectedIndex == 2)
			Backtracking->GetCorrectionTick(player, 14);
	}*/

	EnginePrediction->StartPrediction();

	if (g_pSettings->RagebotEnabled->Checked)
	{
		if (G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->GetWeaponID() == WEAPON_REVOLVER
			&& g_pSettings->RagebotRevolver->SelectedIndex == 1)
		{
			INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();

			float latency = nci->GetAvgLatency(FLOW_OUTGOING);

			if ((G::LocalPlayer->GetWeapon()->GetFireReadyTime() - (GetServerTime() + latency) > 0.05f))
				cmd->buttons |= IN_ATTACK;
		}

		Aimbot->Tick();

		if (!g_pSettings->RagebotSilent->Checked)
			Interfaces->EngineClient->SetViewAngles(cmd->viewangles);

		if (cmd->buttons & IN_ATTACK)
		{
			G::UserCmd->viewangles -= G::LocalPlayer->GetPunch() * 2.f;
		}
	}

	EnginePrediction->EndPrediction();

	Miscellaneous->Fakelag();

	if(g_pSettings->RagebotEnabled->Checked && g_pSettings->RagebotAntiaim->Checked)
		Miscellaneous->AntiAim();

	if (g_pSettings->VisualsEnabled->Checked && g_pSettings->VisualsGrenadeTrajectory->Checked) 
		GrenadePrediction->Tick(G::UserCmd->buttons); 

	if (g_pSettings->RagebotEnabled->Checked && G::Aimbotting && G::UserCmd->buttons & IN_ATTACK)
		*(bool*)(*framePointer - 0x1C) = false;

	*(bool*)(*framePointer - 0x1C) = G::SendPacket;

	if (G::SendPacket || !G::ShouldAA || !G::InAntiAim)
	{
		G::VisualAngle = cmd->viewangles;
	}

	else if (!G::SendPacket)
	{
		G::FakeAngle = cmd->viewangles;
	}

	if (g_pSettings->MiscRestrictions->SelectedIndex > 0)
	{
		cmd->viewangles.Clamp();
		Miscellaneous->FixMovement(wish_angle);
		ClampUserCmd(cmd);
	}

	/*for (auto i = 0; i < 64; ++i)
	{
		CBaseEntity* player = Interfaces->ClientEntityList->GetClientEntity(i);

		if (!player || player == G::LocalPlayer)
			continue;

		if (player->IsDormant())
			continue;

		if (player->GetImmune())
			continue;

		if (!player->IsAlive())
			continue;

		if (!player->IsEnemy() && g_pSettings->RagebotFriendly->Checked)
			continue;

		float best = g_pSettings->RagebotHistoryTicks->value / 1000.0f;

		if (g_pSettings->RagebotPositionAdjustment->SelectedIndex == 2)
			Backtracking->SetCurrentEntity(player);
	}*/

	return false;
}

