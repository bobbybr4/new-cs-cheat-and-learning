#include "MenuWindow.h"

#pragma region keyshit
bool keys[256];
bool oldKeys[256];

#pragma endregion
#pragma region input
bool MWindow::ToggleButton(ButtonCode_t code)
{
	static int buttonPressedTick = 0;

	if (Interfaces->InputSystem->IsButtonDown(code) && (GetTickCount64() - buttonPressedTick) > 100)
	{
		buttonPressedTick = GetTickCount64();
		return true;
	}
	return false;
}

bool MWindow::ToggleButton(int code)
{
	static bool PressedButtons[256];

	if (!PressedButtons[code] && GetAsyncKeyState(code))
	{
		PressedButtons[code] = true;
		return false;
	}
	else if (PressedButtons[code] && !GetAsyncKeyState(code))
	{
		PressedButtons[code] = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool MWindow::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}
void MWindow::ClickHandler()
{
	if ((GetAsyncKeyState(VK_LBUTTON)))
	{
		bMouse1pressed = true;
	}
	else if (!(GetAsyncKeyState(VK_LBUTTON)))
	{
		if (bMouse1pressed)
			bMouse1released = true;
		else
		{
			bMouse1released = false;
		}
		bMouse1pressed = false;
	}

	if ((GetAsyncKeyState(VK_RBUTTON)))
	{
		bMouse2pressed = true;
	}
	else if (!(GetAsyncKeyState(VK_RBUTTON)))
	{
		if (bMouse2pressed)
			bMouse2released = true;
		else
		{
			bMouse2released = false;
		}
		bMouse2pressed = false;
	}
}
bool MWindow::Clicked(Vector position, Vector bounds)
{

	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y &&
		bMouse1released);//ToggleButton(VK_LBUTTON));
}

bool MWindow::InBounds(Vector position, Vector bounds)
{

	return (Cur.x > position.x && Cur.y > position.y &&
		Cur.x < position.x + bounds.x && Cur.y < position.y + bounds.y);
}
#pragma endregion
MWindow::MWindow(std::string name, Vector pos, Vector size, bool IsMainWindow)
{
	this->name = name;
	this->position = pos;
	this->size = size;
	this->visible = false;
	this->bDragging = false;
	this->IsMainWindow = IsMainWindow;
	categoryXoffset = 100;// this->size.x / 5.3333333f;
	dragYoffset = 20;// this->size.y / 10;
					 //init move
	this->moved_in_that_frame = true;
}

DWORD ChangeOpacity(DWORD color, float opacity) {
	int alpha = (color >> 24) & 0xff;
	int r = (color >> 16) & 0xff;
	int g = (color >> 8) & 0xff;
	int b = color & 0xff;

	int newAlpha = ceil(alpha * opacity);

	UINT newColor = r << 16;
	newColor += g << 8;
	newColor += b;
	newColor += (newAlpha << 24);


	return (DWORD)newColor;
}

void MWindow::Draw()
{
	static bool risetotop = false;
	static bool risetobottom = false;

	static int Clock = 0;
	if (m_pMenu->alpha == 0 | m_pMenu->alpha == 255)
		if (ToggleButton(VK_DELETE))
		{
			Clock = 0;
			this->visible = !this->visible;
			char buf[32];
			sprintf_s(buf, "cl_mouseenable %i", !this->visible);
			Interfaces->EngineClient->ClientCmd(buf);
		}

	double timeSoFar1 = (double)Clock / 70.f;
	Clock++;

	if (this->visible)
	{
		if (m_pMenu->alpha != 255)
			if (m_pMenu->alpha > 255)
				m_pMenu->alpha = 255;
			else
				m_pMenu->alpha = timeSoFar1 * 255.f;
	}
	else if (m_pMenu->alpha != 0)
	{
		if (m_pMenu->alpha < 0)
			m_pMenu->alpha = 0;
		else
			m_pMenu->alpha = 255 - (timeSoFar1 * 255.f);
	}

	if (!this->visible && m_pMenu->alpha == 0)
		return;

	m_pMenu->color = ChangeOpacity(g_pSettings->ColorMenu->color, (m_pMenu->alpha / 255));

	//Surface->LockCursor();

	ClickHandler();
	GetCursorPos(&Cur);
	ScreenToClient(G::Window, &Cur);


	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	static int iXDif = 0;
	static int iYDif = 0;

	/*Menu dragging*/
	/*Adjusted for themes, just delete the addition with categoryXoFsset*/
	if ((InBounds(Vector(this->position.x, this->position.y, 0), Vector(this->size.x, dragYoffset, 0)) && bMouse1pressed))
		this->bDragging = true;
	if (this->bDragging)
	{
		if (bMouse1pressed)
		{
			if (iXDif == -1 || iYDif == -1)
			{
				iXDif = Cur.x - this->position.x;
				iYDif = Cur.y - this->position.y;
			}
			this->position.x += Cur.x - (iXDif + this->position.x);
			this->position.y += Cur.y - (iYDif + this->position.y);

			this->moved_in_that_frame = true;
		}
		else
			this->bDragging = false;
	}
	else
	{
		iXDif = -1;
		iYDif = -1;
	}


	int alpha = m_pMenu->alpha;

	/*background*/
	g_pRender->FilledBox(this->position.x, this->position.y, this->size.x, this->size.y, D3DCOLOR_ARGB(static_cast<int>(alpha * 0.94117), 14, 14, 14));

	/*dragbar*/
	g_pRender->FilledBox(this->position.x, this->position.y, this->size.x, dragYoffset + 5, D3DCOLOR_ARGB(alpha, 11, 11, 11));

	/*outline*/
	g_pRender->BorderedBox(this->position.x - 1, this->position.y - 1, this->size.x + 2, this->size.y + 2, D3DCOLOR_ARGB(alpha, 5, 5, 5));
	g_pRender->BorderedBox(this->position.x - 2, this->position.y - 2, this->size.x + 4, this->size.y + 4, D3DCOLOR_ARGB(alpha, 15, 15, 15));
	g_pRender->BorderedBox(this->position.x - 3, this->position.y - 3, this->size.x + 6, this->size.y + 6, D3DCOLOR_ARGB(alpha, 15, 15, 15));
	g_pRender->BorderedBox(this->position.x - 4, this->position.y - 4, this->size.x + 8, this->size.y + 8, D3DCOLOR_ARGB(alpha, 5, 5, 5));

	//g_Draw.StringA(g_Draw.font_GUI, true, this->position.x + categoryXoffset - 10 + (this->size.x - categoryXoffset) / 2, this->position.y + 15, 255, 255, 255, m_pMenu->alpha, "%s", this->name.c_str());

	for (int i = 0; i < Children.size(); i++)
		Children[i]->Draw();

	//m_pMenu->SkinChanger->Draw();
}