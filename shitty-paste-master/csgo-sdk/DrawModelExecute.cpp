#include "csgo-sdk.h"
#include "MaterialHelper.h"

DrawModelExecuteFn oDrawModelExecute;

/*
	to-do:
		- implement player chams
		- arm chams
		- weapon chams
		- wireframe additive possibly lul
*/

/* disabled / flat / textured / wireframe / glass / animated */

/*
	then:
		- implement night-mode (also modify sky accordingly i think)
		- asus blend (only doors on non 'aim_' maps)
*/

void __stdcall Hooks::DrawModelExecute(void* context, void* state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld) // hhhhhhhhh phooked but crashes me in fsn lach
{
	/*static bool bOldSkyEnable = false;

	if (bOldSkyEnable != g_pSettings->VisualsAsusMode->Checked) {
		ToggleSky();
		bOldSkyEnable = g_pSettings->VisualsAsusMode->Checked;
	}

	WallModification();*/

	oDrawModelExecute(context, state, pInfo, pCustomBoneToWorld);
	Interfaces->ModelRender->ForcedMaterialOverride(NULL);
}
