#include "../../csgo-sdk.h"
#include "../MENU.h"

#define charenc(s) s
void RetaliateMenu(IDirect3DDevice9* pDevice);
void GUI_Init(IDirect3DDevice9* pDevice);
static const char* tabNames[] = { "HvH", " Visuals ", " Misc ", " Colours " };
static const int numTabs = sizeof(tabNames) / sizeof(tabNames[0]);
static const char* tabTooltips[numTabs] = { "", "", "", "" };
static int tabItemOrdering[numTabs] = { 0, 1, 2, 3 };
static int selectedTab = 0;
static int optionalHoveredTab = 0;
static ImFont* defaultFont;
static ImFont* arialFont;
static ImFont* titleFont;

PresentFn oPresent;
HRESULT __stdcall Hooks::Present(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) {
	DO_ONCE{
		Drawmanager->Initialize(pDevice);
		//GUI_Init(pDevice);
	}


	Drawmanager->BeginRendering();
	//Draw Shit
	//Drawmanager->AddFont("Tahoma", 18, true, false);


	//Drawmanager->DrawText(Color::Purple(), "Tahoma", 100, 80, FNT_OUTLINED, "csgo-sdk");
	//Drawmanager->FillGradientRect(0, 0, 300, 200, Color::Yellow(), Color::Red(), Color::Blue(), Color::Green());

	Drawmanager->EndRendering();

	//Visuals->Tick();

	//RetaliateMenu(pDevice);
	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
