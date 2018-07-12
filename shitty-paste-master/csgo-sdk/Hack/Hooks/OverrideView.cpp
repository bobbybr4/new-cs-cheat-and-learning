#include "../../csgo-sdk.h"

OverrideViewFn oOverrideView;
void __stdcall Hooks::OverrideView(CViewSetup* vsView) {
	if (!Interfaces->EngineClient->IsInGame() || !Interfaces->EngineClient->IsConnected()
		|| !G::LocalPlayer)
	{
		oOverrideView(vsView);
		return;
	}

	if (G::LocalPlayer && G::LocalPlayer->GetWeapon() && !g_pSettings->VisualsThirdPerson->Checked)
		vsView->fov += (float)g_pSettings->VisualsOverrideFOV->value;

	if (G::LocalPlayer && G::LocalPlayer->GetWeapon() && G::LocalPlayer->GetWeapon()->IsSniper() &&
		G::LocalPlayer->GetWeapon()->GetZoomLevel() && g_pSettings->VisualsThirdPerson->Checked)
		vsView->fov = 90.0f;

	if (g_pSettings->VisualsEnabled->Checked && g_pSettings->VisualsGrenadeTrajectory->Checked)
		GrenadePrediction->PredictGrenade();

	oOverrideView(vsView);
}
