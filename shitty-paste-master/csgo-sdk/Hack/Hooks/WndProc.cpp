#include "../../csgo-sdk.h"

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oWndProc;
LRESULT __stdcall Hooks::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//OpenMenu();
	//if (Config->Menu.Opened &&  ImGui_ImplDX9_WndProcHandler(hWnd, uMsg, wParam, lParam))
	//	return true;

	if (m_pMenu && m_pMenu->MainWindow && m_pMenu->MainWindow->visible && !(GetAsyncKeyState(VK_END) & 0x8000))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

