#include "ConservativeAutowall.h"

ConservativeAutowall* g_pAutowall = new ConservativeAutowall;

const float GetHitgroupDamageMultiplier(int iHitGroup) {
	switch (iHitGroup) {
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_CHEST:
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
	default: return 1.0f;
	}
	return 1.0f;
}

void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage) {
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);
	float m_flArmor = (float)enemy->GetArmor();
	if (m_flArmor > 0.0f && hitgroup < HITGROUP_LEFTLEG)  //don't do gear, or legs
	{
		if (hitgroup == HITGROUP_HEAD && !enemy->HasHelmet())
			return;

		float armorscaled = (weapon_armor_ratio)* current_damage;
		if ((current_damage - armorscaled) > m_flArmor)
			armorscaled = current_damage - (m_flArmor * 2.0f);
		current_damage = armorscaled;
	}
}

//Old autowall
DWORD dwClipTraceToPlayer;
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{

	if (!dwClipTraceToPlayer)
		dwClipTraceToPlayer = Util::FindPattern("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");


	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwClipTraceToPlayer
		ADD		ESP, 0xC
	}
}
DWORD dwEntBreakable = NULL;
bool TraceToExit(Vector& vecEnd, trace_t* pEnterTrace, Vector vecStart, Vector vecDir, trace_t* pExitTrace)
{
	auto distance = 0.0f;
	auto DidHitNonWorldCBaseEntity = [](CBaseEntity* m_pEnt) -> bool
	{
		return m_pEnt != nullptr && m_pEnt == Interfaces->ClientEntityList->GetClientEntity(0);
	};

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		vecEnd = vecStart + vecDir * distance;

		auto point_contents = Interfaces->EngineTrace->GetPointContents(vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = vecEnd - (vecDir * 4.0f);

		Ray_t ray;
		ray.Init(vecEnd, new_end);
		Interfaces->EngineTrace->TraceRay(ray, 0x4600400B, nullptr, pExitTrace);

		if (pExitTrace->m_pEnt == nullptr)
			return false;

		if (pExitTrace->startsolid && pExitTrace->surface.flags & SURF_HITBOX)
		{
			CTraceFilter filter;
			filter.pSkip = pExitTrace->m_pEnt;

			ray.Init(vecEnd, vecStart);
			Interfaces->EngineTrace->TraceRay(ray, 0x600400B, &filter, pExitTrace);

			if ((pExitTrace->fraction < 1.0f || pExitTrace->allsolid) && !pExitTrace->startsolid)
			{
				vecEnd = pExitTrace->endpos;
				return true;
			}

			continue;
		}

		if (!(pExitTrace->fraction < 1.0 || pExitTrace->allsolid || pExitTrace->startsolid) || pExitTrace->startsolid)
		{
			if (pExitTrace->m_pEnt)
			{
				/*disable the FindPattern shit and stuff and use the rebuilt one, once glass doesn't crash anymore*/
				CBaseEntity* pBreakEnt = pExitTrace->m_pEnt;

				if (dwEntBreakable == NULL)
				{
					dwEntBreakable = Util::FindPattern("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
				}
				bool bBreakable;

				__asm
				{
					MOV ECX, pBreakEnt
					CALL dwEntBreakable
					MOV bBreakable, AL
				}


				if (!DidHitNonWorldCBaseEntity(pExitTrace->m_pEnt) && /*GameUtils::IsBreakableEntity(pExitTrace->m_pEnt) &&*/ bBreakable)
					return true;
			}

			continue;
		}

		if (((pExitTrace->surface.flags >> 7) & 1) && !((pExitTrace->surface.flags >> 7) & 1))
			continue;

		if (pExitTrace->plane.normal.Dot(vecDir) < 1.1f)
		{
			float fraction =	pExitTrace->fraction * 4.0f;
			vecEnd = vecEnd - (vecDir * fraction);

			return true;
		}
	}

	return false;
}

bool ConservativeAutowall::HandleBulletPenetration(WeaponInfo_t* wpn_data, FireBulletDat& data)
{
	surfacedata_t *enter_surface_data = Interfaces->Physprops->GetSurfaceData(data.enter_trace.surface.surfaceProps);

	int enter_material = enter_surface_data->game.material;

	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)pow(wpn_data->m_flRangeModifier, (data.trace_length * 0.002));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;

	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = Interfaces->Physprops->GetSurfaceData(trace_exit.surface.surfaceProps);

	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}


	auto flModifier = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	auto flTakenDamage = (data.current_damage * final_damage_modifier) + flModifier * 3.f * fmaxf(0.f, (3.f / wpn_data->m_flPenetration) * 1.25f);
	auto flThickness = Math::VectorDistance(trace_exit.endpos, data.enter_trace.endpos);

	flThickness *= flThickness;
	flThickness *= flModifier;
	flThickness /= 24.f;


	auto flTravelDamage = fmaxf(0.f, flTakenDamage + flThickness);

	if (flTravelDamage > data.current_damage)
		return false;

	if (flTravelDamage >= 0.0f)
		data.current_damage -= flTravelDamage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;



	return true;
}

void TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter filter;
	filter.pSkip = ignore;

	Interfaces->EngineTrace->TraceRay(ray, mask, &filter, ptr);
}

bool ConservativeAutowall::SimulateFireBullet(CBaseEntity* pBaseEntity, FireBulletDat& data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = G::LocalPlayer->GetWeapon()->GetCSWpnData();

	data.current_damage = (float)wpn_data->m_iDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_flRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		TraceLine(data.src, end, 0x4600400B, G::LocalPlayer, &data.enter_trace);

		//proper way but seems to have a bad performance?
		//UTIL_ClipTraceToPlayers(data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);


		if (data.enter_trace.fraction == 1.0f)
			break;
		ClientClass* pClass = (ClientClass*)data.enter_trace.m_pEnt->GetClientClass();
		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (data.enter_trace.m_pEnt->GetTeam() != G::LocalPlayer->GetTeam()) && pClass->m_ClassID != (int)85 /* CGame::EntityTypes::CSHostage */)
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= (float)pow(wpn_data->m_flRangeModifier, data.trace_length * 0.002);

			switch(data.enter_trace.hitgroup)
			{
				case 1:
					data.current_damage *= 4.0f;
					break;
				case 3:
					data.current_damage *= 1.25f;
					break;
				case 6:
				case 7:
					data.current_damage *= 0.75f;
				default:
					break;
			}

			if (data.enter_trace.m_pEnt->GetArmor() > 0)
			{
				if (data.enter_trace.hitgroup == 1)
				{
					if (data.enter_trace.m_pEnt->HasHelmet())
						data.current_damage *= (wpn_data->m_flArmorRatio * .5f);
				}
				else
					data.current_damage *= (wpn_data->m_flArmorRatio * .5f);
			}

			return true;
		}

		if (/*!RagebotElements.Ragebot_Autowall->Checked || */!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}

int ConservativeAutowall::PenetrateWall(CBaseEntity* pBaseEntity, const Vector& vecPoint)
{
	auto AngleVectors = [](const Vector &angles, Vector *forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	};

	FireBulletDat data;
	data.src = G::LocalPlayer->GetEyePosition();
	data.filter = CTraceFilter();
	data.filter.pSkip = G::LocalPlayer;

	QAngle angles = Math::CalcAngle(data.src, vecPoint);
	AngleVectors(Vector(angles.x, angles.y, angles.z), &data.direction);
	data.direction.Normalize();

	if (SimulateFireBullet(G::LocalPlayer, data))
	{
		return data.current_damage;
	}

	return 0;
}

