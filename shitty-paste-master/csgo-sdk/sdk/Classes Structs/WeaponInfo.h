#pragma once

class WeaponInfo_t {
public:
	char pad00[0xC8];
	int m_nWeaponType;		// 0xC8
	char padCC[0x20];
	int m_iDamage;			// 0xEC
	float m_flArmorRatio;		// 0xF0
	char padF4[0x4];
	float m_flPenetration;	// 0xF8
	char padFC[0x8];
	float m_flRange;			// 0x104
	float m_flRangeModifier;	// 0x108
	char pad10C[0x10];
	bool m_bHasSilencer;		// 0x11C
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	//	DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED
	//	DATA_UPDATE_LEFT_PVS,
	//DATA_UPDATE_DESTROYED,
};

class C_TEFireBullets
{
public:
	char pad[16];
	int m_iPlayer; //0x10
	int m_nItemDefIndex; //0x14
	Vector m_vecOrigin; //0x18
	Vector m_vecAngles; //0x24
	int m_weapon; //0x30
	int m_iWeaponID; //0x34
	int m_iMode; //0x38
	int m_iSeed; //0x3C
	float m_fInaccuracy; //0x40
	float m_flRecoilIndex; //0x44
	float m_fSpread; //0x48
	int m_iSoundType; //0x4C
};