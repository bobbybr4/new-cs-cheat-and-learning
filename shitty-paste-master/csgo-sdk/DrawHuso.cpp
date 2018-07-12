#include "csgo-sdk.h"
#include "MaterialHelper.h"

DrawHuso_t oDrawHuso;

/*
to-do:
- implement player chams
- arm chams
- weapon chams
- wireframe additive possibly lul
*/

/* disabled / flat / textured / wireframe / glass / animated */

void PlayerChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights,
	float *pFlexDelayedWeights, const Vector &modelOrigin, int flags);

void HandChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights,
	float *pFlexDelayedWeights, const Vector &modelOrigin, int flags);

void WeaponChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights,
	float *pFlexDelayedWeights, const Vector &modelOrigin, int flags);

void __fastcall Hooks::DrawHuso(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights,
	float *pFlexDelayedWeights, const Vector &modelOrigin, int flags)
{
	if (!Interfaces->EngineClient->IsConnected() || !Interfaces->EngineClient->IsInGame() || !G::LocalPlayer)
	{
		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		return;
	}

	const char* model_name = info.m_pStudioHdr->name;

	if (strstr(model_name, "shadow"))
	{
		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		return;
	}
	else if (strstr(model_name, "player") && !(strstr(model_name, "weapon")))
	{
		PlayerChams(thisptr, edx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}
	else if (strstr(model_name, "arms"))
	{
		HandChams(thisptr, edx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}
	else if (strstr(model_name, "weapon"))
	{
		if(!strstr(model_name, XorStr("v_")) && !strstr(model_name, XorStr("arms")))
			WeaponChams(thisptr, edx, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}

	oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
}

void PlayerChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags)
{
	if (!g_pSettings->VisualsEnabled->Checked || !g_pSettings->VisualsChams->SelectedIndex)
		return;

	CBaseEntity* ent = reinterpret_cast<CBaseEntity*>(info.m_pClientEntity);
	
	if (!ent || ent->IsDormant() || ent == G::LocalPlayer || !ent->IsAlive())
		return;

	if (!g_pSettings->VisualsFriends->Checked && !ent->IsEnemy())
		return;

	if (!g_pSettings->VisualsEnemies->Checked && ent->IsEnemy())
		return;

	Color visible = ent->IsEnemy() ? Color(g_pSettings->ColorEnemyVisibleChams->color) : Color(g_pSettings->ColorTeamVisibleChams->color);
	Color invisible = ent->IsEnemy() ? Color(g_pSettings->ColorEnemyInvisibleChams->color) : Color(g_pSettings->ColorTeamInvisibleChams->color);

	if (!g_pSettings->VisualsLegit->Checked)
	{
		Interfaces->StudioRender->SetColorModulation(invisible.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->CoveredFlat);
		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}

	Interfaces->StudioRender->SetColorModulation(visible.Base());
	Interfaces->StudioRender->SetAlphaModulation(1.0f);
	Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->OpenFlat);
	oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
}

/*"disabled", "textured", "material", "flat", "golden"*/

void HandChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags)
{
	if (!g_pSettings->VisualsArmChams->SelectedIndex)
		return;

	Color col = Color(g_pSettings->ColorHands->color);
	Color col_wireframe = Color(col.r() * 0.8, col.g() * 0.8, col.b() * 0.8);

	switch (g_pSettings->VisualsArmChams->SelectedIndex) /* there must be a better more optimized way to do this? */
	{
	case 0: //wtf 
		return; break;
	case 1: // textured
	{
		Interfaces->StudioRender->SetColorModulation(col.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->OpenLit);
	}break;

	case 2: // material
	{
		/*IMaterial* skins[MAXSTUDIOSKINS];
		CBaseEntity* model_entity = reinterpret_cast<CBaseEntity*>(info.m_pClientEntity);
		
		if (!model_entity)
			return;

		studiohdr_t* hdr = Interfaces->ModelInfo->GetStudioModel(model_entity->GetModel());
		Interfaces->ModelInfo->GetModelMaterials(model_entity->GetModel(), hdr->numtextures, skins);

		for (auto i = 0; i < hdr->numtextures; ++i)
		{
			if (!skins[i])
			{
				continue;
			}

			skins[i]->ColorModulate(col.rBase(), col.gBase(), col.bBase());
			skins[i]->AlphaModulate(col.aBase());
		}

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		
		for (auto i = 0; i < hdr->numtextures; ++i)
		{
			if (!skins[i])
			{
				continue;
			}

			skins[i]->ColorModulate(1.0f, 1.0f, 1.0f);
			skins[i]->AlphaModulate(1.0f);
		}*/
	}break;

	case 3: // flat
	{
		Interfaces->StudioRender->SetColorModulation(col.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->OpenFlat);
	}break;

	case 4: // golden
	{
		static IMaterial* gold = Interfaces->MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
		Interfaces->StudioRender->ForcedMaterialOverride(gold);

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

	}break;

	case 5: // glass
	{
		static IMaterial* glass = Interfaces->MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(glass);

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}break;
	}

	if (g_pSettings->VisualsArmWireframe->Checked)
	{
		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		Interfaces->StudioRender->SetColorModulation(col_wireframe.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->AddWire);
	}
}

void WeaponChams(void* thisptr, void* edx, DrawModelResults_t *pResults, const DrawModelInfo_t& info, matrix3x4_t *pBoneToWorld, float *pFlexWeights, float *pFlexDelayedWeights, const Vector &modelOrigin, int flags)
{
	if (!g_pSettings->VisualsWeaponChams->SelectedIndex)
		return;

	Color col = Color(g_pSettings->ColorWeapon->color);
	Color col_wireframe = Color(col.r() * 0.8, col.g() * 0.8, col.b() * 0.8);

	switch (g_pSettings->VisualsWeaponChams->SelectedIndex) /* there must be a better more optimized way to do this? */
	{
	case 0: //wtf 
		return; break;
	case 1: // textured
	{
		Interfaces->StudioRender->SetColorModulation(col.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->OpenLit);
	}break;

	case 2: // material
	{
		/*IMaterial* skins[MAXSTUDIOSKINS];
		CBaseEntity* model_entity = reinterpret_cast<CBaseEntity*>(info.m_pClientEntity);

		if (!model_entity)
			return;

		studiohdr_t* hdr = Interfaces->ModelInfo->GetStudioModel(model_entity->GetModel());
		Interfaces->ModelInfo->GetModelMaterials(model_entity->GetModel(), hdr->numtextures, skins);

		for (auto i = 0; i < hdr->numtextures; ++i)
		{
			if (!skins[i])
			{
				continue;
			}

			skins[i]->ColorModulate(col.rBase(), col.gBase(), col.bBase());
			skins[i]->AlphaModulate(col.aBase());
		}

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

		for (auto i = 0; i < hdr->numtextures; ++i)
		{
			if (!skins[i])
			{
				continue;
			}

			skins[i]->ColorModulate(1.0f, 1.0f, 1.0f);
			skins[i]->AlphaModulate(1.0f);
		}*/
	}break;

	case 3: // flat
	{
		Interfaces->StudioRender->SetColorModulation(col.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->OpenFlat);
	}break;

	case 4: // golden
	{
		static IMaterial* gold = Interfaces->MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER);
		Interfaces->StudioRender->ForcedMaterialOverride(gold);

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);

	}break;

	case 5: // glass
	{
		static IMaterial* glass = Interfaces->MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(glass);

		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
	}break;
	}

	if (g_pSettings->VisualsWeaponWireframe->Checked)
	{
		oDrawHuso(thisptr, pResults, info, pBoneToWorld, pFlexWeights, pFlexDelayedWeights, modelOrigin, flags);
		Interfaces->StudioRender->SetColorModulation(col_wireframe.Base());
		Interfaces->StudioRender->SetAlphaModulation(1.0f);
		Interfaces->StudioRender->ForcedMaterialOverride(g_pMaterialHelper->AddWire);
	}
}
