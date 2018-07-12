#include "csgo-sdk.h"

IsPlayingTimeDemoFn oIsPlayingTimeDemo;

bool __fastcall Hooks::IsPlayingTimeDemo(void* thisptr, void* edx)
{
	if (Interfaces->EngineClient->IsConnected() && Interfaces->EngineClient->IsInGame() && G::LocalPlayer)
	{
		if (g_pSettings->RagebotEnabled->Checked)
		{
			return true;
		}
	}

	return oIsPlayingTimeDemo(thisptr);
}