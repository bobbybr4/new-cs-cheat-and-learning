#include "../../csgo-sdk.h"

float GetNetworkLatency()
{
	INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();
	if (nci)
	{
		return nci->GetAvgLatency(FLOW_INCOMING);
	}
	return 0.0f;
}

//Find out how many ticks player is choking
int GetNumberOfTicksChoked(CBaseEntity* pEntity)
{
	float flSimulationTime = pEntity->GetSimulationTime();
	float flSimDiff = Interfaces->Globals->curtime - flSimulationTime;
	float latency = GetNetworkLatency();
	return TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
}

void CAimbot::Tick() {
	if (!G::LocalPlayer || G::LocalPlayer->GetWeapon() == nullptr || !G::UserCmd)
		return;

	if (g_pSettings->RagebotKey->Key && !GetAsyncKeyState(g_pSettings->RagebotKey->Key))
		return;

	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));

	if (!G::LocalPlayer->GetWeapon()->IsGun() || G::LocalPlayer->GetWeapon()->IsEmpty())
		return;

	if (m_bestent && !G::LocalPlayer->GetWeapon()->IsReloading() && can_shoot) {
		if (g_pSettings->RagebotAutoStop->Checked) {
			Vector velocity = G::LocalPlayer->GetVelocity();
			QAngle direction;
			Math::VectorAngles(velocity, direction);
			float speed = velocity.Length();

			direction.y = G::UserCmd->viewangles.y - direction.y;
			Vector dir;
			Math::AngleVectors(direction, dir);

			Vector negated_direction = (dir) * -speed;

			G::UserCmd->forwardmove = negated_direction.x;
			G::UserCmd->sidemove = negated_direction.y;
		}

		if (g_pSettings->RagebotAutoCrouch->Checked)
			G::UserCmd->buttons |= IN_DUCK;
	}
	if (can_shoot) {
		FindTarget();

		if (!m_bestent)
			return;

		if (g_pSettings->RagebotAutoScope->Checked && m_bestent && G::LocalPlayer->GetWeapon()->IsSniper() && G::LocalPlayer->GetWeapon()->GetZoomLevel() == 0) {
			G::UserCmd->buttons |= IN_ATTACK2;
			return;
		}

		if (g_pSettings->RagebotDelayShot->Checked && (GetNumberOfTicksChoked(m_bestent) > 3))
			return;

		if (m_bestent && G::LocalPlayer->GetWeapon()->IsGun() && !m_besthitbox.Empty())
			GoToTarget();

		if (G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2) 
		{
			if (m_bestent) 
			{
				auto tartime = TIME_TO_TICKS(m_bestent->GetSimulationTime() + Lagcompensation->GetLerpTime());

				if (g_pSettings->RagebotPositionAdjustment->SelectedIndex == 1)
				{		
					if (Backtracking->bTickIsValid(tartime))
						G::UserCmd->tick_count = tartime;
				}

				else if (g_pSettings->RagebotPositionAdjustment->SelectedIndex == 2)
				{
					LagCompResolver->AdjustTickCountForCmd(m_bestent, G::UserCmd);
				}
			}
		}
	}

}

void CAimbot::FindTarget() {
	m_besthitbox = Vector(0, 0, 0);
	m_bestent = nullptr;
	m_bestdmg = -1;

	for (const auto& Entity : G::EnemyList) {
		if(!EntityIsValid(Entity))
			continue;

		//Backtracking->ShotBackTrackBeforeAimbot(Entity);

		Vector hitbox = Entity->GetBonePosition((g_pSettings->RagebotPreferHitbox->SelectedIndex));
		{
			float bestfov = g_pSettings->RagebotFOV->value;
			QAngle va;
			Interfaces->EngineClient->GetViewAngles(va);
			float fov = Math::GetFov(va, Math::CalcAngle(G::LocalPlayer->GetEyePosition(), hitbox));

			if (fov < bestfov) 
			{
				int tmpdmg;

				if(g_pSettings->RagebotHead->Checked)
					tmpdmg = BestAimPointAll(Entity, m_besthitbox);

				else if (g_pSettings->RagebotMultipoint->Checked)
					tmpdmg = BestAimPointHitbox(Entity, m_besthitbox);

				else
					tmpdmg = static_cast<int>(Autowall->GetDamage(hitbox));

				if ((tmpdmg > g_pSettings->RagebotMinimumDamage->value || tmpdmg > Entity->GetHealth()) && tmpdmg > m_bestdmg) 
				{
					m_bestdmg = tmpdmg;
					m_bestent = Entity;
					bestfov = fov;

					if (!g_pSettings->RagebotMultipoint->Checked)
						m_besthitbox = hitbox;

					if (tmpdmg >= Entity->GetHealth())
						break;
				}
			}
		}

		//Backtracking->ShotBackTrackAimbotEnd(Entity);
	}
}

void CAimbot::GoToTarget() {
	bool auto_shoot = false;

	CBaseCombatWeapon* weapon = G::LocalPlayer->GetWeapon();
	bool can_shoot = (weapon->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	static CBaseCombatWeapon* oldWeapon;
	if (weapon != oldWeapon) {
		oldWeapon = weapon;
		G::UserCmd->buttons &= ~IN_ATTACK;
		return;
	}
	
	if (weapon->IsPistol() && G::UserCmd->tick_count % 2) {
		static int lastshot;
		if (G::UserCmd->buttons & IN_ATTACK)
			lastshot++;

		if (!G::UserCmd->buttons & IN_ATTACK || lastshot > 1) {
			G::UserCmd->buttons &= ~IN_ATTACK;
			lastshot = 0;
		}
		return;
	}

	m_besthitbox = m_bestent->GetPredicted(m_besthitbox);

	QAngle aim_angle = Math::CalcAngle(G::LocalPlayer->GetEyePosition(), m_besthitbox);

	if (can_shoot && (static_cast<int>(Math::GetHitchance(m_bestent, aim_angle)) >= g_pSettings->RagebotMinimumHitchance->value || G::LocalPlayer->GetWeapon()->GetWeaponID() == WEAPON_REVOLVER)) {
		G::Aimbotting = true;
		G::UserCmd->viewangles = aim_angle;
		auto_shoot = true;
	}
	else {
		G::Aimbotting = false;
	}

	if (auto_shoot && can_shoot && g_pSettings->RagebotAutoFire->Checked)
		if (weapon->GetWeaponID() == WEAPON_REVOLVER)
		{
			if (g_pSettings->RagebotRevolver->SelectedIndex == 2)
			{
				G::UserCmd->buttons |= IN_ATTACK2;
			}
			else
				G::UserCmd->buttons |= IN_ATTACK;
		}
		else
			G::UserCmd->buttons |= IN_ATTACK;

	G::ShotsFired[m_bestent->GetIndex()]++;
}

bool CAimbot::GetHitbox(CBaseEntity* target, Hitbox* hitbox) {
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!target->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
		return false;


	studiohdr_t* hdr = Interfaces->ModelInfo->GetStudioModel(target->GetModel());

	if (!hdr)
		return false;

	mstudiohitboxset_t* hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());
	mstudiobbox_t* untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);

	Vector bbmin = untransformedBox->bbmin;
	Vector bbmax = untransformedBox->bbmax;

	if (untransformedBox->radius != -1.f) {
		bbmin -= Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
		bbmax += Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
	}

	Vector points[9] = { ((bbmin + bbmax) * (g_pSettings->RagebotMinimumMultipoint->value / 100)),
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z) };

	for (int index = 0; index < 9; index++) {
		if (index != 0)
			points[index] = ((((points[index] + points[0]) * (g_pSettings->RagebotMinimumMultipoint->value / 100)) 
				+ points[index]) * (g_pSettings->RagebotMinimumMultipoint->value / 100));

		hitbox->points[index] = Math::VectorTransform(points[index], matrix[untransformedBox->bone]);
	}

	return true;
}bool CAimbot::GetBestPoint(CBaseEntity* target, Hitbox* hitbox, BestPoint* point) {
	if (hitbox->hitbox == HITBOX_HEAD) {
		Vector high = ((hitbox->points[3] + hitbox->points[5]) * .5f);

		float pitch = target->GetEyeAngles().x;
		if ((pitch > 0.f) && (pitch <= 89.f)) {
			Vector height = (((high - hitbox->points[0]) / 3) * 4);
			Vector new_height = (hitbox->points[0] + (height * (pitch / 89.f)));

			hitbox->points[0] = new_height;
		}
	}

	for (int index = 0; index < 9; index++) {
		int temp_damage = static_cast<int>(Autowall->GetDamage(hitbox->points[index]));
		if (point->dmg < temp_damage) {
			point->dmg = temp_damage;
			point->point = hitbox->points[index];
			point->index = index;
			if (temp_damage >= target->GetHealth())
				break;
		}
	}

	return (point->dmg >= g_pSettings->RagebotMinimumDamage->value);
}


int CAimbot::BestAimPointAll(CBaseEntity* target, Vector& hitbox) {
	m_hitboxes.clear();
	if (g_pSettings->RagebotPelvis->Checked)
		m_hitboxes.push_back(HITBOX_PELVIS);
	if (g_pSettings->RagebotChest->Checked)
		m_hitboxes.push_back(HITBOX_CHEST);
	if (g_pSettings->RagebotHead->Checked)
	{
		m_hitboxes.push_back(HITBOX_HEAD);
		m_hitboxes.push_back(HITBOX_LOWER_NECK);
	}
	if (g_pSettings->RagebotArms->Checked) {
		m_hitboxes.push_back(HITBOX_RIGHT_FOREARM);
		m_hitboxes.push_back(HITBOX_LEFT_FOREARM);
		m_hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
		m_hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
		m_hitboxes.push_back(HITBOX_RIGHT_HAND);
		m_hitboxes.push_back(HITBOX_LEFT_HAND);
	}
	if (g_pSettings->RagebotLegs->Checked) {
		m_hitboxes.push_back(HITBOX_RIGHT_CALF);
		m_hitboxes.push_back(HITBOX_LEFT_CALF);
		m_hitboxes.push_back(HITBOX_LEFT_FOOT);
		m_hitboxes.push_back(HITBOX_RIGHT_FOOT);
	}

	BestPoint aim_point;
	int bestdmg = 0, tempdmg = 0;

	Hitbox meme;

	for (auto hitbx : m_hitboxes) {
		Hitbox curhitbox(hitbx);
		if (!GetHitbox(target, &curhitbox))
			continue;
		{
			tempdmg = static_cast<int>(Autowall->GetDamage(curhitbox.points[0]));
			if (tempdmg > bestdmg) {
				bestdmg = tempdmg;
				meme = curhitbox;
				hitbox = curhitbox.points[0];
				if (bestdmg >= target->GetHealth())
					return bestdmg;
			}
		}
	}

	if (g_pSettings->RagebotMultipoint->Checked) {
		GetBestPoint(target, &meme, &aim_point);
		if (aim_point.dmg > g_pSettings->RagebotMinimumDamage->value)
			hitbox = aim_point.point;
		if (aim_point.dmg >= target->GetHealth())
			return aim_point.dmg;
	}

	return bestdmg;
}


int CAimbot::BestAimPointHitbox(CBaseEntity* target, Vector& hitbox) {
	BestPoint aim_point;

	int hitboxnum;

	switch (g_pSettings->RagebotPreferHitbox->SelectedIndex) {
	case 0:
		hitboxnum = HITBOX_PELVIS;
		break;
	case 2:
		hitboxnum = HITBOX_NECK;
		break;
	case 1:
		hitboxnum = HITBOX_BODY;
		break;
	case 3:
		hitboxnum = HITBOX_PELVIS;
		break;
	case 4:
	case 5:
		hitboxnum = HITBOX_BODY;
		break;
	case 6:
	case 7:
		hitboxnum = HITBOX_CHEST;
		break;
	default:
		hitboxnum = HITBOX_HEAD;
	}

	Hitbox hitboxx(hitboxnum);
	if (!GetHitbox(target, &hitboxx))
		return 0;

	if (!GetBestPoint(target, &hitboxx, &aim_point))
		return 0;

	if (aim_point.dmg >= g_pSettings->RagebotMinimumDamage->value) {
		hitbox = aim_point.point;
		return aim_point.dmg;
	}

	return 0;
}

bool CAimbot::EntityIsValid(CBaseEntity* Entity) {
	if (!Entity)
		return false;
	if (Entity == G::LocalPlayer)
		return false;

	if (!g_pSettings->RagebotFriendly->Checked) {
		if (Entity->GetTeam() == G::LocalPlayer->GetTeam())
			return false;
	}

	if (Entity->GetDormant())
		return false;
	if (Entity->GetImmune())
		return false;
	if (Entity->GetHealth() <= 0)
		return false;


	return true;
}
