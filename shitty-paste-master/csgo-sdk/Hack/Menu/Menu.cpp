#include "..\..\Common.h"
#include "Menu.h"
#include "tinyxml.h"

CMenu* m_pMenu = nullptr;
CSettings* g_pSettings = nullptr;

BYTE* meme = NULL;

CSettings::CSettings(MWindow* MainWindow)
{


	//--------------------------------------------------------------//
	//----------------------------SETUP-----------------------------//
	//--------------------------------------------------------------//


	RageCategory = new MCategory("P", MainWindow, meme, sizeof(meme), 269, 421);

	MCategory::CurrentCategory = RageCategory;
	MCategory::CurrentCategory->active = true;

	AACategory = new MCategory("Q", MainWindow, meme, sizeof(meme), 69, 420);
	VisualCategory = new MCategory("R", MainWindow, meme, sizeof(meme), 1337, 444444444);
	MiscCategory = new MCategory("S", MainWindow, meme, sizeof(meme), 111, 666);
	ConfigCategory = new MCategory("T", MainWindow, meme, sizeof(meme), 888, 9999999999);



	//--------------------------------------------------------------//
	//-------------------------RAGEBOT------------------------------//
	//--------------------------------------------------------------//

	//-------------------------AIMBOT-------------------------------//

	RageConfig = new MGroupbox(RageCategory, "Aimbot", 1, 1, 1, 1, false);
	RagebotEnabled = new MCheckbox(RageConfig, "Enable", "Enables the Aimbot.", RageCategory, "necro_rage_Enable");
	RagebotKey = new MKeybind(RageConfig, "Key", "The Key under which the Aimbot will activate.", RageCategory, "necro_rage_Key");
	RagebotFriendly = new MCheckbox(RageConfig, "Friendly Fire", "Enables Aimbot on Team.", RageCategory, "necro_rage_friend");
	RagebotSilent = new MCheckbox(RageConfig, "Silent Aim", "Makes the Aimbot flicks invisible (visible to spectators).", RageCategory, "necro_rage_silent");
	RagebotAutoWall = new MDropdown(RageConfig, "Auto Wall", "Automatically fires", RageCategory, "necro_rage_Autowall", std::vector<std::string>{"Conservative", "Intense"});
	RagebotAutoFire = new MCheckbox(RageConfig, "Auto Shoot", "Automatically fires", RageCategory, "necro_rage_shoot");
	/*RagebotAutoScope = new MCheckbox(RageConfig, "auto-zoom", "Automatically scopes", RageCategory, "necro_rage_scope");
	RagebotAutoStop = new MCheckbox(RageConfig, "auto-stop", "Force stops", RageCategory, "necro_rage_stop");
	RagebotAutoCrouch = new MCheckbox(RageConfig, "auto-crouch", "Automatically crouches", RageCategory, "necro_rage_crouch");*/
	RagebotRevolver = new MDropdown(RageConfig, "Auto Revolver", "Different Auto revolver methods", RageCategory, "necro_rage_revolver", std::vector<std::string>{("None"), ("Spread"), ("No-Spread")});
	RagebotAWP = new MCheckbox(RageConfig, "Awp Bodyaim", "", RageCategory, "necro_rage_awpbaim");
	//RagebotHitscan = new MDropdown(RageConfig, "hitscan", "", RageCategory, "necro_rage_hitscan", std::vector<std::string>{"off", "bare", "hesitant", "absolute"});

	RageConfig = new MGroupbox(RageCategory, "Filter", 1, 2, 1, 1, false);
	RagebotHead = new MCheckbox(RageConfig, "Head", "", RageCategory, "necro_rage_head");
	RagebotChest = new MCheckbox(RageConfig, "Chest", "", RageCategory, "necro_rage_chst");;
	RagebotPelvis = new MCheckbox(RageConfig, "Pelvis", "", RageCategory, "necro_rage_plvs");;
	RagebotArms = new MCheckbox(RageConfig, "Arms", "", RageCategory, "necro_rage_arms");;
	RagebotLegs = new MCheckbox(RageConfig, "Legs", "", RageCategory, "necro_rage_legs");;

	RagebotDeltaTolerance = new MSlider(RageConfig, "Maximum Hitscan Delta", "", 0, 25, true, RageCategory, "necro_rage_delta");
	RagebotBodyAimLethal = new MCheckbox(RageConfig, "Bodyaim if Lethal", "", RageCategory, "necro_rage_lethal");
	RagebotBodyAimShots = new MSlider(RageConfig, "Bodyaim After X Shots", "", 0, 15, true, RageCategory, "necro_rage_shots");

	RageConfig = new MGroupbox(RageCategory, "Variables", 2, 1, 1, 1, false); // shh perfect positioning system with no memory leaks
	RagebotMinimumDamage = new MSlider(RageConfig, "Min Damage", "Changes damage from 0 - 180", 0, 100, true, RageCategory, "necro_rage_damage");
	RagebotMinimumHitchance = new MSlider(RageConfig, "Hit Change", "Changes damage from 0 - 180", 0, 100, true, RageCategory, "necro_rage_hitchance");
	RagebotDelayShot = new MCheckbox(RageConfig, "Delay Shot", "", RageCategory, "necro_rage_delay");
	RagebotCorrections = new MDropdown(RageConfig, "Resolver", "", RageCategory, "necro_rage_correct", std::vector<std::string>{"Disabled", "Basic", "History"});
	RagebotBruteforce = new MKeybind(RageConfig, "Angle Override", "", RageCategory, "necro_rage_brute");
	RagebotPositionAdjustment = new MDropdown(RageConfig, "Position Adjustment", "", RageCategory, "necro_rage_posadj", std::vector<std::string>{"Disabled", "Basic", "History"});
	//RagebotHistoryTicks = new MSlider(RageConfig, "backtrack limit (ms)", "", 0, 200, true, RageCategory, "necro_rage_histicks");

	RageConfig = new MGroupbox(RageCategory, "Target", 2, 2, 1, 1, false);
	RagebotFOV = new MSlider(RageConfig, "FOV", "Changes FOV from 0 - 180", 0, 180, true, RageCategory, "necro_rage_fov");
	RagebotPreferHitbox = new MDropdown(RageConfig, "Hitbox", "Hitbox preference", RageCategory, "necro_rage_hitbox", std::vector<std::string>{	"Pelvis",
		"Upper Pelvis",
		"Neck",
		"Lower Pelvis",
		"Stomach",
		"Upper-Stomach",
		"Chest",
		"Lower-Neck",
		"Head"});
	RagebotSelection = new MDropdown(RageConfig, "Selection", "Scan preference", RageCategory, "necro_rage_select", std::vector<std::string>{("Closest to Crosshair"), ("Proximity"), ("Lowest HP"), ("Next Shot")});
	RagebotMultipoint = new MCheckbox(RageConfig, "Multi Point", "", RageCategory, "necro_rage_multipoint");
	RagebotMinimumMultipoint = new MSlider(RageConfig, "Point Scale", "Changes damage from 0 - 180", 0, 100, true, RageCategory, "necro_rage_scale");
	
	RageConfig = new MGroupbox(RageCategory, "Miscellaneous", 3, 1, 1, 2, false); // should probably add FakeLag here, aswell sa moving Autostop etc.
	RagebotAutoScope = new MCheckbox(RageConfig, "Auto Scope", "Automatically scopes", RageCategory, "necro_rage_scope");
	RagebotAutoStop = new MCheckbox(RageConfig, "Auto Stop", "Force stops", RageCategory, "necro_rage_stop");
	RagebotAutoCrouch = new MCheckbox(RageConfig, "Auto Crouch", "Automatically crouches", RageCategory, "necro_rage_crouch");
	RagebotAngleLock = new MKeybind(RageConfig, "Angle Flip Key", "", RageCategory, "necro_rage_anglock");
	//RagebotFakeWalk = new MDropdown(RageConfig, "fake-walk", "", RageCategory, "necro_rage_fw", std::vector<std::string>{"regular", "slow-motion"});//
	RagebotWalkKey = new MKeybind(RageConfig, "Fake Walk Key", "", RageCategory, "necro_rage_fwKey");//
	// RagebotFakeWalkCapSpeed = new MSlider(RageConfig, "maximum fake-walk velocity", "", 0, 75, true, RageCategory, "necro_rage_fw_maxspeed"); //

	RageConfig = new MGroupbox(AACategory, "Presets", 1, 1, 1, 2, false);
	RagebotAntiaim = new MCheckbox(RageConfig, "Enable", "", AACategory, "necro_rage_antiaim");
	RagebotPitch = new MDropdown(RageConfig, "Pitch", "", AACategory, "necro_rage_x", std::vector<std::string>{"Disabled", "Dynamic", "Emotion", "Fake Down", "Fake Zero"});
	RagebotYaw = new MDropdown(RageConfig, "Yaw", "", AACategory, "necro_rage_y", std::vector<std::string>{"Disabled", "180", "180 Jitter", "180 Smooth jitter", "Z-Jitter", "Directional", "Spin", "Custom", "LBY", "LBY Flip", "LBY Jitter", "LBY test"});
	RagebotFakeYaw = new MDropdown(RageConfig, "FakeYaw", "", AACategory, "necro_rage_fy", std::vector<std::string>{"Disabled", "180", "180 Jitter", "Directional", "Custom", "LBY", "LBY Flip", "LBY Jitter", "LBY test"});
	RagebotWalkYaw = new MDropdown(RageConfig, "MoveYaw", "", AACategory, "necro_rage_wy", std::vector<std::string>{"Disabled", "180", "180 Jitter", "180 Smooth jitter", "Z-Jitter", "Directional", "Spin", "Custom", "LBY", "LBY Flip", "LBY Jitter", "LBY test"});
	RagebotEdgeYaw = new MDropdown(RageConfig, "Edge", "", AACategory, "necro_rage_ey", std::vector<std::string>{"Disabled", "Static", "Independent"});
	RagebotDynamic = new MCheckbox(RageConfig, "At Targets", "", AACategory, "necro_rage_dyn");

	RageConfig = new MGroupbox(AACategory, "Custom Offsets", 2, 1, 1, 1, false);
	RagebotRealAngle = new MSlider(RageConfig, "Custom Real", "", -180, 180, true, AACategory, "necro_rage_ar");
	RagebotFakeAngle = new MSlider(RageConfig, "Custom Fake", "", -180, 180, true, AACategory, "necro_rage_ax");	
	RagebotWalkAngle = new MSlider(RageConfig, "Custom Move", "", -180, 180, true, AACategory, "necro_rage_az");


	RageConfig = new MGroupbox(AACategory, "Custom Ranges", 2, 2, 1, 1, false);
	RagebotJitterReal = new MSlider(RageConfig, "Jitter (Real)", "", 0, 180, true, AACategory, "necro_rage_jr");
	RagebotJitterFake = new MSlider(RageConfig, "Jitter (Fake)", "", 0, 180, true, AACategory, "necro_rage_jj");
	RagebotJitterWalk = new MSlider(RageConfig, "Jitter (Moving)", "", 0, 180, true, AACategory, "necro_rage_jf");

	RageConfig = new MGroupbox(AACategory, "Miscellaneous", 3, 1, 1, 2, false);
	MiscFakelag = new MDropdown(RageConfig, "Fake Lag", "", AACategory, "necro_fLag_Mode", std::vector<std::string>{"Disabled", "Static", "Adaptive", "Dynamic", "Randomized"});
	MiscFakelagFactor = new MSlider(RageConfig, "Max Choke Factor", "", 0, 13, true, AACategory, "necro_fLag_fac");
	MiscFakelagJitter = new MSlider(RageConfig, "Max Jitter", "", 0, 13, true, AACategory, "necro_fLag_jit");
	//--------------------------------------------------------------//
	//-------------------------LEGITBOT-----------------------------//
	//--------------------------------------------------------------//

	/*
	RageConfig = new MGroupbox(LegitCategory, "aim assistance*", 1, 1, 1, 1, false);
	LegitEnable = new MCheckbox(RageConfig, "enabled", "", LegitCategory, "necro_legit_Enable");
	LegitOnKey = new MKeybind(RageConfig, "hotkey", "", LegitCategory, "necro_legit_Key");
	LegitFriendly = new MCheckbox(RageConfig, "friendly fire", "", LegitCategory, "necro_legit_friend");
	LegitPositionAdjustment = new MDropdown(RageConfig, "position-adjustment", "", LegitCategory, "necro_legit_backtrack", std::vector<std::string>{"disabled", "regular", "history"});
	LegitHistoryTicks = new MSlider(RageConfig, "backtrack limit (ms)", "", 0, 200, true, LegitCategory, "necro_legit_histicks");

	RageConfig = new MGroupbox(LegitCategory, "triggerbot*", 1, 2, 1, 1, false);
	TriggerbotEnable = new MCheckbox(RageConfig, "enable triggerbot", "", LegitCategory, "necro_Trigger_Enable");
	TriggerbotOnKey = new MKeybind(RageConfig, "hotkey", "", LegitCategory, "necro_Trigger_Key");
	TriggerbotFilter = new MDropdown(RageConfig, "filter", "", LegitCategory, "necro_Trigger_Filter", std::vector<std::string>{"head only", "above neck", "upper body", "everything"});
	TriggerbotDelay = new MSlider(RageConfig, "delay", "", 0, 500, true, LegitCategory, "necro_Trigger_delay");
	TriggerbotCooldown = new MSlider(RageConfig, "cooldown", "", 0, 1500, true, LegitCategory, "necro_Trigger_cooldown");
	TriggerbotAfterburst = new MSlider(RageConfig, "after-burst (rounds)", "", 0, 15, true, LegitCategory, "necro_Trigger_abshots");
	TriggerbotRecoil = new MSlider(RageConfig, "recoil compensation (%)", "", 0, 100, true, LegitCategory, "necro_Trigger_recoil");

	RageConfig = new MGroupbox(LegitCategory, "rifles*", 2, 1, 1, 1, false); // hehe fuck weapon config handling xddddddddddddddddddddddd
	LegitHitboxRifle = new MDropdown(RageConfig, "hitbox", "", LegitCategory, "necro_rif_aimspot", std::vector<std::string>{"closest", "head", "neck", "stomach", "pelvis"});
	LegitFOVRifle = new MSlider(RageConfig, "field-of-view", "", 0, 15, false, LegitCategory, "necro_rif_fov");
	LegitSmoothRifle = new MSlider(RageConfig, "smoothing (%)", "", 0, 100, true, LegitCategory, "necro_rif_Smooth");;
	LegitTimeoutRifle = new MSlider(RageConfig, "desired lock amount (ms)", "", 0, 250, true, LegitCategory, "necro_rif_timeout");;
	LegitRecoilRifle = new MSlider(RageConfig, "recoil compensation (%)", "", 0, 100, true, LegitCategory, "necro_rif_recoil");;

	RageConfig = new MGroupbox(LegitCategory, "snipers*", 2, 2, 1, 1, false);
	LegitHitboxSniper = new MDropdown(RageConfig, "hitbox", "", LegitCategory, "necro_snip_aimspot", std::vector<std::string>{"closest", "head", "neck", "stomach", "pelvis"});
	LegitFOVSniper = new MSlider(RageConfig, "field-of-view", "", 0, 15, false, LegitCategory, "necro_snip_fov");
	LegitSmoothSniper = new MSlider(RageConfig, "smoothing (%)", "", 0, 100, true, LegitCategory, "necro_snip_Smooth");;
	LegitTimeoutSniper = new MSlider(RageConfig, "desired lock amount (ms)", "", 0, 250, true, LegitCategory, "necro_snip_timeout");;
	LegitRecoilSniper = new MSlider(RageConfig, "recoil compensation (%)", "", 0, 100, true, LegitCategory, "necro_snip_recoil");;

	RageConfig = new MGroupbox(LegitCategory, "pistols*", 3, 1, 1, 1, false);
	LegitHitboxPistol = new MDropdown(RageConfig, "hitbox", "", LegitCategory, "necro_pist_aimspot", std::vector<std::string>{"closest", "head", "neck", "stomach", "pelvis"});
	LegitFOVPistol = new MSlider(RageConfig, "field-of-view", "", 0, 15, false, LegitCategory, "necro_pist_fov");
	LegitSmoothPistol = new MSlider(RageConfig, "smoothing (%)", "", 0, 100, true, LegitCategory, "necro_pist_Smooth");;
	LegitTimeoutPistol = new MSlider(RageConfig, "desired lock amount (ms)", "", 0, 250, true, LegitCategory, "necro_pist_timeout");;
	LegitRecoilPistol = new MSlider(RageConfig, "recoil compensation (%)", "", 0, 100, true, LegitCategory, "necro_pist_recoil");;

	RageConfig = new MGroupbox(LegitCategory, "other*", 3, 2, 1, 1, false);
	LegitHitboxOther = new MDropdown(RageConfig, "hitbox", "", LegitCategory, "necro_pist_aimspot", std::vector<std::string>{"closest", "head", "neck", "stomach", "pelvis"});
	LegitFOVOther = new MSlider(RageConfig, "field-of-view", "", 0, 15, false, LegitCategory, "necro_pist_fov");
	LegitSmoothOther = new MSlider(RageConfig, "smoothing (%)", "", 0, 100, true, LegitCategory, "necro_pist_Smooth");;
	LegitTimeoutOther = new MSlider(RageConfig, "desired lock amount (ms)", "", 0, 250, true, LegitCategory, "necro_pist_timeout");;
	LegitRecoilOther = new MSlider(RageConfig, "recoil compensation (%)", "", 0, 100, true, LegitCategory, "necro_pist_recoil");;
	*/

	//--------------------------------------------------------------//
	//--------------------------VISUALS-----------------------------//
	//--------------------------------------------------------------//


	RageConfig = new MGroupbox(VisualCategory, "Filter", 1, 1, 1, 1, false);
	VisualsEnabled = new MCheckbox(RageConfig, "Enable", "", VisualCategory, "necro_vis_Enable");
	VisualsEnemies = new MCheckbox(RageConfig, "Draw Enemies", "", VisualCategory, "necro_vis_Enemy");
	VisualsFriends = new MCheckbox(RageConfig, "Draw Teammates", "", VisualCategory, "necro_vis_friend");
	VisualsC4 = new MCheckbox(RageConfig, "C4", "", VisualCategory, "necro_vis_c4");
	VisualsDropped = new MCheckbox(RageConfig, "Items", "", VisualCategory, "necro_vis_drop");
	//VisualsSelf = new MCheckbox(RageConfig, "draw self", "", VisualCategory, "necro_vis_self");
	VisualsLegit = new MCheckbox(RageConfig, "Legit Mode", "", VisualCategory, "necro_vis_legit");

	RageConfig = new MGroupbox(VisualCategory, "Extrasensory Perception", 1, 2, 1, 1, false);
	VisualsBox = new MDropdown(RageConfig, "Box", "", VisualCategory, "necro_vis_box", std::vector<std::string>{"Disabled", "Frame", "Full", "3D"});
	VisualsSkeleton = new MDropdown(RageConfig, "Skeleton", "", VisualCategory, "necro_vis_box", std::vector<std::string>{"Disabled", "Regular"/*, "history"*/});
	VisualsGlow = new MCheckbox(RageConfig, "Glow", "", VisualCategory, "necro_vis_glow");
	VisualsName = new MCheckbox(RageConfig, "Name", "", VisualCategory, "necro_vis_name");
	VisualsHealth = new MDropdown(RageConfig, "Health", "", VisualCategory, "necro_vis_health", std::vector<std::string>{"Disabled", "Bar", "Text"});
	VisualsFlags = new MCheckbox(RageConfig, "Armor", "", VisualCategory, "necro_vis_fLags");
	VisualsWeapon = new MCheckbox(RageConfig, "Weapon", "", VisualCategory, "necro_vis_weapon");

	RageConfig = new MGroupbox(VisualCategory, "Removals", 2, 1, 1, 1, false);
	VisualsRemovalSmoke = new MCheckbox(RageConfig, "Smoke", "", VisualCategory, "necro_rmv_smoke");//
	VisualsRemovalFlash = new MCheckbox(RageConfig, "Flash", "", VisualCategory, "necro_rmv_flash");//
	VisualsRemovalScope = new MCheckbox(RageConfig, "Scope", "", VisualCategory, "necro_rmv_scope");//
	VisualsRemovalParticles = new MCheckbox(RageConfig, "Particles", "", VisualCategory, "necro_rmv_postproc");//
	VisualsRemovalRecoil = new MCheckbox(RageConfig, "Visual Recoil", "", VisualCategory, "necro_rmv_recoil");//
	VisualsAsusMode = new MCheckbox(RageConfig, "ASUS Sky", "", VisualCategory, "necro_rmv_asusMode");//
	VisualsAsusBlend = new MSlider(RageConfig, "ASUS Blend", "", 0, 100, true, VisualCategory, "necro_rmv_asusblend");

	RageConfig = new MGroupbox(RageConfig, "Chams", 2, 2, 1, 1, false);
	VisualsChams = new MDropdown(RageConfig, "Player", "", VisualCategory, "necro_vis_Cham", std::vector<std::string>{"Disabled", "Flat"});
	VisualsArmChams = new MDropdown(RageConfig, "Arms", "", VisualCategory, "necro_vis_aCham", std::vector<std::string>{"Disabled", "Textured", "Material", "Flat", "Golden", "Glass"});
	VisualsArmWireframe = new MCheckbox(RageConfig, "Arm Wireframe", "", VisualCategory, "necro_vis_awire");
	VisualsWeaponChams = new MDropdown(RageConfig, "Weapon", "", VisualCategory, "necro_vis_wCham", std::vector<std::string>{"Disabled", "Textured", "Material", "Flat", "Golden", "Glass"});
	VisualsWeaponWireframe = new MCheckbox(RageConfig, "Weapon Wireframe", "", VisualCategory, "necro_vis_wwire");

	RageConfig = new MGroupbox(VisualCategory, "Discellaneous", 3, 1, 1, 2, false);
	VisualsOverrideFOV = new MSlider(RageConfig, "View FOV", "", 0, 45, true, VisualCategory, "necro_vis_fov");
	VisualsGrenadeTrajectory = new MCheckbox(RageConfig, "Nade Prediction", "", VisualCategory, "necro_vis_nadepred");
	VisualsBrightnessAdjust = new MCheckbox(RageConfig, "Night Mode", "", VisualCategory, "necro_vis_walls");
	VisualsReticle = new MDropdown(RageConfig, "Crosshair", "", VisualCategory, "necro_vis_reticle", std::vector<std::string>{"None", "Static", "Recoil"});
	//VisualsReticlePenetration = new MCheckbox(RageConfig, "show penetration", "", VisualCategory, "necro_vis_penetration");
	VisualsOverlayInaccuracy = new MCheckbox(RageConfig, "Draw Spread", "", VisualCategory, "necro_vis_inacc");
	VisualsSpectatorAlert = new MCheckbox(RageConfig, "Spectators*", "", VisualCategory, "necro_vis_specalert");
	//VisualsDamageOutput = new MCheckbox(RageConfig, "damage output*", "", VisualCategory, "necro_vis_dmglog");
	VisualsHitMarker = new MCheckbox(RageConfig, "Hitmarker", "", VisualCategory, "necro_vis_hitmarker");
	VisualsHitMarkerSound = new MCheckbox(RageConfig, "Hitsound", "", VisualCategory, "necro_vis_hitsound");
	VisualsThirdPerson = new MCheckbox(RageConfig, "Third Person", "", VisualCategory, "necro_vis_thirdperson");
	VisualsThirdPersonGhost = new MDropdown(RageConfig, "Third Person Ghost", "", VisualCategory, "necro_vis_tpghost", std::vector<std::string>{"Disabled", "Lines", "Skeleton (broken)"});


	//--------------------------------------------------------------//
	//-----------------------------MISC-----------------------------//
	//--------------------------------------------------------------//



	RageConfig = new MGroupbox(MiscCategory, "Chams (Colours)", 2, 1, 1, 1, false);
	ColorEnemyVisibleChams = new MColorPicker(RageConfig, "Enemy", "", VisualCategory, "necro_vis_colenvx");
	ColorEnemyInvisibleChams = new MColorPicker(RageConfig, "Enemy (Behind Walls)", "", VisualCategory, "necro_vis_colenivx");
	ColorTeamVisibleChams = new MColorPicker(RageConfig, "Team", "", VisualCategory, "necro_vis_coltevx");
	ColorTeamInvisibleChams = new MColorPicker(RageConfig, "Team (Behind Walls)", "", VisualCategory, "necro_vis_colteivx");
	ColorHands = new MColorPicker(RageConfig, "Arms", "", VisualCategory, "necro_vis_colArms");
	ColorWeapon = new MColorPicker(RageConfig, "Weapon", "", VisualCategory, "necro_vis_colweap");

	RageConfig = new MGroupbox(MiscCategory, "Draw (Colours)", 2, 2, 1, 1, false);
	ColorEnemyVisible = new MColorPicker(RageConfig, "Enemy", "", VisualCategory, "necro_vis_colenv");
	ColorEnemyInvisible = new MColorPicker(RageConfig, "Enemy (Behind Walls)", "", VisualCategory, "necro_vis_coleniv");
	ColorTeamVisible = new MColorPicker(RageConfig, "Team", "", VisualCategory, "necro_vis_coltev");
	ColorTeamInvisible = new MColorPicker(RageConfig, "Team (Behind Walls)", "", VisualCategory, "necro_vis_colteiv");
	ColorEntity = new MColorPicker(RageConfig, "Dropped Items", "", VisualCategory, "necro_vis_colent");
	ColorMenu = new MColorPicker(RageConfig, "Interface", "", VisualCategory, "necro_vis_menucol");

	RageConfig = new MGroupbox(MiscCategory, "Miscellaneous", 1, 1, 1, 1, false);//
	MiscRestrictions = new MDropdown(RageConfig, "Restrictions", "", MiscCategory, "necro_Misc_restr", std::vector<std::string>{"disabled", "Anti Untrusted", "Aim-Step"});//
	MiscBunnyhop = new MCheckbox(RageConfig, "Auto Hop", "", MiscCategory, "necro_Misc_bhop");;//
	MiscAutostrafe = new MDropdown(RageConfig, "Auto Strafe", "", MiscCategory, "necro_Misc_strafe", std::vector<std::string>{"Disabled", "Smooth", "Perfect"});;//
	MiscPrespeed = new MKeybind(RageConfig, "Circle Strafe Key", "", MiscCategory, "necro_prespd_Key");//
	//MiscEdgeJump = new MKeybind(RageConfig, "edge-jump*", "", MiscCategory, "necro_edgjmp_Key");
	MiscKnifeTrigger = new MCheckbox(RageConfig, "knife trigger", "", MiscCategory, "necro_Misc_knifebot");;//
	MiscAirstuck = new MKeybind(RageConfig, "Air Stuck Key", "", MiscCategory, "necro_airstuck_Key");//
	MiscRankReveal = new MCheckbox(RageConfig, "Show Rank", "", MiscCategory, "necro_Misc_rankrev");;

	MiscNamespam = new MCheckbox(RageConfig, "Name Spam", "", MiscCategory, "necro_Misc_name");
	MiscChatspam = new MDropdown(RageConfig, "Chat Spam", "", MiscCategory, "necro_Misc_chat", std::vector<std::string>{"Disabled", "Advertisement", "Necrophiliac"});
	MiscClantag = new MDropdown(RageConfig, "Clan Tag Changer", "", MiscCategory, "necro_Misc_clan", std::vector<std::string>{"Disabled", "Static", "Animated", "Hide Name"});
	MiscClantagText = new MTextbox(RageConfig, "[enter text]", "", MiscCategory, "necro_Misc_clantext");

	//--------------------------------------------------------------//
	//--------------------------PRESETS-----------------------------//
	//--------------------------------------------------------------//


	PresetSavePanel = new MSavePanel(ConfigCategory);
}

CMenu::CMenu()
{
	MainWindow = new MWindow(/* "tranquility cs:go", */ "NECROPHILI.AC", Vector(100, 100, 0), Vector(730, 450, 0), true);

	// fuck nullptrs
	Sleep(200);

	// initializes our settings system and menu, how dope is that?
	g_pSettings = new CSettings(MainWindow);

	g_pSettings->ColorMenu->color = D3DCOLOR_ARGB(255, 230, 184, 0);

	g_pSettings->EventLog = new MSmallWindow("", Vector(20, 20, 0), Vector(250, 400, 0), MainWindow, true);

	//memset(&g_pSettings, 0, sizeof(g_pSettings));
	this->initialized = true;
}

void CMenu::SaveWindowState(std::string Filename)
{
	tinyxml2::XMLDocument Doc;

	tinyxml2::XMLElement *Root = Doc.NewElement(XorStr("necro_root"));
	Doc.LinkEndChild(Root);

	// If the window has some tabs..
	if (Root && MainWindow->Children.size() > 0)
	{
		for (auto Cat : MainWindow->Children)
		{
			MCategory* Category = (MCategory*)Cat;
			// Add a new section for this tab
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Category->name.c_str());
			Root->LinkEndChild(TabElement);


			// Now we itterate the controls this tab contains
			if (TabElement && Category->SaveChildren.size() > 0)
			{
				for (auto Control : Category->SaveChildren)
				{
					// If the control is ok to be saved
					if (Control &&  Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						//Utilities::Log(XorStr("Saving control %s"), Control->FileIdentifier.c_str());

						if (!ControlElement)
						{
							//Utilities::Log(XorStr("Errorino :(XorStr("));
							return;
						}

						MCheckbox* cbx = nullptr;
						MDropdown* cbo = nullptr;
						MKeybind* key = nullptr;
						MSlider* sld = nullptr;
						MColorPicker* cpr = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (MCheckbox*)Control;
							ControlElement->SetText(cbx->Checked);
							break;
						case UIControlTypes::UIC_DropDown:
							cbo = (MDropdown*)Control;
							ControlElement->SetText(cbo->SelectedIndex);
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (MKeybind*)Control;
							ControlElement->SetText(key->Key);
							break;
						case UIControlTypes::UIC_Slider:
							sld = (MSlider*)Control;
							ControlElement->SetText(sld->value);
							break;
						case UIControlTypes::UIC_ColorPicker:
							cpr = (MColorPicker*)Control;

							char buf[256];
							sprintf_s(buf, "%d", cpr->color);

							std::string buffer = buf;

							ControlElement->SetText(buffer.c_str());
							break;
						}
					}
				}
			}
		}
	}
	Filename.append(XorStr(".nec"));
	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
	}
}

void CMenu::LoadWindowState(std::string Filename)
{
	// Lets load our meme
	Filename.append(XorStr(".nec"));
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		// The root "ayy" element
		if (Root)
		{
			// If the window has some tabs..
			if (Root && MainWindow->Children.size() > 0)
			{
				for (auto Cat : MainWindow->Children)
				{
					MCategory* Category = (MCategory*)Cat;

					// We find the corresponding element for this tab
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Category->name.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Category->SaveChildren.size() > 0)
						{
							for (auto Control : Category->SaveChildren)
							{
								// If the control is ok to be saved
								if (Control && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										//Utilities::Log(XorStr("We tryin to load fam"));
										MCheckbox* cbx = nullptr;
										MDropdown* cbo = nullptr;
										MKeybind* key = nullptr;
										MSlider* sld = nullptr;
										MColorPicker* cpr = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (MCheckbox*)Control;
											cbx->Checked = (ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_DropDown:
											cbo = (MDropdown*)Control;
											//cbo->SelectIndex(atoi(ControlElement->GetText()));
											cbo->SelectedIndex = (atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (MKeybind*)Control;
											key->Key = (atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (MSlider*)Control;
											sld->value = (atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ColorPicker:
											DWORD doubleWord;

											cpr = (MColorPicker*)Control;

											std::string buffer = ControlElement->GetText();

											cpr->color = atoll(buffer.c_str());
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}