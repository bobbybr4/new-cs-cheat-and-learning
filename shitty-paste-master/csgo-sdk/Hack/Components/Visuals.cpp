#include "../../csgo-sdk.h"

struct box_t
{
	int x, y, w, h;
};

auto GetBox(CBaseEntity* player) -> Vector*
{
	Vector vOrigin, min, max;
	// Get the locations
	vOrigin = player->GetAbsOrigin();
	min = player->GetCollideable()->OBBMins() + vOrigin;
	max = player->GetCollideable()->OBBMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	return points;
}

bool GetBox(CBaseEntity* pEntity, box_t& result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->GetCollideable()->OBBMins() + vOrigin;
	max = pEntity->GetCollideable()->OBBMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!D::WorldToScreen(points[3], flb) || !D::WorldToScreen(points[5], brt)
		|| !D::WorldToScreen(points[0], blb) || !D::WorldToScreen(points[4], frt)
		|| !D::WorldToScreen(points[2], frb) || !D::WorldToScreen(points[1], brb)
		|| !D::WorldToScreen(points[6], blt) || !D::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

void DrawBox(CBaseEntity* player, Color col)
{
	Vector* boxVectors = GetBox(player);

	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;

	if (D::WorldToScreen(boxVectors[0], boxVectors0) &&
		D::WorldToScreen(boxVectors[1], boxVectors1) &&
		D::WorldToScreen(boxVectors[2], boxVectors2) &&
		D::WorldToScreen(boxVectors[3], boxVectors3) &&
		D::WorldToScreen(boxVectors[4], boxVectors4) &&
		D::WorldToScreen(boxVectors[5], boxVectors5) &&
		D::WorldToScreen(boxVectors[6], boxVectors6) &&
		D::WorldToScreen(boxVectors[7], boxVectors7))
	{
		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };

		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++)
		{
			D::DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, col); 
			D::DrawLine(lines[i][0].x - 1, lines[i][0].y - 1, lines[i][1].x - 1, lines[i][1].y - 1, Color(0, 0, 0, 155));
			D::DrawLine(lines[i][0].x + 1, lines[i][0].y + 1, lines[i][1].x + 1, lines[i][1].y + 1, Color(0, 0, 0, 255));
		}
	}
}

void Crosshair()
{
	int screenW, screenH;
	Interfaces->EngineClient->GetScreenSize(screenW, screenH);

	int crX = screenW / 2, crY = screenH / 2;
	int drX;
	int drY;

	if (g_pSettings->VisualsReticle->SelectedIndex == 2 && G::LocalPlayer->GetWeapon())
	{
		int dy = screenH / 90;
		int dx = screenW / 90;
		if (g_pSettings->VisualsRemovalRecoil->Checked)
		{
			drX = crX - (int)(dx * (G::LocalPlayer->GetPunch().y * 2));
			drY = crY + (int)(dy * (G::LocalPlayer->GetPunch().x * 2));
		}
		else
		{
			drX = crX - (int)(dx * (((G::LocalPlayer->GetPunch().y * 2.f) * 0.45f) + G::LocalPlayer->GetViewPunch().y));
			drY = crY + (int)(dy * (((G::LocalPlayer->GetPunch().x * 2.f) * 0.45f) + G::LocalPlayer->GetViewPunch().x));
		}
	}
	else
	{
		drX = crX;
		drY = crY;
	}

	Color col = Color::Cyan();

	//Vector vec(G::LastAngle2.x, G::LastAngle2.y, G::LastAngle2.z);
	/*Vector vec;
	Math::AngleVectors(G::RealAngle, vec);
	
	bool pencrosh = g_pSettings->VisualsReticlePenetration->Checked;
	float dmg;

	if (pencrosh)
	{
		bool penetrated;
		dmg = Autowall->GetDamage(vec, penetrated);

		if (penetrated)
		{
			col = Color::Green();
		}
		else if (penetrated && dmg)
		{
			col = Color::Cyan();
		}
		else
		{
			col = Color::Red();
		}
	}

	if (dmg && pencrosh)
	{
		D::DrawString(F::ESP, screenW / 2, screenH / 2 + 20, Color::White(), 0, "-%i HP", dmg);
	}*/


	D::DrawLine(drX - 5, drY, drX + 5, drY, col);
	D::DrawLine(drX, drY - 5, drX, drY + 5, col);

	if (g_pSettings->VisualsOverlayInaccuracy->Checked && G::LocalPlayer->GetWeapon())
	{
		if (G::LocalPlayer->GetWeapon()->IsGun())
		{
			CBaseCombatWeapon* g_pWeapon = G::LocalPlayer->GetWeapon();
			
			float flCone = g_pWeapon->GetInaccuracy();
			float flSpread = g_pWeapon->GetWeaponSpread();
		
			//	take the spread and the inaccuracy, spread is at 485 and inaccuracy at 484, add them together, multiply that by 500 and you have the radius for your circle
			float flRadius = (flCone + flSpread) * 500.0f; // s/o: mlooser18

			D::DrawCircle(screenW / 2, screenH / 2, flRadius, 100, Color::Cyan());
		}
	}
}

float flHitmarkerAlpha = 0.0f;

void HitmarkerRenderable()
{
	int w, h;
	Interfaces->EngineClient->GetScreenSize(w, h);

	// credits: markhc
	int screenCenterX = w / 2;
	int screenCenterY = h / 2;
	int lineSize = 12;

	D::DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, static_cast<int>(255 * flHitmarkerAlpha)));
	D::DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, static_cast<int>(255 * flHitmarkerAlpha)));
	D::DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4), Color(255, 255, 255, static_cast<int>(255 * flHitmarkerAlpha)));
	D::DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4), Color(255, 255, 255, static_cast<int>(255 * flHitmarkerAlpha)));
}

void CVisuals::Tick() {

	int w, h;
	Interfaces->EngineClient->GetScreenSize(w, h);

	D::DrawString(F::ESP, w - 5, 11, Color(255, 255, 255, 255), FONT_RIGHT, "necrophili.ac");
	D::DrawString(F::ESP, w - 5, 23, Color::White(), FONT_RIGHT, "built on: %s", __DATE__);

	if (flHitmarkerAlpha > 0)
	{
		flHitmarkerAlpha -= 0.05f;
	}

	if (!g_pSettings->VisualsEnabled->Checked || !G::LocalPlayer || !Interfaces->EngineClient->IsInGame())
		return;
	
	if (g_pSettings->VisualsHitMarker->Checked)
		HitmarkerRenderable();

	DrawGlow();
	
	if (g_pSettings->VisualsGrenadeTrajectory->Checked) 
	{
		GrenadePrediction->PaintGrenadeTrajectory();
	}

	for (int i = 0; i < Interfaces->ClientEntityList->GetHighestEntityIndex(); i++) {
		PlayerESP(i);
		WorldESP(i);
	}

	if (g_pSettings->VisualsReticle->SelectedIndex)
		Crosshair();

	DrawLBYDelta(w, h);
}

bool CVisuals::ScreenTransform(const Vector& point, Vector& screen) {
	float w;
	const VMatrix& worldToScreen = Interfaces->EngineClient->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool CVisuals::WorldToScreen(const Vector &origin, Vector &screen) {
	if (!ScreenTransform(origin, screen)) {
		int ScreenWidth, ScreenHeight;
		Interfaces->EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = float(ScreenWidth) / 2;
		float y = (float)ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

void CVisuals::DrawGlow() {
	static CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)offsets->GlowManager;

	if (!g_pSettings->VisualsGlow->Checked)
		return;

	for (int i = 0; i < GlowObjectManager->size; i++) {
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		CBaseEntity* Entity = glowEntity->GetEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		if (!g_pSettings->VisualsFriends->Checked && (!Entity->IsEnemy() || Entity == G::LocalPlayer))
			continue;

		if (!g_pSettings->VisualsEnemies->Checked && Entity->IsEnemy())
			continue;

		if (Entity == G::LocalPlayer)
			continue;

		switch (Entity->GetClientClass()->m_ClassID) {

		case 1:
			if (g_pSettings->VisualsDropped->Checked)
				glowEntity->Set(Color(g_pSettings->ColorEntity->color));

			break;

		case 29:
			if (g_pSettings->VisualsC4->Checked)
				glowEntity->Set(Color(g_pSettings->ColorEntity->color));

			break;

		case 35:
				if (Entity->IsVisible(HEAD_0))
					glowEntity->Set(Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyVisible->color) : Color(g_pSettings->ColorTeamVisible->color));
				else
					glowEntity->Set(Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyInvisible->color) : Color(g_pSettings->ColorTeamInvisible->color));			
			break;

		case 39:
			if (g_pSettings->VisualsDropped->Checked)
				glowEntity->Set(Color(g_pSettings->ColorEntity->color));

			break;

		case 41:
			if (g_pSettings->VisualsDropped->Checked)
				glowEntity->Set(Color(g_pSettings->ColorEntity->color));

			break;

		case 105:
			if (g_pSettings->VisualsC4->Checked)
				glowEntity->Set(Color(g_pSettings->ColorEntity->color));
			break;

		default:
			if (g_pSettings->VisualsDropped->Checked) {
				if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon"))
					glowEntity->Set(Color(g_pSettings->ColorEntity->color));
			}
			break;
		}
	}
}

template <typename T>
inline T clamp_(const T& val, const T& lower, const T& upper)
{
	return max(lower, min(val, upper));
}

void DrawHealth(CBaseEntity* player, int x, int y, int w, int h)
{
	int health = player->GetHealth();

	UINT hp = (h - (UINT)((h * health) / 100)); // Percentage

	if (hp > h)
		hp = h;

	D::DrawRect(x, y, w, h, Color(0, 0, 0, 180));
	D::DrawRect(x + 1, y + 1 + hp, w - 2, h - hp - 2, health < 50 ? health < 25 ? Color::Red() : Color::Orange() : Color::Green());
	D::DrawOutlinedRect(x, y, w, h, Color(25, 25, 25, 240));

	for (int i = 0; i <= 3; i++)
	{
		D::DrawRect(x, clamp_(y + h / w * i, y, y + h), w, 1, Color(25, 25, 25, 240));
	}

	if (health > 0 && health < 100)
	{
		D::DrawString(F::ESP, x - D::GetStringWidth(F::ESP, "%i", health), y + hp, Color(255, 255, 255, 255), FONT_CENTER, "%i", health);
	}
}

void CVisuals::PlayerESP(int index) {
	CBaseEntity* Entity = Interfaces->ClientEntityList->GetClientEntity(index);

	if (EntityIsInvalid(Entity))
		return;

	if (!Entity->IsPlayer())
		return;

	if (!G::LocalPlayer->IsAlive()) {
		if (Interfaces->ClientEntityList->GetClientEntityFromHandle(G::LocalPlayer->GetObserverTarget()) == Entity)
			return;
	}

	if (!g_pSettings->VisualsFriends->Checked && (!Entity->IsEnemy() || Entity == G::LocalPlayer))
		return;

	if (!g_pSettings->VisualsEnemies->Checked && Entity->IsEnemy())
		return;

	if (Entity == G::LocalPlayer)
		return;

	if (g_pSettings->VisualsLegit->Checked && !Entity->IsVisible(HEAD_0))
		return;

	box_t box;

	if (!GetBox(Entity, box))
		return;

	int width = box.w;
	int height = box.h;

	if (g_pSettings->VisualsSkeleton->SelectedIndex)
	{
		Skeleton(Entity, Color::White());
	}

	if (g_pSettings->VisualsBox->SelectedIndex)
	{
		switch (g_pSettings->VisualsBox->SelectedIndex)
		{
		// i've been laying low, out in tokyo
		case 1:
		{
			if (Entity->IsVisible(HEAD_0))
			{
				PlayerCorners(box.x, box.y, width, height, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyVisible->color) : Color(g_pSettings->ColorTeamVisible->color));
			}
			else
			{
				PlayerCorners(box.x, box.y, width, height, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyInvisible->color) : Color(g_pSettings->ColorTeamInvisible->color));
			}
		}break;

		case 2:
		{
			if (Entity->IsVisible(HEAD_0))
			{
				PlayerBox(box.x, box.y, width, height, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyVisible->color) : Color(g_pSettings->ColorTeamVisible->color));
			}
			else
			{
				PlayerBox(box.x, box.y, width, height, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyInvisible->color) : Color(g_pSettings->ColorTeamInvisible->color));
			}
		}break;

		case 3:
		{
			if (Entity->IsVisible(HEAD_0))
			{
				DrawBox(Entity, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyVisible->color) : Color(g_pSettings->ColorTeamVisible->color));
			}
			else
			{
				DrawBox(Entity, Entity->IsEnemy() ? Color(g_pSettings->ColorEnemyInvisible->color) : Color(g_pSettings->ColorTeamInvisible->color));
			}
		}break;

		}
	}
	
	/*if (g_pSettings->VisualsHitbox->SelectedIndex)
		Hitboxes(Entity);*/

	if (g_pSettings->VisualsName->Checked) {
		player_info_t pinfo;
		Interfaces->EngineClient->GetPlayerInfo(index, &pinfo);
		std::string name = pinfo.name;
		if (name.length() > 16)
		{
			name.erase(16, name.length());
		}

		D::DrawString(F::ESP, static_cast<int>(box.x + width / 2), static_cast<int>(box.y) - 8, Color::White(), FONT_CENTER, "%s", name.c_str());
	}

	int bottom2 = 0;

	if (g_pSettings->VisualsHealth->SelectedIndex == 1 || g_pSettings->VisualsHealth->SelectedIndex == 3)
		DrawHealth(Entity, box.x - 6, box.y - 1, 4, box.h + 1);

	else if (g_pSettings->VisualsHealth->SelectedIndex == 2 || g_pSettings->VisualsHealth->SelectedIndex == 3)
		D::DrawString(F::ESP, static_cast<int>(box.x + width + 3), static_cast<int>(box.y + 2), Color::White(), FONT_CENTER, "%i HP", Entity->GetHealth());

	int bottom = 0;

	if (g_pSettings->VisualsWeapon->Checked)
		D::DrawString(F::ESP, static_cast<int>(box.x + width / 2), static_cast<int>(box.y) + static_cast<int>(height) + 8 + (12 * bottom++), Color::White(), FONT_CENTER, "%s", Entity->GetWeapon()->GetWeaponName().c_str());

	if (g_pSettings->VisualsFlags->Checked)
		D::DrawString(F::ESP, static_cast<int>(box.x + width / 2), static_cast<int>(box.y) + static_cast<int>(height) + 8 + (12 * bottom++), Color::Yellow(), FONT_CENTER, "%i AP", Entity->GetArmor());

	if (Entity == Aimbot->m_bestent)
		D::DrawString(F::ESP, static_cast<int>(box.x + width / 2), static_cast<int>(box.y) + static_cast<int>(height) + 8 + (12 * bottom++), Color::Red(), FONT_CENTER, "Target");
}

void CVisuals::WorldESP(int index) {
	CBaseEntity* Entity = Interfaces->ClientEntityList->GetClientEntity(index);

	if (!Entity)
		return;

	Vector pos, pos3D;
	pos3D = Entity->GetOrigin();

	if (pos3D.Empty())
		return;

	if (!WorldToScreen(pos3D, pos))
		return;

	auto owner = Entity->GetOwnerEntity();

	if (!owner.IsValid()) {
		if (g_pSettings->VisualsDropped->Checked) {
			if (strstr(Entity->GetClientClass()->m_pNetworkName, "CWeapon")) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, std::string(Entity->GetClientClass()->m_pNetworkName).substr(7).c_str());
			}

			if (Entity->GetClientClass()->m_ClassID == 1) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, "AK-47");
			}

			if (Entity->GetClientClass()->m_ClassID == 39) {
				D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::White(), FONT_RIGHT, "Deagle");
			}
		}

		if (Entity->GetClientClass()->m_ClassID == 29 && g_pSettings->VisualsC4->Checked)
			D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Color::LightBlue(), FONT_RIGHT, "C4");
	}

	if (Entity->GetClientClass()->m_ClassID == 41 && g_pSettings->VisualsDropped->Checked) {
		CBaseEntity* OwnerEntity = Interfaces->ClientEntityList->GetClientEntity(owner.ToInt() & 0xFFF);
	}

	if (strstr(Entity->GetClientClass()->m_pNetworkName, "CPlantedC4") && g_pSettings->VisualsC4->Checked)
	{
		D::DrawString(F::ESP, static_cast<int>(pos.x), static_cast<int>(pos.y), Entity->GetBombTimer() < 5.0f ? Color::Red() : Color::White(), FONT_CENTER, "C4 Detonation: %.1f", clamp(Entity->GetBombTimer(), 0.0f, 120.0f));
	}
}

float GetFLatency()
{
	INetChannelInfo *nci = Interfaces->EngineClient->GetNetChannelInfo();
	if (nci)
	{
		return nci->GetAvgLatency(FLOW_INCOMING);
	}
	return 0.0f;
}

int GetChoked(CBaseEntity* pEntity)
{
	float flSimulationTime = pEntity->GetSimulationTime();
	float flSimDiff = Interfaces->Globals->curtime - flSimulationTime;
	float latency = GetFLatency();
	return TIME_TO_TICKS(max(0.0f, flSimDiff - latency));
}

bool IsBreakingLagCompensation(CBaseEntity* player)
{
	int m_iIndex = player->GetIndex();

	LagRecord* m_LagRecords = Backtracking->m_LagRecord[m_iIndex];

	LagRecord recentLR = m_LagRecords[0], prevLR;

	prevLR = m_LagRecords[8];
	
	if (recentLR.m_fSimulationTime == 0.0f)
		return false;

	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).LengthSqr() > 4096.0f)
	{
		return true;
	}

	return false;
}

void CVisuals::DrawLBYDelta(int w, int h) {
	if (!g_pSettings->RagebotEnabled->Checked || !g_pSettings->RagebotAntiaim->Checked)
		return;

	auto dif = static_cast<float>(abs(Math::ClampYaw(G::LocalPlayer->GetEyeAngles().y - (G::LocalPlayer->IsMovingOnGround() ? G::LocalPlayer->GetEyeAngles().y : G::LocalPlayer->GetLowerBodyYawTarget()))));
	auto msg = std::wstring(L"LBY data: ");
	std::string msga = std::string(msg.begin(), msg.end());
	int wid, heig;
	Interfaces->Surface->GetTextSize(F::ESP, msg.c_str(), wid, heig);
	D::DrawString(F::ESP, 20, h / 2, Color(255, 255, 255, 255), 0, msga.c_str());
	D::DrawString(F::ESP, (20) + wid, (h / 2), dif > 35 ? dif > 90 ? Color::Green() : Color::Orange() : Color::Red(), 0, "%s", dif > 35 ? dif > 90 ? "[secured]" : "[dangerous]" : "[critical]");
	Interfaces->Surface->GetTextSize(F::ESP, L"LC data: ", wid, heig);
	D::DrawString(F::ESP, 20, h / 2 + (heig), Color::White(), 0, "LC data: ");
	D::DrawString(F::ESP, 20 + wid, h / 2 + (heig), IsBreakingLagCompensation(G::LocalPlayer) ? Color::Green() : Color::Red(), 0, IsBreakingLagCompensation(G::LocalPlayer) ? "[secured]" : "[critical]");
	D::DrawString(F::ESP, 20, h / 2 + (heig * 2), Color::White(), 0, "Predictable yaw: %2.f deg", G::LocalPlayer->GetLowerBodyYawTarget());
	//D::DrawString(F::ESP, 20, h / 2 + (heig * 3), Color::White(), 0, "Map: %s", Interfaces->EngineClient->GetLevelName());

	if ((Interfaces->Input->m_bCameraInThirdPerson || g_pSettings->VisualsThirdPerson->Checked) && g_pSettings->VisualsThirdPersonGhost->SelectedIndex == 1)
	{
		Vector vecLby, vecFake;
		Vector org = G::LocalPlayer->GetOrigin(), vOrg;
		Vector end, vEnd;
		Math::AngleVectors(QAngle(0, (G::LocalPlayer->GetLowerBodyYawTarget()), 0), vecLby);
		Math::AngleVectors(QAngle(0, (G::FakeAngle.y), 0), vecFake);

		end = org + (vecFake * 70);

		/*if (WorldToScreen(org, vOrg) && WorldToScreen(end, vEnd))
		{
			D::DrawLine(vOrg.x, vOrg.y, vEnd.x, vEnd.y, Color(255, 0, 0, 255));
			D::DrawString(F::ESP, vEnd.x, vEnd.y, Color::White(), FONT_CENTER, "REAL");
		}*/
		end = org + (vecLby * 70);

		if (WorldToScreen(org, vOrg) && WorldToScreen(end, vEnd))
		{
			D::DrawLine(vOrg.x, vOrg.y, vEnd.x, vEnd.y, dif > 35 ? dif > 90 ? Color::Green() : Color::Orange() : Color::Red());
			D::DrawString(F::ESP, vEnd.x, vEnd.y, dif > 35 ? dif > 90 ? Color::Green() : Color::Orange() : Color::Red(), FONT_CENTER, "LBY");
		}
	}
	else if ((Interfaces->Input->m_bCameraInThirdPerson || g_pSettings->VisualsThirdPerson->Checked) && g_pSettings->VisualsThirdPersonGhost->SelectedIndex == 2)
	{
		Skeleton(G::LocalPlayer, dif > 35 ? dif > 90 ? Color::Green() : Color::Orange() : Color::Red());
	}
}

//////////////////////////////////////////////////////////////////////////


bool CVisuals::EntityIsInvalid(CBaseEntity* Entity) {
	if (!Entity)
		return true;
	if (Entity->GetHealth() <= 0)
		return true;
	if (Entity->GetDormant())
		return true;
	if (g_pSettings->VisualsLegit->Checked && !Entity->IsVisible(HITBOX_CHEST))
		return true;


	return false;
}

void CVisuals::PlayerBox(float x, float y, float w, float h, Color clr) {
	D::DrawOutlinedRect(x, y, w, h, Color::Black());
	D::DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2, clr);
	D::DrawOutlinedRect(x + 2, y + 2, w - 4, h - 4, Color::Black());
}

void CVisuals::PlayerCorners(float x, float y, float w, float h, Color col) 
{
	// to get perfect corners on any entity i need to use a dynamic formula that depends on their
	// height and width, meaning it will be scaled. this will result in a nice looking corner esp

	/*
	y    w
   x+--------+
	|        |
   h|        |
    |        |	
    |        |	
    |        |	
	|        |
	+--------+
	*/

	int mw = w / 3; // yeah i lied fuck that ^^
	int mh = h / 5.33;

	D::DrawCorner(x, y, mw, mh, false, false, col);
	D::DrawCorner(x + w, y, mw, mh, true, false, col);
	D::DrawCorner(x, y + h, mw, mh, false, true, col);
	D::DrawCorner(x + w, y + h, mw, mh, true, true, col);
}

void AngleMatrix(const QAngle &angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;
	Math::SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	Math::SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	Math::SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;

	float crcy = cr*cy;
	float crsy = cr*sy;
	float srcy = sr*cy;
	float srsy = sr*sy;
	matrix[0][1] = sp*srcy - crsy;
	matrix[1][1] = sp*srsy + crcy;
	matrix[2][1] = sr*cp;

	matrix[0][2] = (sp*crcy + srsy);
	matrix[1][2] = (sp*crsy - srcy);
	matrix[2][2] = cr*cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void CVisuals::Skeleton(CBaseEntity *Entity, Color color) {
	studiohdr_t* pStudioModel = Interfaces->ModelInfo->GetStudioModel(Entity->GetModel());
	if (pStudioModel) {
		static matrix3x4_t pBoneToWorldOut[128];

		if (Entity->SetupBones(pBoneToWorldOut, 128, 256, Interfaces->Globals->curtime)) {

			/* lifestyle */
			matrix3x4_t* RotatedMatrix = pBoneToWorldOut;

			if (Entity == G::LocalPlayer && g_pSettings->VisualsThirdPersonGhost->SelectedIndex == 2)
			{
				for(auto i = 0; i < 128; ++i)
					AngleMatrix(G::FakeAngle, RotatedMatrix[i]);
			}

			for (int i = 0; i < pStudioModel->numbones; i++) {
				mstudiobone_t* pBone = pStudioModel->pBone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector vBonePos1;
				if (!WorldToScreen(Vector(RotatedMatrix[i][0][3], RotatedMatrix[i][1][3], RotatedMatrix[i][2][3]), vBonePos1))
					continue;

				Vector vBonePos2;
				if (!WorldToScreen(Vector(RotatedMatrix[pBone->parent][0][3], RotatedMatrix[pBone->parent][1][3], RotatedMatrix[pBone->parent][2][3]), vBonePos2))
					continue;

				D::DrawLine((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, color);
			}
		}
	}
	
	if (g_pSettings->VisualsSkeleton->SelectedIndex == 2)
	{
		int index = Entity->GetIndex();

		LagRecord* m_LagRecords = Backtracking->m_LagRecord[index];
		LagRecord recentLR = m_LagRecords[14];


		Vector lastPosition = Entity->GetBonePosition(8);
		Vector lastSpot;

		for (int i = 0; i < 9; i++)
		{
			Vector screenSpot;

			if (D::WorldToScreen(m_LagRecords[i].headSpot, screenSpot))
			{
				D::DrawRect(screenSpot.x, screenSpot.y, 3, 3, Color::Green());
			}
		}

		Vector screenSpot2;
		if (D::WorldToScreen(recentLR.headSpot, screenSpot2))
		{
			D::DrawRect(screenSpot2.x, screenSpot2.y, 3, 3, Color::Red());
		}
	}
}

void CVisuals::DrawHitbox(matrix3x4a_t* matrix, Vector bbmin, Vector bbmax, int bone, Color color) {
	Vector points[] = {
		Vector(bbmin.x, bbmin.y, bbmin.z),
		Vector(bbmin.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmin.z),
		Vector(bbmax.x, bbmin.y, bbmin.z),
		Vector(bbmax.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmax.y, bbmax.z),
		Vector(bbmin.x, bbmin.y, bbmax.z),
		Vector(bbmax.x, bbmin.y, bbmax.z)
	};

	Vector pointsTransformed[8];

	for (int index = 0; index < 8; ++index) {
		if (index != 0)
			points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

		pointsTransformed[index] = Math::VectorTransform(points[index], matrix[bone]);
	}

	Draw3DBox(pointsTransformed, color);
}

void CVisuals::Hitboxes(CBaseEntity *Entity) {
	matrix3x4a_t matrix[MAXSTUDIOBONES];

	if (!Entity->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, Interfaces->EngineClient->GetLastTimeStamp()))
		return;

	studiohdr_t* pStudioModel = Interfaces->ModelInfo->GetStudioModel(Entity->GetModel());
	if (!pStudioModel)
		return;

	mstudiohitboxset_t* set = pStudioModel->pHitboxSet(Entity->GetHitboxSet());
	if (!set)
		return;

	for (int i = 0; i < set->numhitboxes; i++) {
		mstudiobbox_t* hitbox = set->pHitbox(i);
		if (!hitbox)
			return;

		int bone = hitbox->bone;

		Vector vMaxUntransformed = hitbox->bbmax;
		Vector vMinUntransformed = hitbox->bbmin;

		if (hitbox->radius != -1.f) {
			vMinUntransformed -= Vector(hitbox->radius, hitbox->radius, hitbox->radius);
			vMaxUntransformed += Vector(hitbox->radius, hitbox->radius, hitbox->radius);
		}

		Color color = Color::White();

		DrawHitbox(matrix, vMinUntransformed, vMaxUntransformed, hitbox->bone, color);
	}
}

void CVisuals::Draw3DBox(Vector* boxVectors, Color color) {
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (WorldToScreen(boxVectors[0], boxVectors0) &&
		WorldToScreen(boxVectors[1], boxVectors1) &&
		WorldToScreen(boxVectors[2], boxVectors2) &&
		WorldToScreen(boxVectors[3], boxVectors3) &&
		WorldToScreen(boxVectors[4], boxVectors4) &&
		WorldToScreen(boxVectors[5], boxVectors5) &&
		WorldToScreen(boxVectors[6], boxVectors6) &&
		WorldToScreen(boxVectors[7], boxVectors7)) {

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++) {

			D::DrawLine(static_cast<int>(lines[i][0].x), static_cast<int>(lines[i][0].y), static_cast<int>(lines[i][1].x), static_cast<int>(lines[i][1].y), color);
		}
	}
}

HFont F::ESP = 0;

void D::SetupFonts() {
	if (!F::ESP)
		Interfaces->Surface->SetFontGlyphSet(F::ESP = Interfaces->Surface->Create_Font(), "Tahoma", 12, FW_DEMIBOLD, NULL, NULL, FONTFLAG_DROPSHADOW);
}

void D::DrawString(HFont font, int x, int y, Color color, uintptr_t alignment, const char* msg, ...) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	Interfaces->Surface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	Interfaces->Surface->DrawSetTextFont(font);
	Interfaces->Surface->DrawSetTextColor(r, g, b, a);
	Interfaces->Surface->DrawSetTextPos(x, y - height / 2);
	Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf));
}

void D::DrawStringUnicode(HFont font, int x, int y, Color color, bool bCenter, const wchar_t* msg, ...) {
	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int iWidth, iHeight;

	Interfaces->Surface->GetTextSize(font, msg, iWidth, iHeight);
	Interfaces->Surface->DrawSetTextFont(font);
	Interfaces->Surface->DrawSetTextColor(r, g, b, a);
	Interfaces->Surface->DrawSetTextPos(!bCenter ? x : x - iWidth / 2, y - iHeight / 2);
	Interfaces->Surface->DrawPrintText(msg, wcslen(msg));
}

void D::DrawRect(int x, int y, int w, int h, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawFilledRect(x, y, x + w, y + h);
}

int D::CreateTexture(void* tex, int w, int h) {
	int iTexture = Interfaces->Surface->CreateNewTextureID(true);
	Interfaces->Surface->DrawSetTextureRGBA(iTexture, (unsigned char*)tex, w, h);
	return iTexture;
}

void D::DrawTexturedRect(float x, float y, float w, float h, int TextureID) {
	D::DrawTexturedRect(x, y, w, h, TextureID, Color(255, 255, 255, 255));
}

void D::DrawTexturedRect(float x, float y, float w, float h, int TextureID, Color clr) {
	Interfaces->Surface->DrawSetColor(clr);
	Interfaces->Surface->DrawSetTexture(TextureID);
	Interfaces->Surface->DrawTexturedRect(x, y, x + w, y + h);
}

void D::DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow) {
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++) {
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		D::DrawRect(x + i, y, 1, height, colRainbow);
	}
}

void D::DrawRectGradientHorizontal(int x, int y, int width, int height, Color color1, Color color2) {
	float flDifferenceR = (float)(color2.r() - color1.r()) / (float)width;
	float flDifferenceG = (float)(color2.g() - color1.g()) / (float)width;
	float flDifferenceB = (float)(color2.b() - color1.b()) / (float)width;

	for (float i = 0.f; i < width; i++) {
		Color colGradient = Color(color1.r() + (flDifferenceR * i), color1.g() + (flDifferenceG * i), color1.b() + (flDifferenceB * i), color1.a());
		D::DrawRect(x + i, y, 1, height, colGradient);
	}
}

void D::DrawPixel(int x, int y, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawFilledRect(x, y, x + 1, y + 1);
}

void D::DrawOutlinedRect(int x, int y, int w, int h, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void D::DrawOutlinedCircle(int x, int y, int r, Color col) {
	//Interfaces->Surface->DrawSetColor(col);
	//Interfaces->Surface->DrawOutlinedCircle(x, y, r, 1);

	float step = (M_PI * 2.0f) / ((float)r * 50.f);

	for (float a = 0; a < (M_PI * 2.0f); a += step) {
		Vector2D start(r * cosf(a) + x, r * sinf(a) + y);
		Vector2D end(r * cosf(a + step) + x, r * sinf(a + step) + y);
		DrawLine(start.x, start.y, end.x, end.y, col);
	}
}

void D::DrawLine(int x0, int y0, int x1, int y1, Color col) {
	Interfaces->Surface->DrawSetColor(col);
	Interfaces->Surface->DrawLine(x0, y0, x1, y1);
}

void D::DrawCorner(int iX, int iY, int iWidth, int iHeight, bool bRight, bool bDown, Color colDraw) {
	int iRealX = bRight ? iX - iWidth : iX;
	int iRealY = bDown ? iY - iHeight : iY;

	if (bDown && bRight)
		iWidth = iWidth + 1;

	D::DrawRect(iRealX, iY, iWidth, 1, colDraw);
	D::DrawRect(iX, iRealY, 1, iHeight, colDraw);

	D::DrawRect(iRealX, bDown ? iY + 1 : iY - 1, !bDown && bRight ? iWidth + 1 : iWidth, 1, Color(0, 0, 0, 255));
	D::DrawRect(bRight ? iX + 1 : iX - 1, bDown ? iRealY : iRealY - 1, 1, bDown ? iHeight + 2 : iHeight + 1, Color(0, 0, 0, 255));
}

void D::DrawPolygon(int count, Vertex_t* Vertexs, Color color) {
	static int Texture = Interfaces->Surface->CreateNewTextureID(true);
	unsigned char buffer[4] = { 255, 255, 255, 255 };

	Interfaces->Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1);
	Interfaces->Surface->DrawSetColor(color);
	Interfaces->Surface->DrawSetTexture(Texture);

	Interfaces->Surface->DrawTexturedPolygon(count, Vertexs);
}

void D::DrawRoundedBox(int x, int y, int w, int h, int r, int v, Color col) {
	std::vector<Vertex_t> p;
	for (int _i = 0; _i < 3; _i++) {
		int _x = x + (_i < 2 && r || w - r);
		int _y = y + (_i % 3 > 0 && r || h - r);
		for (int i = 0; i < v; i++) {
			int a = RAD2DEG((i / v) * -90 - _i * 90);
			p.push_back(Vertex_t(Vector2D(_x + sin(a) * r, _y + cos(a) * r)));
		}
	}

	D::DrawPolygon(4 * (v + 1), &p[0], col);
}

bool D::ScreenTransform(const Vector &point, Vector &screen) // tots not pasted
{
	float w;
	const VMatrix& worldToScreen = Interfaces->EngineClient->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f) {
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else {
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool D::WorldToScreen(const Vector &origin, Vector &screen) {
	if (!ScreenTransform(origin, screen)) {
		int ScreenWidth, ScreenHeight;
		Interfaces->EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5f * screen.x * ScreenWidth + 0.5f;
		y -= 0.5f * screen.y * ScreenHeight + 0.5f;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}

int D::GetStringWidth(HFont font, const char* msg, ...) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf_s(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int iWidth, iHeight;

	Interfaces->Surface->GetTextSize(font, wbuf, iWidth, iHeight);

	return iWidth;
}

void D::Draw3DBox(Vector* boxVectors, Color color) {
	Vector boxVectors0, boxVectors1, boxVectors2, boxVectors3,
		boxVectors4, boxVectors5, boxVectors6, boxVectors7;


	if (D::WorldToScreen(boxVectors[0], boxVectors0) &&
		D::WorldToScreen(boxVectors[1], boxVectors1) &&
		D::WorldToScreen(boxVectors[2], boxVectors2) &&
		D::WorldToScreen(boxVectors[3], boxVectors3) &&
		D::WorldToScreen(boxVectors[4], boxVectors4) &&
		D::WorldToScreen(boxVectors[5], boxVectors5) &&
		D::WorldToScreen(boxVectors[6], boxVectors6) &&
		D::WorldToScreen(boxVectors[7], boxVectors7)) {

		/*
		.+--5---+
		.8 4    6'|
		+--7---+'  11
		9   |  10  |
		|  ,+--|3--+
		|.0    | 2'
		+--1---+'
		*/

		Vector2D lines[12][2];
		//bottom of box
		lines[0][0] = { boxVectors0.x, boxVectors0.y };
		lines[0][1] = { boxVectors1.x, boxVectors1.y };
		lines[1][0] = { boxVectors1.x, boxVectors1.y };
		lines[1][1] = { boxVectors2.x, boxVectors2.y };
		lines[2][0] = { boxVectors2.x, boxVectors2.y };
		lines[2][1] = { boxVectors3.x, boxVectors3.y };
		lines[3][0] = { boxVectors3.x, boxVectors3.y };
		lines[3][1] = { boxVectors0.x, boxVectors0.y };

		lines[4][0] = { boxVectors0.x, boxVectors0.y };
		lines[4][1] = { boxVectors6.x, boxVectors6.y };

		// top of box
		lines[5][0] = { boxVectors6.x, boxVectors6.y };
		lines[5][1] = { boxVectors5.x, boxVectors5.y };
		lines[6][0] = { boxVectors5.x, boxVectors5.y };
		lines[6][1] = { boxVectors4.x, boxVectors4.y };
		lines[7][0] = { boxVectors4.x, boxVectors4.y };
		lines[7][1] = { boxVectors7.x, boxVectors7.y };
		lines[8][0] = { boxVectors7.x, boxVectors7.y };
		lines[8][1] = { boxVectors6.x, boxVectors6.y };


		lines[9][0] = { boxVectors5.x, boxVectors5.y };
		lines[9][1] = { boxVectors1.x, boxVectors1.y };

		lines[10][0] = { boxVectors4.x, boxVectors4.y };
		lines[10][1] = { boxVectors2.x, boxVectors2.y };

		lines[11][0] = { boxVectors7.x, boxVectors7.y };
		lines[11][1] = { boxVectors3.x, boxVectors3.y };

		for (int i = 0; i < 12; i++) {
			D::DrawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, color);
		}
	}
}

void D::DrawCircle(float x, float y, float r, float s, Color color) {
	float Step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI*2.0); a += Step) {
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;

		DrawLine(x1, y1, x2, y2, color);
	}
}
