#include "csgo-sdk.h"
GetLastTimeStampFn oGetLastTimeStamp;

float __fastcall Hooks::GetLastTimeStamp(void* ecx, void*)
{
	if (Interfaces->EngineClient->IsConnected() && Interfaces->EngineClient->IsInGame() && G::LocalPlayer)
	{
		if (g_pSettings->RagebotEnabled->Checked)
		{
			void* m_bIsInterpolating = _ReturnAddress();

			if (DWORD(m_bIsInterpolating) == offsets->SetLastTimeStampInterpolate)
			{
				*reinterpret_cast< BYTE*>(offsets->bInterpolate) = NULL;
			}
		}
	}

	return oGetLastTimeStamp(ecx);
}