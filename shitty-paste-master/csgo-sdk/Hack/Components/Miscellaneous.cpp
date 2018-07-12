#include "../../csgo-sdk.h"

void FakeWalk()
{
	static int iChoked = -1;

	if (GetAsyncKeyState(g_pSettings->RagebotWalkKey->Key))
	{
		if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND) ||
			(G::UserCmd->command_number % 7) == 0 ||
			G::LocalPlayer->GetVelocity().Length() > 75)
			return;

		G::UserCmd->buttons &= ~IN_SPEED;

		iChoked++;

		if (iChoked < 1)
			G::SendPacket = false;

		G::UserCmd->tick_count += 10;
		G::UserCmd->command_number += 7 + G::UserCmd->tick_count % 2 ? 0 : 1;

		G::UserCmd->buttons |= G::LocalPlayer->GetMoveType() == IN_BACK;
		G::UserCmd->forwardmove = G::UserCmd->sidemove = 0.f;
	}
	else
	{
		G::SendPacket = true;
		iChoked = -1;

		Interfaces->Globals->frametime *= (G::LocalPlayer->GetVelocity().Length2D()) / 1.f;
		G::UserCmd->buttons |= G::LocalPlayer->GetMoveType() == IN_FORWARD;
	}
}

void SetName(std::string name)
{
	static ConVar* nameCvar = Interfaces->Cvar->FindVar("name");
	*(int*)((DWORD)&nameCvar->fnChangeCallback + 0xC) = 0;

	if (nameCvar)
	{
		nameCvar->SetValue(name.c_str());
	}
}

typedef void(__fastcall* ClanTagFn)(const char*, const char*);
ClanTagFn dw_ClanTag;
void SetClanTag(const char* tag)
{
	if (!dw_ClanTag)
		dw_ClanTag = reinterpret_cast<ClanTagFn>(Util::FindPattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));

	if (dw_ClanTag)
		dw_ClanTag(tag, tag);
}

void clantag_run()
{
	if (g_pSettings->MiscClantag->SelectedIndex > 0)
	{
		if (g_pSettings->MiscClantag->SelectedIndex == 1)
		{
			SetClanTag(g_pSettings->MiscClantagText->command.c_str());
		}
		else if (g_pSettings->MiscClantag->SelectedIndex != 3)
		{
			static int iLastTime;

			float latency = Interfaces->EngineClient->GetNetChannelInfo()->GetLatency(FLOW_INCOMING) + Interfaces->EngineClient->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);

			if (int(Interfaces->Globals->curtime * 2 + latency) != iLastTime)
			{
				std::string Name = g_pSettings->MiscClantagText->command;
				SetClanTag(Name.substr(0, int(Interfaces->Globals->curtime * 2 + latency) % (Name.length() + 1)).c_str());
				iLastTime = int(Interfaces->Globals->curtime * 2 + latency);
			}
		}
		else
		{
			std::string Name = g_pSettings->MiscClantagText->command;
			char meme[64];
			sprintf_s(meme, "%s\n", Name);
			SetClanTag(Name.c_str());
		}
	}
}

void Vanity()
{
	clantag_run();

		static int changes;

		auto GetEpochTime = []()
		{
			auto duration = std::chrono::system_clock::now().time_since_epoch();

			return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		};

		long currentTime_ms = GetEpochTime();
		static long timeStamp = currentTime_ms;

		if (currentTime_ms - timeStamp < 150)
			return;

		timeStamp = currentTime_ms;
		changes++;

		stringstream ss;

		for (auto i = 0; i <= 64; i++)
		{
			ss << static_cast<char>(-1);
		}

		ss << "necrophili.ac";

		for (auto i = 0; i <= 71; i++)
		{
			ss << static_cast<char>(-1);
		}

		if (changes % 2)
			ss << "\t";

		if(g_pSettings->MiscNamespam->Checked)
			SetName(ss.str());


	if (g_pSettings->MiscChatspam->SelectedIndex > 0)
	{
		if (g_pSettings->MiscChatspam->SelectedIndex == 1)
		{
			Interfaces->EngineClient->ClientCmd("say \"necrophili.ac - #1 AU cheat\"");
		}
		else
		{
			Interfaces->EngineClient->ClientCmd("say \"I am a necrophili.ac\"");
		}
	}
}
typedef void(*RevealAllFn)(float*);
RevealAllFn fnReveal;
void RankReveal()
{
	static float fBuffer[3] = { 0.0f, 0.0f, 0.0f };

	if (!fnReveal)
		fnReveal = (RevealAllFn)Util::FindPattern(XorStr("client.dll"), XorStr("55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ?"));

	if ((G::UserCmd->buttons & IN_SCORE) && g_pSettings->MiscRankReveal->Checked)
	{
		if(fnReveal)
			fnReveal(fBuffer);
	}
}

void CMiscellaneous::Tick() {
	RankReveal();
	
	if (g_pSettings->MiscAirstuck->Key)
		Airstuck();
	
	if (g_pSettings->MiscAutostrafe->SelectedIndex == 2)
	{
		Autostrafe();
	}

	else if(g_pSettings->MiscAutostrafe->SelectedIndex == 1)
	{
		if (G::LocalPlayer)
		{
			if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
			{
				if (G::UserCmd->mousedx > 0)
				{
					G::UserCmd->sidemove = 450;
				}
				else if (G::UserCmd->mousedx < 0)
				{
					G::UserCmd->sidemove = -450;
				}
				else
				{
					G::UserCmd->sidemove = (G::UserCmd->command_number % 2) ? 450 : -450; 
				}
			}
		}
	}


	if (g_pSettings->MiscBunnyhop->Checked)
		Bunnyhop();
	
	if (g_pSettings->MiscKnifeTrigger->Checked)
		KnifeTrigger();

	if (g_pSettings->MiscRankReveal->Checked)
		RankReveal();
	
	FakeWalk();
	Vanity();
}

void CMiscellaneous::Airstuck() {
	if (GetAsyncKeyState(g_pSettings->MiscAirstuck->Key) & 0x8000)
		if (g_pSettings->MiscRestrictions->SelectedIndex > 0)
			G::UserCmd->tick_count = INT_MAX;
		else
			G::UserCmd->forwardmove = *(float*)std::numeric_limits<float>::infinity;
}

float GetTraceFractionWorldProps(Vector startpos, Vector endpos) {
	Ray_t ray;
	trace_t tr;
	CTraceFilterNoPlayers filter;

	ray.Init(startpos, endpos);

	Interfaces->EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	return tr.fraction;
}

void CMiscellaneous::Autostrafe() {
	static float m_circle_yaw;
	static float m_previous_yaw;

	auto get_angle_from_speed = [](float speed) {
		auto ideal_angle = RAD2DEG(std::atan2(30.f, speed));
		std::clamp(ideal_angle, 0.f, 90.f);
		return ideal_angle;
	};

	auto get_velocity_step = [](Vector velocity, float speed, float circle_yaw) {
		auto velocity_degree = RAD2DEG(std::atan2(velocity.x, velocity.y));
		auto step = 1.5f;

		Vector start = G::LocalPlayer->GetOrigin(), end = start;

		Ray_t ray;
		CGameTrace trace;
		CTraceFilter filter;

		while (true) {
			end.x += (std::cos(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end.y += (std::sin(DEG2RAD(velocity_degree + circle_yaw)) * speed);
			end *= Interfaces->Globals->frametime;

			ray.Init(start, end, Vector(-20.f, -20.f, 0.f), Vector(20.f, 20.f, 32.f));
			Interfaces->EngineTrace->TraceRay(ray, CONTENTS_SOLID, &filter, &trace);

			if (trace.fraction < 1.f || trace.allsolid || trace.startsolid)
				break;

			step -= Interfaces->Globals->frametime;

			if (step == 0.f)
				break;

			start = end;
			velocity_degree += (velocity_degree + circle_yaw);
		}

		return step;
	};

	if (G::LocalPlayer->GetMoveType() != MOVETYPE_WALK || G::LocalPlayer->GetFlags() & FL_ONGROUND)
		return;

	auto velocity = G::LocalPlayer->GetVelocity();
	velocity.z = 0;

	static bool flip = false;
	auto turn_direction_modifier = flip ? 1.f : -1.f;
	flip = !flip;

	if (G::UserCmd->forwardmove > 0.f)
		G::UserCmd->forwardmove = 0.f;

	auto speed = velocity.Length2D();

	// circle strafe - cred: $qch
	if (GetAsyncKeyState(g_pSettings->MiscPrespeed->Key)) { 
		static float StrafeAngle;

		CBaseEntity *pLocal = G::LocalPlayer;
		Vector Velocity = pLocal->GetVelocity();
		Velocity.z = 0;

		float Speed = Velocity.Length();
		if (Speed < 45) Speed = 45;
		if (Speed > 750) Speed = 750;

		
		float FinalPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Vector(0, 0, 10) + Velocity / 2.0f);
		float DeltaAngle = fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / Interfaces->Globals->interval_per_tick)) * 4.f, 2.0f);
		StrafeAngle += DeltaAngle;

		if (fabs(StrafeAngle) >= 360.0f) {
			StrafeAngle = 0.0f;
		}
		else {
			G::UserCmd->forwardmove = cos((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
			G::UserCmd->sidemove = sin((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
		}

		return;
	}

	auto ideal_move_angle = RAD2DEG(std::atan2(15.f, speed));
	std::clamp(ideal_move_angle, 0.f, 90.f);

	auto yaw_delta = Math::NormalizeFloat(G::UserCmd->viewangles.y - m_previous_yaw);
	auto abs_yaw_delta = abs(yaw_delta);
	m_circle_yaw = m_previous_yaw = G::UserCmd->viewangles.y;

	if (yaw_delta > 0.f)
		G::UserCmd->sidemove = -450.f;

	else if (yaw_delta < 0.f)
		G::UserCmd->sidemove = 450.f;

	if (abs_yaw_delta <= ideal_move_angle || abs_yaw_delta >= 30.f) {
		QAngle velocity_angles;
		Math::VectorAngles(velocity, velocity_angles);

		auto velocity_angle_yaw_delta = Math::NormalizeFloat(G::UserCmd->viewangles.y - velocity_angles.y);
		auto velocity_degree = get_angle_from_speed(speed) * 2/*menu->main.misc.retrack_speed.value()*/;

		if (velocity_angle_yaw_delta <= velocity_degree || speed <= 15.f) {
			if (-(velocity_degree) <= velocity_angle_yaw_delta || speed <= 15.f) {

				if (g_pSettings->MiscAutostrafe->SelectedIndex == 2)
					G::UserCmd->viewangles.y += (ideal_move_angle * turn_direction_modifier);
	
				G::UserCmd->sidemove = 450.f * turn_direction_modifier;
			}

			else {

				if (g_pSettings->MiscAutostrafe->SelectedIndex == 2)
					G::UserCmd->viewangles.y = velocity_angles.y - velocity_degree;
	
				G::UserCmd->sidemove = 450.f;
			 }
		}

		else {

			if (g_pSettings->MiscAutostrafe->SelectedIndex == 2)
				G::UserCmd->viewangles.y = velocity_angles.y + velocity_degree;
		
			G::UserCmd->sidemove = -450.f;
		}
	}
}

void CMiscellaneous::Bunnyhop() {
	static ConVar* sv_autobunnyhopping = Interfaces->Cvar->FindVar("sv_autobunnyhopping");

	if (sv_autobunnyhopping->GetInt() == 1)
		return;

	if (!(G::UserCmd->buttons & IN_JUMP))	
		return;

	if (!(G::LocalPlayer->GetFlags() & FL_ONGROUND))
		G::UserCmd->buttons &= ~IN_JUMP;
}

void CMiscellaneous::Fakelag() 
{
	//std::vector<std::string>{"disabled", "static", "adaptive", "dynamic", "randomized"}
	bool in_lbyjitter = ShouldFake(); // choke a tick if about to flick

	int variance = g_pSettings->MiscFakelagJitter->value;
	int max_choke = g_pSettings->MiscFakelagFactor->value;

	static int choked_ticks = 0;
	static int to_choke = 7;

	if (g_pSettings->MiscFakelag->SelectedIndex)
	{
		 /* lifestyle, lifestyle */
		
		switch (g_pSettings->MiscFakelag->SelectedIndex)
		{
			case 1:
			{
				to_choke = max_choke; // static
			}break;

			case 2:
			{
				static int old_health = G::LocalPlayer->GetHealth();
				if (old_health != G::LocalPlayer->GetHealth())
				{
					old_health = G::LocalPlayer->GetHealth();
					to_choke = Math::RandomInt(0, max_choke);
				}
			}break;

			case 3:
			{
				to_choke = max(1, min((int)(fabs(G::LocalPlayer->GetVelocity().Length() / 80.f)), max_choke));
			}break;

			case 4:
			{
				to_choke = Math::RandomInt(0, max_choke);
			}
		}

		bool bRandom = Math::RandomInt(0, 1) == 0;
		int variance = Math::RandomInt(0, variance);

		to_choke += bRandom ? variance : -(variance);
		to_choke = clamp(to_choke, 0, max_choke);
		
		if (choked_ticks <= to_choke && !in_lbyjitter)
		{
			G::SendPacket = false;
			choked_ticks++;
		}
		else
		{
			G::SendPacket = true;
			choked_ticks = 0;
		}
	}

	else
	{
		choked_ticks = 0;
	}
}

void TraceKnife(bool stab, trace_t* tr) {
	// Attack range of knife attack
	float range = stab ? 32.0f : 48.0f;

	// Calculte end spot
	Vector myPos = G::LocalPlayer->GetEyePosition();
	Vector vecAim;
	Math::AngleVectors(G::UserCmd->viewangles, vecAim);
	Vector end = myPos + vecAim * range;

	// Trace attack
	Ray_t ray;
	ray.Init(G::LocalPlayer->GetEyePosition(), end);
	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;
	Interfaces->EngineTrace->TraceRay(ray, 0x46004003, &filter, tr);

	// If that failed, try a hull trace
	if (tr->fraction >= 1.0f) {
		static const Vector mins = Vector(-16.0f, -16.0f, -18.0f);
		static const Vector maxs = Vector(16.0f, 16.0f, 18.0f);

		ray.Init(G::LocalPlayer->GetEyePosition(), end, mins, maxs);

		Interfaces->EngineTrace->TraceRay(ray, 0x200400B, &filter, tr);
	}
}

bool IsBehind(CBaseEntity* pTarget) {
	//Vector vecLOS = pTarget->CalcAbsolutePosition() - vars.vecMe;
	Vector vecLOS = pTarget->GetAbsOrigin() - G::LocalPlayer->GetAbsOrigin();
	vecLOS.z = 0.0f;
	vecLOS.Normalize();

	Vector vTargetForward;
	QAngle angAbsRotation = pTarget->GetAbsAngles();
	Math::AngleVectors(pTarget->GetAbsAngles(), vTargetForward);
	vTargetForward.z = 0.0f; // BUG? vTargetForward.AsVector2D() isn't normalized.

							 // CSS:0.8, CSGO:0.475
	return (vecLOS.Dot(vTargetForward) > 0.475f);
}

void CMiscellaneous::KnifeTrigger() {
	trace_t tr;

	TraceKnife(false, &tr);

	if (!tr.m_pEnt || !tr.m_pEnt->IsPlayer() || !tr.m_pEnt->IsEnemy())
		return; //don't attack

	CBaseEntity* pl = tr.m_pEnt;
	CBaseCombatWeapon* pWeapon = G::LocalPlayer->GetWeapon();


	if (!pWeapon || !pWeapon->IsKnife())
		return;
	//------------------------------------------------
	// Swing and stab damage

	struct table_t {
		// [first][armor][back]
		unsigned char swing[2][2][2];
		// [armor][back]
		unsigned char stab[2][2];
	};
	static const table_t table = {
		{ { { 25,90 },{ 21,76 } },{ { 40,90 },{ 34,76 } } },
		{ { 65,180 },{ 55,153 } }
	};

	bool armor = (pl->GetArmor() > 0);
	bool first = pWeapon->GetNextPrimaryAttack() + 0.4f < Interfaces->Globals->curtime; //fuck prediction
	bool back = IsBehind(pl);

	int stab_dmg = table.stab[armor][back];
	int slash_dmg = table.swing[first][armor][back]; //first slash, then swings
	int swing_dmg = table.swing[false][armor][back];

	int health = pl->GetHealth();
	bool stab;

	if (health <= slash_dmg) stab = false;
	else if (health <= stab_dmg) stab = true;
	else if (health > (slash_dmg + swing_dmg + stab_dmg)) stab = true;
	else stab = false;

	G::UserCmd->buttons |= stab ? IN_ATTACK2 : IN_ATTACK;
}

void CMiscellaneous::FixMovement(QAngle& wish_angle) { // s/o: d3dcom
	float normalizedx = fmod(G::UserCmd->viewangles.x + 180.f, 360.f) - 180.f;

	Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	auto viewangles = G::UserCmd->viewangles;
	viewangles.Normalize();

	Math::AngleVectors(wish_angle, &view_fwd, &view_right, &view_up);
	Math::AngleVectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

	float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
	float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
	float v12 = sqrtf(view_up.z * view_up.z);

	Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
	Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
	Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

	float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
	float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
	float v18 = sqrtf(cmd_up.z * cmd_up.z);

	Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
	Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
	Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

	float v22 = norm_view_fwd.x * G::UserCmd->forwardmove;
	float v26 = norm_view_fwd.y * G::UserCmd->forwardmove;
	float v28 = norm_view_fwd.z * G::UserCmd->forwardmove;
	float v24 = norm_view_right.x * G::UserCmd->sidemove;
	float v23 = norm_view_right.y * G::UserCmd->sidemove;
	float v25 = norm_view_right.z * G::UserCmd->sidemove;
	float v30 = norm_view_up.x * G::UserCmd->upmove;
	float v27 = norm_view_up.z * G::UserCmd->upmove;
	float v29 = norm_view_up.y * G::UserCmd->upmove;

	G::UserCmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
		+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
		+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
	G::UserCmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
		+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
		+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
	G::UserCmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
		+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
		+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

	G::UserCmd->forwardmove = std::clamp(G::UserCmd->forwardmove, -450.f, 450.f);
	G::UserCmd->sidemove = std::clamp(G::UserCmd->sidemove, -450.f, 450.f);
	G::UserCmd->upmove = std::clamp(G::UserCmd->upmove, -320.f, 320.f);

	if (normalizedx >= 90.0f || normalizedx <= -90.0f || (G::UserCmd->viewangles.x >= 90.f && G::UserCmd->viewangles.x <= 200) || G::UserCmd->viewangles.x <= -90)
	{
		if (G::UserCmd->forwardmove > 0 || G::UserCmd->forwardmove < 0)
		{
			G::UserCmd->forwardmove = -G::UserCmd->forwardmove;
		}
	}

	//G::UserCmd->sidemove = sin(flYaw) * flSpeed;
}

void CMiscellaneous::AntiAim() {
	if (!G::LocalPlayer || !G::LocalPlayer->GetWeapon())
		return;
	bool can_shoot = (G::LocalPlayer->GetWeapon()->GetNextPrimaryAttack() <= (G::LocalPlayer->GetTickBase() * Interfaces->Globals->interval_per_tick));
	if (!(G::UserCmd->buttons & IN_USE) && !(G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER) && !(G::LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP)) { //shooting or pressing E
		G::InAntiAim = true;
		G::Aimbotting = false;
		Antiaim->Run();
	}
	else
	{
		G::InAntiAim = false;
	}
}
