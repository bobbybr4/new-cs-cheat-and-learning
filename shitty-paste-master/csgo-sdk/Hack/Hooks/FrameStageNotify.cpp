#include "../../csgo-sdk.h"

std::vector<const char*> smoke_materials = {
	//"effects/overlaysmoke",
	"particle/beam_smoke_01",
	"particle/particle_smokegrenade",
	"particle/particle_smokegrenade1",
	"particle/particle_smokegrenade2",
	"particle/particle_smokegrenade3",
	"particle/particle_smokegrenade_sc",
	"particle/smoke1/smoke1",
	"particle/smoke1/smoke1_ash",
	"particle/smoke1/smoke1_nearcull",
	"particle/smoke1/smoke1_nearcull2",
	"particle/smoke1/smoke1_snow",
	"particle/smokesprites_0001",
	"particle/smokestack",
	"particle/vistasmokev1/vistasmokev1",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_nearcull",
	"particle/vistasmokev1/vistasmokev1_nearcull_fog",
	"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev4_emods_nocull",
	"particle/vistasmokev1/vistasmokev4_nearcull",
	"particle/vistasmokev1/vistasmokev4_nocull"
};

void ToggleSky() {
	if (!Interfaces->EngineClient->IsInGame() || !Interfaces->EngineClient->IsConnected() || !G::LocalPlayer)
	{
		return;
	}

	static float oldclrs[2048][3];
	// bool is_aim_map = strstr("aim", Interfaces->EngineClient->GetLevelName());

	for (MaterialHandle_t i = Interfaces->MaterialSystem->FirstMaterial(); i != Interfaces->MaterialSystem->InvalidMaterial(); i = Interfaces->MaterialSystem->NextMaterial(i)) {
		IMaterial* pMaterial = Interfaces->MaterialSystem->GetMaterial(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->GetTextureGroupName(), "SkyBox textures")) {
			if (oldclrs[i][0] == 0.f && oldclrs[i][1] == 0.f && oldclrs[i][2] == 0.f)
				pMaterial->GetColorModulation(&oldclrs[i][0], &oldclrs[i][1], &oldclrs[i][2]);
			float col[3] = { 0, 0, 0 };
			if (!g_pSettings->VisualsAsusMode->Checked) {
				if (g_pSettings->VisualsBrightnessAdjust->Checked)
				{
					/* night-mode sky */
					for (auto k = 0; k < 3; ++k)
					{
						col[k] = 0.30f;
					}
				}
				else
				{
					col[0] = oldclrs[i][0];
					col[1] = oldclrs[i][1];
					col[2] = oldclrs[i][2];
				}
			}

			pMaterial->ColorModulate(col[0], col[1], col[2]);
			//Interfaces->ModelRender->ForcedMaterialOverride(pMaterial);
		}
	}
}

void NightMode()
{
	static bool performed = true;
	static bool last_setting = false;
	//static const char* last_sky = "";

	if (!Interfaces->EngineClient->IsInGame() || !Interfaces->EngineClient->IsConnected() || !G::LocalPlayer)
	{
		return;
	}

	if (!performed)
	{
		bool is_aim_map = strstr("aim", Interfaces->EngineClient->GetLevelName());

		//auto LoadNamedSky = reinterpret_cast< void(__fastcall*)(const char*) >(Util::FindPattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
		// static ConVar* sv_skyname = Interfaces->Cvar->FindVar("sv_skyname");

		for (auto i = Interfaces->MaterialSystem->FirstMaterial(); i != Interfaces->MaterialSystem->InvalidMaterial(); i = Interfaces->MaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = Interfaces->MaterialSystem->GetMaterial(i);

			if (!pMaterial)
			{
				return;
			}

			if (strstr(pMaterial->GetTextureGroupName(), "World") || strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (last_setting)
				{
					if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
					{
						pMaterial->ColorModulate(0.30, 0.30, 0.30);
					}
					else
					{							
						pMaterial->ColorModulate(0.10, 0.10, 0.10);
					}
				}
				else
				{
					pMaterial->ColorModulate(1.0, 1.0, 1.0);
				}
			}
		}

		ToggleSky();
		performed = true;
	}

	if (last_setting != g_pSettings->VisualsBrightnessAdjust->Checked)
	{
		last_setting = g_pSettings->VisualsBrightnessAdjust->Checked;
		performed = false;
	}
}

void AsusWalls()
{
	static bool performed = true;
	static int last_setting = false;

	if (!Interfaces->EngineClient->IsInGame() || !Interfaces->EngineClient->IsConnected() || !G::LocalPlayer)
	{
		return;
	}

	if (!performed)
	{
		bool is_aim_map = strstr("maps/aim", Interfaces->EngineClient->GetLevelName());
		float blend = g_pSettings->VisualsAsusBlend->value / 100.0f;

		for (auto i = Interfaces->MaterialSystem->FirstMaterial(); i != Interfaces->MaterialSystem->InvalidMaterial(); i = Interfaces->MaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = Interfaces->MaterialSystem->GetMaterial(i);

			if (!pMaterial)
			{
				return;
			}

			if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				if (last_setting)
				{
					pMaterial->AlphaModulate(blend);		
				}
				else
				{
					pMaterial->AlphaModulate(1.0f);
				}
			}
		}

		performed = true;
	}

	if (last_setting != g_pSettings->VisualsAsusBlend->value)
	{
		last_setting = g_pSettings->VisualsAsusBlend->value;
		performed = false;
	}
}

FrameStageNotifyFn oFrameStageNotify;
void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t stage) {
	QAngle aim_punch_old;
	QAngle view_punch_old;

	QAngle* aim_punch = nullptr;
	QAngle* view_punch = nullptr;


	if (!(G::LocalPlayer && Interfaces->EngineClient->IsInGame() && Interfaces->EngineClient->IsConnected()))
		return oFrameStageNotify(stage);

	if (stage == FRAME_RENDER_START) {
		static ConVar* r_DrawSpecificStaticProp = Interfaces->Cvar->FindVar("r_DrawSpecificStaticProp");
		static SpoofedConvar* d_spoofed = new SpoofedConvar(r_DrawSpecificStaticProp);
		d_spoofed->SetBool(true);

		if (g_pSettings->VisualsRemovalParticles->Checked) {
			*(bool*)offsets->s_bOverridePostProcessingDisable = true;
		}
		else
			*(bool*)offsets->s_bOverridePostProcessingDisable = false;

		if (!Interfaces->Input->m_bCameraInThirdPerson && g_pSettings->VisualsThirdPerson->Checked && G::LocalPlayer->IsAlive() && !(G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsGrenade()))
		{
			Util::ToggleThirdPerson(true);
		}
		else if(!g_pSettings->VisualsThirdPerson->Checked || !G::LocalPlayer->IsAlive() || (G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsGrenade()))
		{
			Util::ToggleThirdPerson(false);
		}

		if (g_pSettings->VisualsRemovalFlash->Checked &&  G::LocalPlayer->GetFlashDuration() > 0)
			*(float*)((uintptr_t)G::LocalPlayer + offsets->m_flFlashDuration) = 0.f;

		if (g_pSettings->VisualsRemovalRecoil->Checked) {
			aim_punch = (QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_aimPunchAngle);
			view_punch = (QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_viewPunchAngle);

			aim_punch_old = *aim_punch;
			view_punch_old = *view_punch;

			*aim_punch = QAngle(0.f, 0.f, 0.f);
			*view_punch = QAngle(0.f, 0.f, 0.f);
		}
		
		if (*(bool*)((uintptr_t)Interfaces->Input + 0xA5))   //Check for thirdperson
			*(QAngle*)((uintptr_t)G::LocalPlayer + offsets->m_fsnViewAngles) = G::VisualAngle;  //deadflag netvar + 4

		for (auto material_name : smoke_materials) {
			IMaterial* mat = Interfaces->MaterialSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, g_pSettings->VisualsRemovalSmoke->Checked ? true : false);
		}

		if (g_pSettings->VisualsRemovalSmoke->Checked) {
			static int* smokecount = *(int**)(Util::FindPattern("client.dll", "A3 ? ? ? ? 57 8B CB") + 0x1);
			*smokecount = 0;
		}

		static bool bOldSkyEnable = false;

		if (bOldSkyEnable != g_pSettings->VisualsAsusMode->Checked) {
			ToggleSky();
			bOldSkyEnable = g_pSettings->VisualsAsusMode->Checked;
		}

		NightMode();
		AsusWalls();

		for (int i = 1; i < 64; i++) {
			CBaseEntity* pEntity = Interfaces->ClientEntityList->GetClientEntity(i);

			if (!pEntity)
				continue;

			if (!g_pSettings->RagebotFriendly->Checked && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
				continue;

			//Backtracking->OverridePosesFsnRenderStart(pEntity);
			//Backtracking->ShotBackTrackStoreFSN(pEntity);
			Resolver->Store(pEntity);
			LagCompResolver->BeginLagCompensation(pEntity);
		}
	}


	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		unsigned long g_iModelBoneCounter = **(unsigned long**)(offsets->InvalidateBoneCacheEx + 10);

		for (int i = 1; i < 64; i++) {
			CBaseEntity* pEntity = Interfaces->ClientEntityList->GetClientEntity(i);

			if (!pEntity)
				continue;

			if (!g_pSettings->RagebotFriendly->Checked && pEntity->GetTeam() == G::LocalPlayer->GetTeam())
				continue;

			if (g_pSettings->RagebotPositionAdjustment->SelectedIndex > 0)
				Lagcompensation->DisableInterpolation(pEntity);

			if (g_pSettings->RagebotPositionAdjustment->SelectedIndex == 2)
			{
				pEntity->UpdateClientSideAnimation();

				*(int*)((DWORD)pEntity + (offsets->m_nForceBone + 0x20)) = 0;
				*(unsigned int*)((DWORD)pEntity + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
				*(unsigned int*)((DWORD)pEntity + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
			}

			Resolver->Resolve(pEntity);
			LagCompResolver->FinishLagCompensation(pEntity);
			if (GetAsyncKeyState(g_pSettings->RagebotBruteforce->Key))
			{
				pEntity->m_angEyeAngles().y += 180.0f;
			}
		}
	}

	oFrameStageNotify(stage);

	if (g_pSettings->VisualsRemovalRecoil->Checked && G::LocalPlayer && aim_punch && view_punch)
	{
		*aim_punch = aim_punch_old;
		*view_punch = view_punch_old;
	}
	

	if (stage == FRAME_NET_UPDATE_END) {

	}
}
