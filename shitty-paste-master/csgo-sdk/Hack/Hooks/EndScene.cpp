#include "../../csgo-sdk.h"

EndSceneFn oEndScene;
HRESULT __stdcall Hooks::EndScene(IDirect3DDevice9* pDevice) {
	DO_ONCE{
		g_pRender->Init(pDevice);
		m_pMenu = new CMenu();
	}
	
	if(g_pRender)
		g_pRender->Reset();

	if (g_pRender && m_pMenu)
	{
		int alpha = m_pMenu->alpha;

		m_pMenu->MainWindow->Draw();

		/*if (g_pSettings->VisualsDamageOutput->Checked)
		{
			g_pSettings->EventLog->Draw();
		}*/

		if (m_pMenu->MainWindow->visible)
		{
			/* draw cursor */
			POINT point = m_pMenu->MainWindow->Cur; // credits: kn4ck3r	

			g_pRender->FilledBox(point.x + 0, point.y + 0, 1, 12, BLACK(alpha));
			g_pRender->FilledBox(point.x + 1, point.y + 0, 1, 11, BLACK(alpha));
			g_pRender->FilledBox(point.x + 1, point.y + 11, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 2, point.y + 1, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 2, point.y + 10, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 3, point.y + 2, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 3, point.y + 9, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 4, point.y + 3, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 5, point.y + 4, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 6, point.y + 5, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 7, point.y + 6, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 8, point.y + 7, 1, 1, BLACK(alpha));
			g_pRender->FilledBox(point.x + 4, point.y + 8, 4, 1, BLACK(alpha));

			g_pRender->FilledBox(point.x + 1, point.y + 1, 1, 10, WHITE(alpha));
			g_pRender->FilledBox(point.x + 2, point.y + 2, 1, 8, WHITE(alpha));
			g_pRender->FilledBox(point.x + 3, point.y + 3, 1, 6, WHITE(alpha));
			g_pRender->FilledBox(point.x + 4, point.y + 4, 1, 4, WHITE(alpha));
			g_pRender->FilledBox(point.x + 5, point.y + 5, 1, 3, WHITE(alpha));
			g_pRender->FilledBox(point.x + 6, point.y + 6, 1, 2, WHITE(alpha));
			g_pRender->FilledBox(point.x + 7, point.y + 7, 1, 1, WHITE(alpha));
		}
	}

	return oEndScene(pDevice);
}
