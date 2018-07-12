#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include "MenuControls.h"

using namespace std;

class MWindow;
class MCheckbox;
class MCategory;
class MColorPicker;
class MDropdown;
class MKeybind;
class MSavePanel;
class MSmallWindow;
class MTab;
class MTextbox;
class MGroupbox;
class MSlider;

class CSettings
{
public:
	CSettings(MWindow* MainWindow);

	/* categories */
	MCategory* RageCategory = nullptr;
	MCategory* AACategory = nullptr;
	MCategory* VisualCategory = nullptr;
	MCategory* MiscCategory = nullptr;
	MCategory* ConfigCategory = nullptr;

	/* small-windows */
	MSmallWindow* EventLog = nullptr;
	MSmallWindow* SkinChanger = nullptr;

	/* rage */
	MGroupbox* RageConfig = nullptr;
	MCheckbox* RagebotEnabled = nullptr; //
	MCheckbox* RagebotFriendly = nullptr; //
	MKeybind*  RagebotKey = nullptr;//
	MCheckbox* RagebotSilent = nullptr;//

	MDropdown* RagebotAutoWall = nullptr;//
	MCheckbox* RagebotAutoFire = nullptr;//
	MCheckbox* RagebotAutoScope = nullptr;//
	MCheckbox* RagebotAutoCrouch = nullptr;//
	MCheckbox* RagebotAutoStop = nullptr;//

	MSlider* RagebotFOV = nullptr;//
	MSlider* RagebotMinimumDamage = nullptr;//
	MSlider* RagebotMinimumHitchance = nullptr;//
	MSlider* RagebotMinimumMultipoint = nullptr;//
	MCheckbox* RagebotMultipoint = nullptr;//

	MDropdown* RagebotRevolver = nullptr;//
	MCheckbox* RagebotAWP = nullptr;//
	MDropdown* RagebotPreferHitbox = nullptr;//
	MDropdown* RagebotSelection = nullptr;//
	MCheckbox* RagebotDelayShot = nullptr;//
	MDropdown* RagebotCorrections = nullptr;//
	MDropdown* RagebotPositionAdjustment = nullptr;//
	MSlider* RagebotHistoryTicks = nullptr;//
	MCheckbox* RagebotCorrectLagComp = nullptr;
	
	MKeybind* RagebotBruteforce = nullptr;//
	/*if (Config->Ragebot.Hitboxes.Pelvis)
		m_hitboxes.push_back(HITBOX_PELVIS);
	if (Config->Ragebot.Hitboxes.Chest)
		m_hitboxes.push_back(HITBOX_CHEST);
	if (Config->Ragebot.Hitboxes.Head)
	{
		m_hitboxes.push_back(HITBOX_HEAD);
		m_hitboxes.push_back(HITBOX_LOWER_NECK);
	}
	if (Config->Ragebot.Hitboxes.Arms) {
		m_hitboxes.push_back(HITBOX_RIGHT_FOREARM);
		m_hitboxes.push_back(HITBOX_LEFT_FOREARM);
		m_hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
		m_hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
		m_hitboxes.push_back(HITBOX_RIGHT_HAND);
		m_hitboxes.push_back(HITBOX_LEFT_HAND);
	}
	if (Config->Ragebot.Hitboxes.Feet) {
		m_hitboxes.push_back(HITBOX_RIGHT_CALF);
		m_hitboxes.push_back(HITBOX_LEFT_CALF);
		m_hitboxes.push_back(HITBOX_LEFT_FOOT);
		m_hitboxes.push_back(HITBOX_RIGHT_FOOT);
	}*/

	MCheckbox* RagebotHead = nullptr;
	MCheckbox* RagebotChest = nullptr;
	MCheckbox* RagebotPelvis = nullptr;
	MCheckbox* RagebotArms = nullptr;
	MCheckbox* RagebotLegs = nullptr;
	MSlider* RagebotDeltaTolerance = nullptr;//
	MCheckbox* RagebotBodyAimLethal = nullptr;//
	MSlider* RagebotBodyAimShots = nullptr;//
	
	MCheckbox* RagebotAntiaim = nullptr;//
	MDropdown* RagebotPitch = nullptr;//
	MDropdown* RagebotYaw = nullptr;//
	MDropdown* RagebotFakeYaw = nullptr;//
	MDropdown* RagebotWalkYaw = nullptr;//
	MDropdown* RagebotEdgeYaw = nullptr;//
	MCheckbox* RagebotDynamic = nullptr;//

	MSlider* RagebotJitterReal = nullptr;//
	MSlider* RagebotJitterFake = nullptr;//
	MSlider* RagebotJitterWalk = nullptr;//
	MKeybind* RagebotAngleLock = nullptr;//

	MDropdown* RagebotFakeWalk = nullptr;//
	MKeybind* RagebotWalkKey = nullptr;//
	MSlider* RagebotFakeWalkCapSpeed = nullptr;//
	MSlider* RagebotRealAngle = nullptr;//
	MSlider* RagebotFakeAngle = nullptr;//
	MSlider* RagebotWalkAngle = nullptr;//


	/* Anti Aim */


	/* legit */
	MCheckbox* LegitEnable = nullptr;
	MKeybind* LegitOnKey = nullptr;
	MCheckbox* LegitFriendly = nullptr;
	MDropdown* LegitPositionAdjustment = nullptr;
	MSlider* LegitHistoryTicks = nullptr;

	/* weapon grouped configuration */
	MDropdown* LegitHitboxRifle = nullptr;
	MSlider* LegitFOVRifle = nullptr;
	MSlider* LegitSmoothRifle = nullptr;
	MSlider* LegitTimeoutRifle = nullptr;
	MCheckbox* LegitCompensateRifle = nullptr;
	MSlider* LegitRecoilRifle = nullptr;

	MDropdown* LegitHitboxPistol = nullptr;
	MSlider* LegitFOVPistol = nullptr;
	MSlider* LegitSmoothPistol = nullptr;
	MSlider* LegitTimeoutPistol = nullptr;
	MCheckbox* LegitCompensatePistol = nullptr;
	MSlider* LegitRecoilPistol = nullptr;

	MDropdown* LegitHitboxSniper = nullptr;
	MSlider* LegitFOVSniper = nullptr;
	MSlider* LegitSmoothSniper = nullptr;
	MSlider* LegitTimeoutSniper = nullptr;
	MCheckbox* LegitCompensateSniper = nullptr;
	MSlider* LegitRecoilSniper = nullptr;

	MDropdown* LegitHitboxOther = nullptr;
	MSlider* LegitFOVOther = nullptr;
	MSlider* LegitSmoothOther = nullptr;
	MSlider* LegitTimeoutOther = nullptr;
	MCheckbox* LegitCompensateOther = nullptr;
	MSlider* LegitRecoilOther = nullptr;

	/* triggerbot (in legit but still) */
	MCheckbox* TriggerbotEnable = nullptr;
	MKeybind* TriggerbotOnKey = nullptr;
	MDropdown* TriggerbotFilter = nullptr;
	MSlider* TriggerbotDelay = nullptr;
	MSlider* TriggerbotCooldown = nullptr;
	MSlider* TriggerbotRecoil = nullptr;
	MSlider* TriggerbotAfterburst = nullptr;

	/* visual */
	MCheckbox* VisualsEnabled = nullptr;
	MCheckbox* VisualsSelf = nullptr;
	MCheckbox* VisualsFriends = nullptr;
	MCheckbox* VisualsEnemies = nullptr;
	MCheckbox* VisualsC4 = nullptr;
	MCheckbox* VisualsDropped = nullptr;
	MCheckbox* VisualsLegit = nullptr;

	MDropdown* VisualsBox = nullptr;
	MDropdown* VisualsHitbox = nullptr;
	MDropdown* VisualsSkeleton = nullptr;
	MCheckbox* VisualsGlow = nullptr;
	MDropdown* VisualsChams = nullptr;
	MCheckbox* VisualsName = nullptr;
	MDropdown* VisualsHealth = nullptr;
	MCheckbox* VisualsFlags = nullptr;
	MCheckbox* VisualsWeapon = nullptr;

	MCheckbox* VisualsRemovalSmoke = nullptr;
	MCheckbox* VisualsRemovalFlash = nullptr;
	MCheckbox* VisualsRemovalScope = nullptr;
	MCheckbox* VisualsRemovalParticles = nullptr;
	MCheckbox* VisualsRemovalRecoil = nullptr;
	MCheckbox* VisualsAsusMode = nullptr;
	MSlider* VisualsAsusBlend = nullptr;

	MSlider* VisualsOverrideFOV = nullptr;
	MCheckbox* VisualsGrenadeTrajectory = nullptr;
	MCheckbox* VisualsBrightnessAdjust = nullptr;
	MDropdown* VisualsReticle = nullptr;
	MCheckbox* VisualsReticlePenetration = nullptr;
	MCheckbox* VisualsOverlayInaccuracy = nullptr;
	
	MDropdown* VisualsArmChams = nullptr;
	MDropdown* VisualsWeaponChams = nullptr;
	
	MCheckbox* VisualsArmWireframe = nullptr;
	MCheckbox* VisualsWeaponWireframe = nullptr;

	MCheckbox* VisualsSpectatorAlert = nullptr;
	MCheckbox* VisualsDamageOutput = nullptr;
	MCheckbox* VisualsHitMarker = nullptr;
	MCheckbox* VisualsHitMarkerSound = nullptr;
	MCheckbox* VisualsThirdPerson = nullptr;
	MDropdown* VisualsThirdPersonGhost = nullptr;

	/* misc */
	// colors first because left over from visuals tab
	MColorPicker* ColorEnemyInvisibleChams = nullptr;
	MColorPicker* ColorEnemyVisibleChams = nullptr;
	MColorPicker* ColorTeamInvisibleChams = nullptr;
	MColorPicker* ColorTeamVisibleChams = nullptr;
	MColorPicker* ColorEnemyInvisible = nullptr;
	MColorPicker* ColorEnemyVisible = nullptr;
	MColorPicker* ColorTeamInvisible = nullptr;
	MColorPicker* ColorTeamVisible = nullptr;
	MColorPicker* ColorHands = nullptr;
	MColorPicker* ColorWeapon = nullptr;
	MColorPicker* ColorEntity = nullptr;
	MColorPicker* ColorMenu = nullptr;

	MDropdown* MiscRestrictions = nullptr;
	MCheckbox* MiscBunnyhop = nullptr;
	MDropdown* MiscAutostrafe = nullptr;

	MKeybind* MiscPrespeed = nullptr;
	MKeybind* MiscEdgeJump = nullptr;
	MCheckbox* MiscKnifeTrigger = nullptr;
	MKeybind* MiscAirstuck = nullptr;
	MCheckbox* MiscRankReveal = nullptr;
	MCheckbox* MiscCustomModel = nullptr;

	MDropdown* MiscFakelag = nullptr;
	MSlider* MiscFakelagFactor = nullptr;
	MSlider* MiscFakelagJitter = nullptr;

	//MSlider* MiscOverrideFOV = nullptr;

	MCheckbox* MiscNamespam = nullptr;
	MDropdown* MiscChatspam = nullptr;
	MDropdown* MiscClantag = nullptr;
	MTextbox* MiscClantagText = nullptr;

	/* presets */
	MGroupbox* Presets = nullptr;
	MSavePanel* PresetSavePanel = nullptr;

}; extern CSettings* g_pSettings;

class CMenu
{
public:
	CMenu();
	//void InitConfig(HMODULE hModule);	
	//void InitSkins();
	void SaveWindowState(std::string Filename);
	void LoadWindowState(std::string Filename);
	//void ReadOrWriteConfig(const char* szPath, bool bRead);
	float alpha = 0;
	DWORD color = D3DCOLOR_ARGB(255, 230, 184, 0);
	MWindow* MainWindow = nullptr;
	bool initialized = false;

}; extern CMenu* m_pMenu;