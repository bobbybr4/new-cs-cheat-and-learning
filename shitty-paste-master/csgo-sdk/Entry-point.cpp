#include "csgo-sdk.h"
#include "MaterialHelper.h"
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")

std::unique_ptr<CHook> VPanel;
std::unique_ptr<CHook> ClientMode;
std::unique_ptr<CHook> Client;
std::unique_ptr<CHook> ModelRender;
std::unique_ptr<CHook> Surface;
std::unique_ptr<CHook> D3D9;
std::unique_ptr<CHook> MaterialSystem;
std::unique_ptr<CHook> EngineTrace;
std::unique_ptr<CHook> EngineClient;
std::unique_ptr<CHook> Prediction;
std::unique_ptr<CHook> GameEvents;
std::unique_ptr<CHook> StudioRender;
std::unique_ptr<CHook> MDLCache;

void SetupExports() {
	Math::RandomSeed = (RandomSeedFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed");
	Math::RandomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloat");
	Math::RandomFloatExp = (RandomFloatExpFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomFloatExp");
	Math::RandomInt = (RandomIntFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomInt");
	Math::RandomGaussianFloat = (RandomFloatFn)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomGaussianFloat");
}

/*
░░░░░░░░░░░░░░▄▄▀▀██▀▄▄▒
░░░░░░░░░░░░▄▀░▄▀▀░░░▒▒▀▀▄
░░░░░░░░░░░▐░▄▀░░░░░░░░░░░█
░░░░░░░░░░░▌▌▒▒▒▒░░░░░░░░░░█
░░░░░░▄▄▄░▐▒▒▒▒▒▒░░▒▄▄▄▄░▄░░▌
░░░░▄▀░▄░▐▐▒▒▒▒▒░░░▀░░░░▀░▄▀▐
░░░█░▌░░▌░▐▐▀▄▒▒░░░▒▌██▐░░▌▄▐
░░▐░▐░░░▐░▌▐▐░▒░░░░░░▀▀░░░░░▀▌
░░▌░▌░░░░▌▐▄▀░▒▒▒░░░░░░░▄▀▄░░▐
░▐▐░▌░░░░▐▐░▌▒▒▒▒▒▒▒▒▒▒█▄▄▄░░░▌
░▌░░▌▌░░░░▌░▐▒▒▒▒▒▒▄▄▄▄▄▄▄▄▀▄▄▀
░▐░░▌▌░░░░▐░░▌▒▒▒▒▒▄▀█▄▄▄▄▀
░▌▌░▌▌░░░░░▌░▐▒▒▒▒▒▒▀▄▀▀▀▄
▐░░░▐▐░░░░░▐▐░▌▒▒▒▒▒▒▒▀▀░▄▀█
▌▌░░░▌▌░░░░░█▐░▌▒▒▒▒▒▒▒▄▀░▄▐▄▄
▌░░░░▐▐░░░░░░▀░▐▒▒▒▒▒▄▀░░░▀▀▄▀▌
░░░░░░▌░░░░░▄▀█▄█▄▀▀░▀▄░░░░▀░▀▐
░░░░░░▐░░░░░░░▌░░░░░░▐▐░▀▄▀▄▀▄▀
░░░░░░░█░░░░░▐░░░▌░░░█▀▀▄▀▄▀▄▀
░░░░░░░░▀▄░░▄▄▄▀▐▄▄▀▀
░░░░░░░░░░▀▄▄▄▄▄▀*/

void SetupHooks() {
	/*oLowerBodyYawTargetProxy = Netvars->HookProp("DT_BasePlayer", "m_flLowerBodyYawTarget", Hooks::LowerBodyYawTargetProxy);*/

	oWndProc = (WNDPROC)SetWindowLongPtr(G::Window, GWL_WNDPROC, (LONG_PTR)Hooks::WndProc);
	VPanel = std::make_unique<CHook>(Interfaces->VPanel);
	Client = std::make_unique<CHook>(Interfaces->Client);
	ClientMode = std::make_unique<CHook>(Interfaces->ClientMode);
	ModelRender = std::make_unique<CHook>(Interfaces->ModelRender);
	Surface = std::make_unique<CHook>(Interfaces->Surface);
	D3D9 = std::make_unique<CHook>(reinterpret_cast<void*>(offsets->d3d9Device));
	MaterialSystem = std::make_unique<CHook>(Interfaces->MaterialSystem);
	EngineClient = std::make_unique<CHook>(Interfaces->EngineClient);
	EngineTrace = std::make_unique<CHook>(Interfaces->EngineTrace);
	Prediction = std::make_unique<CHook>(Interfaces->Prediction);
	GameEvents = std::make_unique<CHook>(Interfaces->GameEventManager);
	StudioRender = std::make_unique<CHook>(Interfaces->StudioRender);
	MDLCache = std::make_unique<CHook>(Interfaces->MdlCache);

	oEndScene = D3D9->HookMethod<EndSceneFn>(Hooks::EndScene, 42);

	while (!m_pMenu || !m_pMenu->initialized)
		Sleep(200);

	oPaintTraverse = VPanel->HookMethod<PaintTraverseFn>(Hooks::PaintTraverse, 41);
	oCreateMove = ClientMode->HookMethod<CreateMoveFn>(Hooks::CreateMove, 24);
	oOverrideView = ClientMode->HookMethod<OverrideViewFn>(Hooks::OverrideView, 18);
	oFrameStageNotify = Client->HookMethod<FrameStageNotifyFn>(Hooks::FrameStageNotify, 36);
	oRunCommand = Prediction->HookMethod<RunCommandFn>(Hooks::RunCommand, 19);
	oDrawHuso = StudioRender->HookMethod<DrawHuso_t>(Hooks::DrawHuso, 29); // idx @ 29 // meme by eclipse xddddddddddddddd
	oReset = D3D9->HookMethod<ResetFn>(Hooks::Reset, 16);
	oPresent = D3D9->HookMethod<PresentFn>(Hooks::Present, 17);
	oTraceRay = EngineTrace->HookMethod<TraceRayFn>(Hooks::TraceRay, 5);
	oGetLastTimeStamp = EngineClient->HookMethod<GetLastTimeStampFn>(Hooks::GetLastTimeStamp, 14);
	oIsPlayingTimeDemo = EngineClient->HookMethod<IsPlayingTimeDemoFn>(Hooks::IsPlayingTimeDemo, 83);
	oIsConnected = EngineClient->HookMethod<IsConnectedFn>(Hooks::IsConnected, 27);
	oFireEvent = GameEvents->HookMethod<FireEventFn>(Hooks::FireEvent, 9);
	oFindMDL = MDLCache->HookMethod<FindMdlFn>(Hooks::FindMDL, 10); // idx @ 10 ==> MDLCache::FindModel()
}

void Setup() {
	AllocConsole();
	FILE* pOut;
	freopen_s(&pOut, "CONOUT$", "w", stdout);
	freopen_s(&pOut, "CONOUT$", "w", stderr);
	fclose(pOut);

	SetupExports();
	Interfaces;

	Netvars;
	offsets;
	Drawmanager;
	Visuals;
	EnginePrediction;
	Lagcompensation;
	Resolver;
	Backtracking->InitializeLagRecord();

	g_pMaterialHelper = new CMaterialHelper();

	D::SetupFonts();
	SetupHooks();	
}

void Terminate() {
	Util::ToggleThirdPerson(false, true);

	if (oLowerBodyYawTargetProxy)
		Netvars->HookProp("DT_BasePlayer", "m_flLowerBodyYawTarget", oLowerBodyYawTargetProxy);

	VPanel->Unhook();
	Client->Unhook();
	ClientMode->Unhook();
	ModelRender->Unhook();
	Surface->Unhook();
	D3D9->Unhook();
	MaterialSystem->Unhook();
	EngineClient->Unhook();
	EngineTrace->Unhook();
	Prediction->Unhook();
	GameEvents->Unhook();
	StudioRender->Unhook();

	SetWindowLongPtr(G::Window, GWL_WNDPROC, (LONG_PTR)oWndProc);

	FreeConsole();
}

DWORD __stdcall CheatThread() {
	Beep(750, 500);

	while (!(G::Window = FindWindowW(L"Valve001", NULL)))
		Sleep(200);

	while (!GetModuleHandleA("serverbrowser.dll")) 
		Sleep(200);

	Setup();

	//necroSecurity::RemoveHeader(); // breaks game ;w;

	//if (!necroSecurity::ObfuscateThread(NULL))
	//	necroSecurity::OnValidException();

	while (!(GetAsyncKeyState(VK_HOME) & 0x8000)) // security thread
	{
	//	if (!necroSecurity::Think())
	//		necroSecurity::OnDebuggerAttempt(); // fuck that nigga up ayy
	//
		Sleep(150);
	}

	Terminate();
	
	// enough time for hooks to finish
	Sleep(5000);
	
	// exit the thread and free our dll for rebuilds etc
	FreeLibraryAndExitThread(G::Dll, NULL);
}

void SecurityThread()
{
//	if (!necroSecurity::ObfuscateThread(NULL))
	//	necroSecurity::OnValidException();

	while (!(GetAsyncKeyState(VK_HOME) & 0x8000))
	{
//		if (!necroSecurity::Think())
//			necroSecurity::OnDebuggerAttempt(); // fuck that nigga up ayy

		if (!InternetCheckConnectionA("http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0))
			exit(0);

		Sleep(15000);
	}

	ExitThread(NULL);
}

void TimeThread()
{
	std::this_thread::sleep_for(8h);
	exit(0);
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		G::Dll = (HMODULE)hModule;
		//GetModuleFileNameA(G::Dll, G::DllPath, sizeof(G::DllPath));
		
		G::DllPath = getenv("APPDATA");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheatThread, 0, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SecurityThread, 0, 0, 0);
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TimeThread, 0, 0, 0);

		//if (necroSecurity::CheckForBan())
		//	necroSecurity::OnDebuggerAttempt();

		break;
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	case DLL_PROCESS_DETACH:  break;
	}

	return TRUE;
}
