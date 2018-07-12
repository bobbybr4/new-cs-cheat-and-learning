#include "csgo-sdk.h"

IsConnectedFn oIsConnected;

bool __stdcall Hooks::IsConnected() // credit: kidua
{
	static DWORD dwEnableInventory = Util::FindPattern("client.dll", "84 C0 75 04 B0 01 5F");

	if (dwEnableInventory == DWORD(_ReturnAddress()))
	{
		return false;
	}

	return oIsConnected(Interfaces->EngineClient);
}