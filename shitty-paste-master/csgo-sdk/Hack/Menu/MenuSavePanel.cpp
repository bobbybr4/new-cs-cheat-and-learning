#include "MenuSavePanel.h"

char* KeyStringsSave[254] = { "Not Bound", "left mouse", "right mouse", "control+break", "middle mouse", "mouse 4", "mouse 5",
nullptr, "backspace", "tab", nullptr, nullptr, nullptr, "enter", nullptr, nullptr, "shift", "ctrl", "alt", "pause",
"caps lock", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "esc", nullptr, nullptr, nullptr, nullptr, "spacebar",
"page up", "page down", "end", "home", "left", "up", "right", "down", nullptr, "print", nullptr, "print screen", "insert",
"delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", "left windows", "right windows", nullptr, nullptr, nullptr, "num 0", "num 1", "num 2", "num 3", "num 4", "num 5", "num 6",
"num 7", "num 8", "num 9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
"f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "num lock", "scroll lock", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "left shift", "right shift", "left control", "right control", "left menu", "right menu", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "next track", "previous track", "stop", "play / pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

void MSavePanel::HandleKeys(int key)
{
	switch (key)
	{
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_LCONTROL:
	case VK_RCONTROL:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_CAPITAL:
	case VK_LBUTTON:
	case VK_RBUTTON:
		return;


	case VK_RETURN:
	case VK_ESCAPE:
		textboxactive = false;
		return;

	case VK_SPACE:
		command.append(" ");
		return;

	case VK_BACK:
		if (command.length() > 0)
		{
			command.erase(command.length() - 1);
			return;
		}


	default:
		char* KeyName = "0";
		KeyName = KeyStringsSave[key];
		//if (GetKeyNameText(key << 16, KeyName, 127))
		if (KeyName != "Backspace" && KeyName != "SHIFT" && KeyName != "CTRL" && KeyName != "ALT")
			command.append(KeyName);

		return;
	}
}

MSavePanel::MSavePanel(MBaseElement* parent)
{
	this->parent = parent;
	this->parent->AddChildControl(this);
}

char* stringToLower(char *string)
{
	int i;
	int len = strlen(string);
	for (i = 0; i<len; i++) {
		if (string[i] >= 'A' && string[i] <= 'Z') {
			string[i] += 32;
		}
	}
	return string;
}

bool bIsDigitInString(std::string pszString)
{
	for (int ax = 0; ax < 9; ax++)
	{
		char buf[MAX_PATH];

		_snprintf_s(buf, (size_t)255, "%d", ax);

		if (strstr(pszString.c_str(), buf))
		{
			return true;
		}
	}
	return false;
}

void MSavePanel::Draw()
{
	static std::string strPath, BasePath;
	static bool doOnce = true;
	if (doOnce)
	{
		char* szPath = new char[255];
		GetModuleFileNameA(GetModuleHandle(NULL), szPath, 255);
		for (int i = strlen(szPath); i > 0; i--)
		{
			if (szPath[i] == '\\')
			{
				szPath[i + 1] = 0;
				break;
			}
		}

		strPath = szPath;
		delete[] szPath;
		BasePath = strPath;
		doOnce = false;

	}
	int alpha = m_pMenu->alpha;
	//this->position = m_pMenu->MainWindow->position;
	//this->position.x += 10;
	//this->position.y += m_pMenu->MainWindow->dragYoffset + 10;
	//this->size.x = (m_pMenu->MainWindow->size.x - 20) / 2; //left, right and middle shit offsets
	this->position = this->parent->position;
	this->position.x -= 190;
	this->position.y += 155;
	this->size.y = this->parent->size.y - 40;




	/*textbox*/
	int textbox_x = position.x;
	int textbox_y = position.y;
	int textbox_h = 20;
	int textbox_w = 130;
	bool bTexthover = m_pMenu->MainWindow->InBounds(Vector(textbox_x, textbox_y, 0), Vector(textbox_w, textbox_h, 0));

	if (!this->textboxactive && bTexthover && m_pMenu->MainWindow->bMouse1released)
	{

		this->textboxactive = !this->textboxactive;
	}


	if (textboxactive)
			g_pRender->FilledBox(textbox_x, textbox_y, textbox_w, textbox_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else if (bTexthover)
		g_pRender->FilledBox(textbox_x, textbox_y, textbox_w, textbox_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
		g_pRender->GradientBox(textbox_x, textbox_y, textbox_w, textbox_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	if (this->textboxactive)
	{
		for (int i = 0; i < 255; i++)
		{
			if (m_pMenu->MainWindow->ToggleButton(i))
			{
				HandleKeys(i);
			}
		}
		if (m_pMenu->MainWindow->bMouse1pressed && !m_pMenu->MainWindow->InBounds(Vector(textbox_x, textbox_y, 0), Vector(textbox_w, textbox_h, 0)))
		{

			this->textboxactive = false;
		}
	}
	if (command.empty() && !(bTexthover && m_pMenu->MainWindow->bMouse1pressed) && !textboxactive)
		g_pRender->Text("[enter text]", textbox_x + textbox_w / 2, textbox_y + textbox_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	g_pRender->Text((char*)command.c_str(), textbox_x + textbox_w / 2, textbox_y + textbox_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	/*end textbox*/

	/*save button*/
	int save_new_button_x = position.x;
	int save_new_button_y = position.y + 30;
	int save_new_button_h = 20;
	int save_new_button_w = 130;
	bool save_new_button_hover = m_pMenu->MainWindow->InBounds(Vector(save_new_button_x, save_new_button_y, 0), Vector(save_new_button_w, save_new_button_h, 0));

	if (save_new_button_hover && m_pMenu->MainWindow->bMouse1pressed)
		g_pRender->GradientBox(save_new_button_x, save_new_button_y, save_new_button_w, save_new_button_h,  D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	else if (save_new_button_hover)
		g_pRender->FilledBox(save_new_button_x, save_new_button_y, save_new_button_w, save_new_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
		g_pRender->GradientBox(save_new_button_x, save_new_button_y, save_new_button_w, save_new_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	g_pRender->Text("Create Preset", save_new_button_x + save_new_button_w / 2, save_new_button_y + save_new_button_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

	if (!this->textboxactive && save_new_button_hover && m_pMenu->MainWindow->bMouse1released)
	{
		m_pMenu->SaveWindowState(command);
		command.clear();
	}
	/*end save button*/


	/*config list getting*/
	strPath = BasePath;
	std::vector<std::string> ConfigList;
	DIR *pDIR;
	struct dirent *entry;
	std::string buffer;
	if (pDIR = opendir(strPath.c_str()))
	{
		while (entry = readdir(pDIR))
		{

			if (strstr(entry->d_name, ".nec"))
			{
				buffer = entry->d_name;
				if (!buffer.find("_walkbot") != std::string::npos)
				{
					std::string str2 = buffer.substr(0, buffer.length() - 4);
					ConfigList.push_back(str2);
				}

			}
		}
		closedir(pDIR);
	}
	/*end config list getting*/

	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	/*config list*/
	int list_x = position.x + 160;
	int list_y = position.y;
	int list_w = 200;
	int list_item_h = 20;


	g_pRender->BorderedBox(list_x, list_y - 96, list_w, list_item_h * 20, D3DCOLOR_ARGB(alpha, 15, 15, 15));
	g_pRender->BorderedBox(list_x + 1, list_y - 97, list_w - 2, list_item_h * 20, BLACK(alpha));

	for (int i = 0; i < ConfigList.size(); i++)
	{
		if (m_pMenu->MainWindow->bMouse1pressed && m_pMenu->MainWindow->InBounds(Vector(list_x, (list_y - 95) + list_item_h * i, 0), Vector(list_w, list_item_h, 0)))
		{

			this->SelectIndex = i;

		}
		if (i == this->SelectIndex)
			g_pRender->FilledBox(list_x + 1, (list_y - 95) + list_item_h * i, list_w - 2, list_item_h, D3DCOLOR_ARGB(alpha,15, 15, 15));

		else
		{
			if (m_pMenu->MainWindow->InBounds(Vector(list_x, (list_y - 95) + list_item_h * i, 0), Vector(200, 15, 0)))
				g_pRender->GradientBox(list_x + 1, (list_y - 95) + list_item_h * i, list_w - 2, list_item_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
			else
				g_pRender->GradientBox(list_x + 1, (list_y - 95) + list_item_h * i, list_w - 2, list_item_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
		}



		g_pRender->Text((char*)ConfigList.at(i).c_str(), list_x + list_w / 2, (list_y - 95) + list_item_h * i + list_item_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	}


	/*end config list*/

	/*current config*/

	//g_pRender->Text((char*)current_config.c_str(), position.x, position.y + size.y / 2 - g_pRender->menu_control_size, lefted, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

	/*end current config*/

	/*save button*/
	int save_button_w = 130;
	int save_button_h = 20;
	int save_button_x = position.x;
	int save_button_y = position.y + size.y - save_button_h * 3 - 10 - 10;

	bool save_button_hover = m_pMenu->MainWindow->InBounds(Vector(save_button_x, save_button_y, 0), Vector(save_button_w, save_button_h, 0));



	if (save_button_hover && m_pMenu->MainWindow->bMouse1pressed)
		g_pRender->FilledBox(save_button_x, save_button_y, save_button_w, save_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else if (save_button_hover)
		g_pRender->FilledBox(save_button_x, save_button_y, save_button_w, save_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
		g_pRender->GradientBox(save_button_x, save_button_y, save_button_w, save_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	g_pRender->Text("Save Preset", save_button_x + save_button_w / 2, save_button_y + save_button_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	if (save_button_hover && m_pMenu->MainWindow->bMouse1released)
	{
		m_pMenu->SaveWindowState(ConfigList.at(this->SelectIndex));
	}
	/*end save button*/
	/*load button*/
	int load_button_w = 130;
	int load_button_h = 20;
	int load_button_x = position.x;
	int load_button_y = position.y + size.y - load_button_h * 2 - 10;

	bool load_button_button_hover = m_pMenu->MainWindow->InBounds(Vector(load_button_x, load_button_y, 0), Vector(load_button_w, load_button_h, 0));



	if (load_button_button_hover && m_pMenu->MainWindow->bMouse1pressed)
		g_pRender->FilledBox(load_button_x, load_button_y, load_button_w, load_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else if (load_button_button_hover)
		g_pRender->FilledBox(load_button_x, load_button_y, load_button_w, load_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
	g_pRender->GradientBox(load_button_x, load_button_y, load_button_w, load_button_h, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	g_pRender->Text("Load Preset", load_button_x + load_button_w / 2, load_button_y + load_button_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	if (load_button_button_hover && m_pMenu->MainWindow->bMouse1released)
	{
		m_pMenu->LoadWindowState(ConfigList.at(this->SelectIndex));
		current_config = ConfigList.at(this->SelectIndex);
	}
	/*end load button*/
	/*delete button*/
	int delete_w = 130;
	int delete_h = 20;
	int delete_x = position.x;
	int delete_y = position.y + size.y - delete_h;

	bool delete_button_hover = m_pMenu->MainWindow->InBounds(Vector(delete_x, delete_y, 0), Vector(delete_w, delete_h, 0));



	if (delete_button_hover && m_pMenu->MainWindow->bMouse1pressed)
		g_pRender->FilledBox(delete_x, delete_y, delete_w, delete_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else if (delete_button_hover)
		g_pRender->FilledBox(delete_x, delete_y, delete_w, delete_h, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
		g_pRender->GradientBox(delete_x, delete_y, delete_w, delete_h,  D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);
	g_pRender->Text("Delete Preset", delete_x + delete_w / 2, delete_y + delete_h / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	if (delete_button_hover && m_pMenu->MainWindow->bMouse1released)
	{
		std::string buf = strPath;
		buf.append("\\");
		buf.append(ConfigList.at(this->SelectIndex));
		buf.append(".cfg");

		DeleteFileA(buf.c_str());
	}
}