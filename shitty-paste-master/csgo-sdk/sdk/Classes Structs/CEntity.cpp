#include "../../csgo-sdk.h"

int	CBaseEntity::GetHealth() {
	return *(int*)((uintptr_t)this + offsets->m_iHealth);
}

int CBaseEntity::GetTeam() {
	return *(int*)((uintptr_t)this + offsets->m_iTeamNum);
}

int CBaseEntity::GetFlags() {
	return *(int*)((uintptr_t)this + offsets->m_fFlags);
}

int CBaseEntity::GetTickBase() {
	return *(int*)((uintptr_t)this + offsets->m_nTickBase);
}

int CBaseEntity::GetShotsFired() {
	return *(int*)((uintptr_t)this + offsets->m_iShotsFired);
}

int CBaseEntity::GetCrosshairId() {
	return *(int*)((uintptr_t)this + offsets->m_iCrosshairId);
}

int CBaseEntity::GetMoveType() {
	return *(int*)((uintptr_t)this + offsets->m_nMoveType);
}

float CBaseEntity::GetOldSimulationTime()
{
	return *(float*)((DWORD)this + offsets->m_flSimulationTime + 0x4);
}

int CBaseEntity::GetModelIndex() {
	return *(int*)((uintptr_t)this + offsets->m_nModelIndex);
}

int CBaseEntity::GetHitboxSet() {
	return *(int*)((uintptr_t)this + offsets->m_nHitboxSet);
}

int CBaseEntity::GetUserID() {
	return this->GetPlayerInfo().userid;
}

int CBaseEntity::GetArmor() {
	return *(int*)((uintptr_t)this + offsets->m_ArmorValue);
}

int CBaseEntity::PhysicsSolidMaskForEntity() {
	typedef unsigned int(__thiscall* OriginalFn)(void*);
	return Util::GetVFunc<OriginalFn>(this, 154)(this);
}

CBaseHandle CBaseEntity::GetOwnerEntity() {
	return *(CBaseHandle*)((uintptr_t)this + offsets->m_hOwnerEntity);
}

IHandleEntity* CBaseHandle::Get() const { return Interfaces->ClientEntityList->GetClientEntityFromHandle(m_Index & 0xFFF); };

int CBaseEntity::GetGlowIndex() {
	return *(int*)((uintptr_t)this + offsets->m_iGlowIndex);
}

float CBaseEntity::GetBombTimer() {
	float bombTime = *(float*)((uintptr_t)this + offsets->m_flC4Blow);
	float returnValue = bombTime - Interfaces->Globals->curtime;
	return (returnValue < 0) ? 0.f : returnValue;
}

float CBaseEntity::GetFlashDuration() {
	return *(float*)((uintptr_t)this + offsets->m_flFlashDuration);
}

bool CBaseEntity::IsFlashed() {
	return GetFlashDuration() > 0 ? true : false;
}

bool CBaseEntity::IsPlayer() {
	return Util::GetVFunc<bool(__thiscall*)(CBaseEntity*), 152>(this)(this);
}

bool CBaseEntity::IsAlive() {
	return (bool)(*(int*)((uintptr_t)this + offsets->m_lifeState) == 0);
}

bool CBaseEntity::GetDormant() {
	return *(bool*)((uintptr_t)this + offsets->m_bDormant);
}

bool CBaseEntity::GetImmune() {
	return *(bool*)((uintptr_t)this + offsets->m_bGunGameImmunity);
}

bool CBaseEntity::HasHelmet() {
	return *(bool*)((uintptr_t)this + offsets->m_bHasHelmet);
}

model_t* CBaseEntity::GetModel() {
	if (!this)
		return nullptr;

	return *(model_t**)((uintptr_t)this + 0x6C);
	Util::GetVFunc<model_t*(__thiscall*)(void*)>(this, 8)(this);
}

bool CBaseEntity::IsEnemy() {
	return this->GetTeam() != G::LocalPlayer->GetTeam();
}

std::array<float, 24> CBaseEntity::GetPoseParameters() {
	return *(std::array<float, 24>*)((uintptr_t)this + offsets->m_flPoseParameter);
}

void CBaseEntity::SetPoseParameters(std::array<float, 24> arr) {
	*(std::array<float, 24>*)((uintptr_t)this + offsets->m_flPoseParameter) = arr;
}

float CBaseEntity::GetCycle() {
	if (!this)
		return 0.0f;
	return *(float*)(uintptr_t(this) + offsets->m_flCycle);
}

int CBaseEntity::GetSequence() {
	if (!this)
		return 0.0f;
	return *(int*)((uintptr_t)this + offsets->m_nSequence);
}

void CBaseEntity::UpdateClientSideAnimation() {
	Util::GetVFunc<void(__thiscall*)(void*)>(this, 218)(this);
}

bool CBaseEntity::IsVisible(Vector pos) {
	Ray_t ray;
	trace_t tr;
	ray.Init(G::LocalPlayer->GetEyePosition(), pos);

	CTraceFilter filter;
	filter.pSkip = G::LocalPlayer;

	Interfaces->EngineTrace->TraceRay(ray, MASK_VISIBLE, &filter, &tr);

	if (tr.m_pEnt == this) {
		return true;
	}

	return false;
}

bool CBaseEntity::IsVisible(int bone = HEAD_0) {
	return IsVisible(GetBonePosition(bone));
}

bool CBaseEntity::IsBroken() {
	return *(bool*)((uintptr_t)this + offsets->m_bIsBroken);
}

QAngle CBaseEntity::GetViewPunch() {
	return *(QAngle*)((uintptr_t)this + offsets->m_viewPunchAngle);
}

QAngle CBaseEntity::GetPunch() {
	return *(QAngle*)((uintptr_t)this + offsets->m_aimPunchAngle);
}

QAngle CBaseEntity::GetEyeAngles() {
	return *(QAngle*)((uintptr_t)this + offsets->m_angEyeAngles);
}

float CBaseEntity::GetLowerBodyYawTarget() {
	if (!this)
		return 0.0f;

	return *(float*)((uintptr_t)this + offsets->m_flLowerBodyYawTarget);
}

void CBaseEntity::SetAbsAngles(const QAngle& a) {
	this->m_angEyeAngles() = a;
}

void CBaseEntity::SetAbsOrigin(const Vector& ori) {
	((void(__thiscall*)(CBaseEntity*, const Vector&))offsets->SetAbsOriginEx)(this, ori);
}

Vector CBaseEntity::GetMins() {
	return *(Vector*)((uintptr_t)this + offsets->m_vecMins);
}

Vector CBaseEntity::GetMaxs() {
	return *(Vector*)((uintptr_t)this + offsets->m_vecMaxs);
}

Vector CBaseEntity::GetOrigin() {
	return *(Vector*)((uintptr_t)this + offsets->m_vecOrigin);
}

Vector CBaseEntity::GetViewOffset() {
	return *(Vector*)((uintptr_t)this + offsets->m_vecViewOffset);
}

Vector CBaseEntity::GetEyePosition() {
	Vector origin = this->GetOrigin();
	Vector offset = *(Vector*)((uintptr_t)this + offsets->m_vecViewOffset);

	return(origin + offset);

	this->GetAbsAngles();
}

Vector CBaseEntity::GetBonePosition(int iBone) {
	matrix3x4_t boneMatrixes[128];
	if (this->SetupBones(boneMatrixes, 128, 0x100, 0)) {
		matrix3x4_t boneMatrix = boneMatrixes[iBone];
		return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
	}
	return Vector(0, 0, 0);
}

bool CBaseEntity::ShouldCollide(int collisionGroup, int contentsMask) {
	return Util::GetVFunc<bool(__thiscall*)(CBaseEntity*, int, int)>(this, 167)(this, collisionGroup, contentsMask);
}

bool CBaseEntity::IsTransparent() {
	return Util::GetVFunc<bool(__thiscall*)(void*)>(this, 50)(this);
}

SolidType_t CBaseEntity::GetSolid() {
	return Util::GetVFunc<SolidType_t(__thiscall*)(void*), 81>(this)(this);
}

float CBaseEntity::GetSimulationTime() {
	if (!this)
		return 0.0f;

	return *(float*)((uintptr_t)this + offsets->m_flSimulationTime);
}

bool CBaseEntity::SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) {
	//if (currentTime = 0.f)
	//	currentTime = Interfaces->EngineClient->GetLastTimeStamp();

	//*(int*)((uintptr_t)this + offsets->m_nWritableBones) = 0;
	//*(int*)((uintptr_t)this + offsets->m_iDidCheckForOcclusion) = reinterpret_cast<int*>(offsets->m_dwOcclusionArray)[1];
	//reinterpret_cast<void(__fastcall*)(void*)>(offsets->InvalidateBoneCache)(ptr);
	*(int*)((uintptr_t)this + 0xA30) = Interfaces->Globals->framecount; //we'll skip occlusion checks now
	*(int*)((uintptr_t)this + 0xA28) = 0;//clear occlusion flags
	typedef bool(__thiscall* SetupBonesFn)(CBaseAnimating*, matrix3x4_t*, int, int, float);
	return Util::GetVFunc<SetupBonesFn>(this->GetBaseAnimating(), 13)(this->GetBaseAnimating(), pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

unsigned int CBaseEntity::GetObserverTarget() {
	if (!this)
		return NULL;

	return *(unsigned int*)((uintptr_t)this + offsets->m_hObserverTarget);
}

Vector CBaseEntity::GetVelocity() {
	return *(Vector*)((uintptr_t)this + offsets->m_vecVelocity);
}

Vector CBaseEntity::GetPredicted(Vector p0) {
	return Math::ExtrapolateTick(p0, this->GetVelocity());
}

ICollideable* CBaseEntity::GetCollideable() {
	if (!this)
		return nullptr;
	return (ICollideable*)((uintptr_t)this + offsets->m_Collision);
}

player_info_t CBaseEntity::GetPlayerInfo() {
	player_info_t pinfo;
	Interfaces->EngineClient->GetPlayerInfo(this->entindex(), &pinfo);
	return pinfo;
}

std::string	CBaseEntity::GetName() {
	if (!this)
		return nullptr;
	return this->GetPlayerInfo().name;
}

std::string	CBaseEntity::GetSteamID() {
	if (!this)
		return nullptr;

	return this->GetPlayerInfo().name;
}

std::string CBaseEntity::GetLastPlace() {
	if (!this)
		return nullptr;

	return std::string((const char*)this + offsets->m_szLastPlaceName);
}

CBaseCombatWeapon* CBaseEntity::GetWeapon() {
	if (!this)
		return nullptr;

	uintptr_t weaponData = *(uintptr_t*)((uintptr_t)this + offsets->m_hActiveWeapon);
	return (CBaseCombatWeapon*)Interfaces->ClientEntityList->GetClientEntityFromHandle(weaponData);
}

int CBaseEntity::GetCollisionGroup() {
	return *(int*)((uintptr_t)this + offsets->m_CollisionGroup);
}

void CBaseEntity::Precache(const char* path) {
	return;
	typedef void(__thiscall* OriginalFn)(void*, const char*);
	return Util::GetVFunc<OriginalFn>(this, 0)(this, path);
}

bool CBaseEntity::IsCrouching() {
	return GetFlags() & FL_DUCKING;
}

float CBaseEntity::GetDuckSpeed() {
	return *(float*)((uintptr_t)this + offsets->m_flDuckSpeed);
}

float CBaseEntity::GetDuckAmount() {
	return *(float*)((uintptr_t)this + offsets->m_flDuckAmount);
}


bool CBaseEntity::IsMovingOnGround() {
	return (this->GetVelocity().Length2D() > 75.0f && (this->GetFlags() & FL_ONGROUND));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int& CBaseCombatWeapon::GetWeaponID() {

	return *(int*)((uintptr_t)this + offsets->m_iItemDefinitionIndex);
}

float& CBaseCombatWeapon::GetNextPrimaryAttack() {
	return *(float*)((uintptr_t)this + offsets->m_flNextPrimaryAttack);
}

float& CBaseCombatWeapon::GetAccuracyPenalty() {
	return *(float*)((uintptr_t)this + offsets->m_fAccuracyPenalty);
}

int& CBaseCombatWeapon::GetItemDefinitionIndex() {
	return *(int*)((uintptr_t)this + offsets->m_iItemDefinitionIndex);
}

WeaponInfo_t* CBaseCombatWeapon::GetCSWpnData() {
	typedef WeaponInfo_t*(__thiscall* OriginalFn)(void*);
	return Util::GetVFunc<OriginalFn>(this, 446)(this);
}

bool CBaseCombatWeapon::IsEmpty() {
	int clip = *(int*)((uintptr_t)this + offsets->m_iClip1);
	return clip == 0;
}

bool CBaseCombatWeapon::IsReloading() {
	return *(bool*)((uintptr_t)this + 0x3235);
}

int	CBaseCombatWeapon::GetZoomLevel() {

	if (!this)
		return 0;

	return *(int*)((uintptr_t)this + offsets->m_zoomLevel);
}

void CBaseCombatWeapon::UpdateAccuracyPenalty() {
	typedef void(__thiscall *OriginalFn)(void*);
	Util::GetVFunc<OriginalFn>(this, 470)(this);
}

float CBaseCombatWeapon::GetWeaponCone() {
	typedef float(__thiscall* OriginalFn)(void*);
	return Util::GetVFunc<OriginalFn>(this, 469)(this);
}

float CBaseCombatWeapon::GetWeaponSpread() {
	typedef float(__thiscall* OriginalFn)(void*);
	return Util::GetVFunc<OriginalFn>(this, 439)(this);
}

float CBaseCombatWeapon::GetInaccuracy() {
	return this->GetWeaponCone();
}

int CBaseCombatWeapon::GetClipOne() {
	return *(int*)((uintptr_t)this + offsets->m_iClip1);
}

bool CBaseCombatWeapon::GetPinPulled() {
	return *(bool*)((uintptr_t)this + offsets->m_bPinPulled);
}

float CBaseCombatWeapon::GetThrowTime() {
	if (!this)
		return 0.0f;

	return *(float*)((uintptr_t)this + offsets->m_fThrowTime);
}

float CBaseCombatWeapon::GetFireReadyTime() {
	if (!this)
		return 0.0f;

	return *(float*)((uintptr_t)this + offsets->m_flPostponeFireReadyTime);
}

bool CBaseCombatWeapon::CanFirePostpone() {
	float firereadytime = this->GetFireReadyTime();

	if (firereadytime > 0 && firereadytime < (Interfaces->Globals->curtime))
		return true;

	return false;
}

bool CBaseCombatWeapon::IsInThrow() {
	if (!this->GetPinPulled() || (G::UserCmd->buttons & IN_ATTACK) || (G::UserCmd->buttons & IN_ATTACK2)) {
		float throwtime = this->GetThrowTime();

		if (throwtime > 0)
			return true;
	}

	return false;
}

bool CBaseCombatWeapon::IsGun() {
	if (!this)
		return false;

	int id = this->GetWeaponID();
	switch (id) {
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
		return true;
	case WEAPON_TASER:
		return true;
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
		return true;
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
		return false;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}

bool CBaseCombatWeapon::IsGrenade() {
	int id = this->GetWeaponID();

	switch (id) {
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_FLASHBANG:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
		return true;
	default:
		return false;
	}
}

bool CBaseCombatWeapon::IsSniper() {
	int id = this->GetWeaponID();

	switch (id) {
	case WEAPON_AWP:
	case WEAPON_SCAR20:
	case WEAPON_G3SG1:
	case WEAPON_SSG08:
		return true;
	default:
		return false;
	}
}

bool CBaseCombatWeapon::IsPistol() {
	int id = this->GetWeaponID();

	switch (id) {
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_USP_SILENCER:
	case WEAPON_P250:
	case WEAPON_TEC9:
	case WEAPON_HKP2000:
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
		return true;
	default:
		return false;
	}
}

bool CBaseCombatWeapon::IsKnife() {
	int id = this->GetWeaponID();

	switch (id) {
	case WEAPON_KNIFE:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
		return true;
	default:
		return false;
	}
}

std::string CBaseCombatWeapon::GetWeaponName() {

	typedef char* (__thiscall* tGetName)(void*);
	if (this == NULL)
		return "INVALID";

	std::string meme = Util::GetVFunc<tGetName>(this, 378)(this);
	meme.erase(0, 13);

	return meme;
}
