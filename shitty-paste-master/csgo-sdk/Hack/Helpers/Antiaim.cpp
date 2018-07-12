#include "../../csgo-sdk.h"

#define RANDOM_FLOAT( min, max ) (( min + 1 ) + ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) * ( max - ( min + 1 ) ))

float QAngle::RealYawDifference(QAngle dst, QAngle add) {
	dst += add;

	dst.x = 0.f;

	Vector ang, aim;

	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

float QAngle::Difference(QAngle dst, QAngle add) {
	dst += add;

	Vector ang, aim;

	Math::AngleVectors(*this, aim);
	Math::AngleVectors(dst, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

bool GetBestHeadAngle(QAngle& angle)
{
	Vector position = G::LocalPlayer->GetOrigin() + G::LocalPlayer->GetViewOffset();

	float closest_distance = 100.0f;

	float radius = G::LocalPlayer->GetWeapon()->GetCSWpnData()->m_flRange;
	float step = M_PI * 2.0 / 8;

	for (float a = 0; a < (M_PI * 2.0); a += step)
	{
		Vector location(radius * cos(a) + position.x, radius * sin(a) + position.y, position.z);

		Ray_t ray;
		trace_t tr;
		ray.Init(position, location);
		CTraceFilter traceFilter;
		traceFilter.pSkip = G::LocalPlayer;
		Interfaces->EngineTrace->TraceRay(ray, 0x4600400B, &traceFilter, &tr);

		float distance = Math::VectorDistance(position, tr.endpos);

		if (distance < closest_distance)
		{
			closest_distance = distance;
			angle.y = RAD2DEG(a);
		}
	}

	return closest_distance < (g_pSettings->RagebotEdgeYaw->SelectedIndex == 2) ? G::LocalPlayer->GetWeapon()->GetCSWpnData()->m_flRange : 7.0f;
}

float GetServerTime()
{
	return (float)G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick;
}

float get3ddist2(Vector myCoords, Vector enemyCoords)
{
	return sqrt(
		pow(double(enemyCoords.x - myCoords.x), 2.0) +
		pow(double(enemyCoords.y - myCoords.y), 2.0) +
		pow(double(enemyCoords.z - myCoords.z), 2.0));
}
void AimAtPlayers()
{
	QAngle val;
	Vector eye_position = G::LocalPlayer->GetEyePosition();
	int value = G::UserCmd->viewangles.y, num = 0;
	float best_dist = 8192.f;
	for (int i = 1; i < Interfaces->Globals->maxClients; ++i)
	{
		if (i == Interfaces->EngineClient->GetLocalPlayer())
			continue;

		CBaseEntity* target = Interfaces->ClientEntityList->GetClientEntity(i);
		//player_info_t info;
		//Calls from left->right so we wont get an access violation error
		Vector pos;
		if (!target || /*target->IsDormant() ||*//*bc otherwise we get 1tapped by enemies throug dormant walls*/ target->GetHealth() < 1)
			continue;
		if ((G::LocalPlayer->GetTeam() == target->GetTeam() && g_pSettings->RagebotFriendly->Checked) || G::LocalPlayer->GetTeam() != target->GetTeam())
		{
			pos = target->GetEyePosition();
			float dist = get3ddist2(pos, G::LocalPlayer->GetEyePosition());
			if (g_pSettings->RagebotDynamic->Checked)
			{
				if (dist >= best_dist)
					continue;
			}
			best_dist = dist;

			QAngle angle = Math::CalcAngle(G::LocalPlayer->GetEyePosition(), pos);
			angle.y = Math::NormalizeFloat(angle.y);
			angle.Clamp();

			value = angle.y;
		}
	}

	G::UserCmd->viewangles.y = value;
}

bool CanAttack()
{
	if (!G::LocalPlayer->GetWeapon())
		return false;

	float flNextPrimaryAttack = G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack();

	return flNextPrimaryAttack <= GetServerTime();
}

bool FireChecks()
{
	static bool hasAttacked = false;

	if (G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsGrenade())
	{
		return true;
	}

	if (G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2)
	{
		if (G::LocalPlayer->GetWeapon())
		{
			int local_weapon = G::LocalPlayer->GetWeapon()->GetWeaponID();

			if (local_weapon == WEAPON_KNIFE)
			{
				return true;
			}

			else if (local_weapon == WEAPON_REVOLVER)
			{
				if (G::UserCmd->buttons & IN_ATTACK2)
				{
					return true;
				}
				else
				{
					INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();
					float latency = nci->GetAvgLatency(FLOW_OUTGOING);

					if (!(G::LocalPlayer->GetWeapon()->GetFireReadyTime() - (GetServerTime() - latency) > 0.03f))
					{
						return true;
					}
				}
			}
			
			else if(!(G::UserCmd->buttons & IN_ATTACK2))
			{
				return true;
			}
		}

		else
		{
			return false;
		}
	}

	else if (G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsGrenade())
	{
		//if (G::LocalPlayer->GetWeapon()->GetPinPulled())
		//{
		return true;
		//}
	}

	return false;
}

void TickReal(QAngle &angle)
{
	//	RagebotYaw = new MDropdown(RageConfig, "yaw", "", RageCategory, "necro_rage_y", std::vector<std::string>{"disabled", "180", "jitter", "smooth jitter", "z-jitter", "directional", "spin"});
	if (g_pSettings->RagebotYaw->SelectedIndex == 0)
	{
		return;
	}
	else
	{
		static bool side = false;

		if (G::UserCmd->sidemove > 0)
			side = true;
		else if ((G::UserCmd->sidemove < 0))
			side = false;

		int jitter_factor = g_pSettings->RagebotJitterReal->value;
		int real_angle = g_pSettings->RagebotRealAngle->value;
		int randomized = rand() % 180 - rand() % 50;

		switch (g_pSettings->RagebotYaw->SelectedIndex)
		{
		case 1: angle.y += 180.0f; break;
		case 2: angle.y += 180.0f; angle.y += (G::UserCmd->command_number % 2) ? -(jitter_factor) : jitter_factor; break;
		case 3: {
			angle.y += 180.0f;
			angle.y += Math::RandomFloat(-(jitter_factor), jitter_factor);
		}break;
		case 4: { 
			angle.y += 180.0f;
			int randomized = rand() % jitter_factor + 1; // fuck uc xd
			angle.y += clamp(G::UserCmd->command_number % 2 ? randomized : -randomized, -(jitter_factor), jitter_factor);
		}break;
		case 5:
		{
			angle.y += (side ? 90 : -90);
		}break;
		
		case 6: {
			angle.y += (float)(fmod(Interfaces->Globals->curtime / 1.8f * 720.0f, 360.0f)); 
		} break;
		case 7: angle.y += real_angle; angle.y += (G::UserCmd->command_number % 2) ? -(jitter_factor) : jitter_factor; break;
		case 8: angle.y += G::LocalPlayer->GetLowerBodyYawTarget(); break;
		case 9:	angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + ShouldFake() ? randomized : -randomized; break;
		case 10: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + rand() % 180 - rand() % 50; break;
		case 11: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + 0.01f + Math::RandomFloat(85.f, 100.f); break;
		}
	}
}

void TickFake(QAngle &angle)
{
	if (g_pSettings->RagebotFakeYaw->SelectedIndex == 0)
	{
		return;
	}
	else
	{
		static bool side = false;

		if (G::UserCmd->sidemove > 0)
			side = true;
		else if ((G::UserCmd->sidemove < 0))
			side = false;

		int jitter_fakefactor = g_pSettings->RagebotJitterFake->value;
		int fake_angle = g_pSettings->RagebotFakeAngle->value;
		int randomized = rand() % 180 - rand() % 50;

		switch (g_pSettings->RagebotFakeYaw->SelectedIndex)
		{
		case 1: angle.y += 180.0f; break;
		case 2: angle.y += 180.0f + Math::RandomFloat(-35, 35); break;
		case 3: angle.y += (side ? -90 : 90); break;
		case 4: angle.y += fake_angle; angle.y += (G::UserCmd->command_number % 2) ? -(jitter_fakefactor) : jitter_fakefactor; break;
		case 5: angle.y += G::LocalPlayer->GetLowerBodyYawTarget(); break;
		case 6: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + ShouldFake() ? randomized : -randomized; break;
		case 7: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + rand() % 180 - rand() % 50; break;
		case 8: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + 0.01f + Math::RandomFloat(85.f, 100.f); break;
		}
	}
}

void TickPitch(QAngle &angle)
{
	if (g_pSettings->RagebotPitch->SelectedIndex == 0)
	{
		return;
	}
	else
	{
		switch (g_pSettings->RagebotPitch->SelectedIndex)
		{
		case 1: angle.x = G::LocalPlayer->GetWeapon()->IsSniper() ? (G::LocalPlayer->GetWeapon()->GetZoomLevel() != 0 ? 87.f : 85.f) : 88.99f;
		case 2: angle.x = 88.95f; break;
		case 3: angle.x = -991.0f; break;
		case 4: angle.x = 2160.0f; break;
		}
	}
}

void TickWalking(QAngle &angle)
{
	//	RagebotYaw = new MDropdown(RageConfig, "yaw", "", RageCategory, "necro_rage_y", std::vector<std::string>{"disabled", "180", "jitter", "smooth jitter", "z-jitter", "directional", "spin"});
	if (g_pSettings->RagebotWalkYaw->SelectedIndex == 0)
	{
		return;
	}
	else
	{
		static bool side = false;

		if (G::UserCmd->sidemove > 0)
			side = true;
		else if ((G::UserCmd->sidemove < 0))
			side = false;

		int jitter_walkfactor = g_pSettings->RagebotJitterWalk->value;
		int moving_angle = g_pSettings->RagebotWalkAngle->value;
		int randomized = rand() % 180 - rand() % 50;

		switch (g_pSettings->RagebotWalkYaw->SelectedIndex)
		{
		case 1: angle.y += 180.0f; break;
		case 2: angle.y += 180.0f; angle.y += (G::UserCmd->command_number % 2) ? -(jitter_walkfactor) : jitter_walkfactor; break;
		case 3: {
			angle.y += 180.0f;
			angle.y += Math::RandomFloat(-(jitter_walkfactor), jitter_walkfactor);
		}break;
		case 4: {
			angle.y += 180.0f;
			int randomized = rand() % jitter_walkfactor + 1; // fuck uc xd
			angle.y += clamp(randomized, -(jitter_walkfactor), jitter_walkfactor);
		}break;
		case 5:
		{
			angle.y += (side ? 90 : -90);
		}break;

		case 6: {
			angle.y += (float)(fmod(Interfaces->Globals->curtime / 1.8f * 720.0f, 360.0f));
		} break;
		case 7: angle.y += moving_angle; angle.y += (G::UserCmd->command_number % 2) ? -(jitter_walkfactor) : jitter_walkfactor; break;
		case 8: angle.y += G::LocalPlayer->GetLowerBodyYawTarget(); break;
		case 9: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + ShouldFake() ? randomized : -randomized; break;
		case 10: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + rand() % 180 - rand() % 50; break;
		case 11: angle.y += G::LocalPlayer->GetLowerBodyYawTarget() + 0.01f + Math::RandomFloat(85.f, 100.f); break;
		}
	}
}

bool ShouldFake()
{
	bool will_update = false;
	static float lby_time = 0;

	INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();

	float latency = nci->GetAvgLatency(FLOW_OUTGOING);

	float server_time = Interfaces->Globals->curtime + latency;

	if (server_time >= (lby_time))
	{
		lby_time = server_time + 1.1f;
		will_update = true;
	}

	return will_update;
}

void CAntiAim::Run()
{
	G::InAntiAim = false;

	if (!g_pSettings->RagebotAntiaim->Checked || !G::ShouldAA)
		return;

	bool in_fakewalk = GetAsyncKeyState(g_pSettings->RagebotWalkKey->Key) & 0x8000;

	INetChannelInfo* nci = Interfaces->EngineClient->GetNetChannelInfo();
	float latency = nci->GetAvgLatency(FLOW_OUTGOING);

	bool in_lbyjitter = ShouldFake();

	if (g_pSettings->MiscFakelag->SelectedIndex == 0)
	{
		G::SendPacket = !(G::UserCmd->command_number % 3) ? true : false;
	}

	if (!FireChecks() || !CanAttack())
	{
		if (!in_fakewalk && !in_lbyjitter && !G::SendPacket && g_pSettings->RagebotDynamic->Checked)
			AimAtPlayers();

		QAngle angle = G::UserCmd->viewangles;

		TickPitch(angle);

		if (!in_fakewalk && G::LocalPlayer->GetVelocity().Length() < 0.5 && g_pSettings->RagebotEdgeYaw->SelectedIndex)
			GetBestHeadAngle(angle);

		if ((G::LocalPlayer->GetVelocity().Length() < 0.5) && (G::LocalPlayer->GetFlags() & FL_ONGROUND)
			|| !(G::LocalPlayer->GetFlags() & FL_ONGROUND)
			|| in_fakewalk)
		{
			if (G::SendPacket) // fake
			{
				if (in_fakewalk || in_lbyjitter)
				{
					TickFake(angle);
					TickReal(angle);
				}
				else
				{
					TickReal(angle);
					TickFake(angle);
				}
			}
			else // real
			{
				if (!(in_fakewalk || in_lbyjitter))
				{
					TickReal(angle);
				}
				else
				{
					TickReal(angle);
					TickFake(angle);
				}
			}
		}
		else
		{
			TickWalking(angle);
		}

		G::UserCmd->viewangles = angle;

		G::InAntiAim = true;
	}
}

#undef RANDOM_FLOAT
