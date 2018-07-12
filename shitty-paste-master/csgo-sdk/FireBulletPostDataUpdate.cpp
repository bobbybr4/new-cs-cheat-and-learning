#include "csgo-sdk.h"

FireBullets_t oFireBullet;

void __stdcall HOOKED_FireBulletPostDataUpdate(C_TEFireBullets* thisptr, DataUpdateType_t updateType)
{
	int m_iIndex = thisptr->m_iPlayer + 1;

	CBaseEntity* entity = Interfaces->ClientEntityList->GetClientEntity(m_iIndex);

	/* to-do: resolving shit idk idc xD */
	//Resolver->StoreShot(entity);

	oFireBullet(thisptr, updateType);
}

void __stdcall Hooks::DETOUR_FireBulletPostDataUpdate(DataUpdateType_t updateType)
{
	__asm
	{
		push[esp + 4]
		push ecx
		call HOOKED_FireBulletPostDataUpdate
		retn 4
	}
}