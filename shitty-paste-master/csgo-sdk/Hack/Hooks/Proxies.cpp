#include "../../csgo-sdk.h"

RecvVarProxyFn oLowerBodyYawTargetProxy;
void Hooks::LowerBodyYawTargetProxy(const CRecvProxyData* pData, void* pStruct, void* pOut) {
	CBaseEntity* ent = reinterpret_cast<CBaseEntity*>(pStruct);

	oLowerBodyYawTargetProxy(pData, pStruct, pOut);
}
