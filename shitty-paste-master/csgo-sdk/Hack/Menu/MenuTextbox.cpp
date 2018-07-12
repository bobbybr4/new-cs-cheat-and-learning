#include "MenuTextbox.h"

char* KeyStrings1[254] = { "not bound", "left mouse", "right mouse", "control+break", "middle mouse", "mouse 4", "mouse 5",
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

void MTextbox::HandleKeys(int key)
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
	case VK_DELETE:
		IsActive = false;
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
		KeyName = KeyStrings1[key];
		//if (GetKeyNameText(key << 16, KeyName, 127))
		if (KeyName != "Backspace" && KeyName != "SHIFT" && KeyName != "CTRL" && KeyName != "ALT")
			command.append(KeyName);

		return;
	}
}
MTextbox::MTextbox(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(parent->size.x - 20, 15, 0);
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_Textbox;

	parent->AddChildControl(this);
	if (SaveCategory != nullptr)
		SaveCategory->SaveChildren.push_back(this);
}
void MTextbox::Draw()
{
	this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 20;

	int width = size.x + 20;

	int alpha = m_pMenu->alpha;

	for (int i = 0; i < this->parent->Children.size(); i++)
	if (this->parent->Children.at(i) != (MBaseElement*)this)
	this->position.y += this->parent->Children.at(i)->size.y; //boxes are 15 px big, that means 5 difference is natural

	bool bHover = m_pMenu->MainWindow->InBounds(position, size);

	if (bHover && m_pMenu->MainWindow->bMouse1released && !IsActive)
	IsActive = true;

	if (IsActive)
		g_pRender->FilledBoxOutlined(this->position.x, this->position.y, 165, 20, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20));
	else if (bHover)
		g_pRender->FilledBoxOutlined(this->position.x, this->position.y, 165, 20, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20));
	else
		g_pRender->FilledBoxOutlined(this->position.x, this->position.y, 165, 20, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20));

	if(command.empty() && !(bHover && m_pMenu->MainWindow->bMouse1pressed) && !IsActive)
		g_pRender->Text((char*)this->name.c_str(), this->position.x + 165 / 2, this->position.y + 20 / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));

	if (IsActive)
	{
	for (int i = 0; i < 255; i++)
	{
	if (m_pMenu->MainWindow->ToggleButton(i))
	{
	HandleKeys(i);
	}
	}
	if (m_pMenu->MainWindow->bMouse1released && !bHover)
	IsActive = false;
	}

	g_pRender->Text((char*)command.c_str(), this->position.x + 170 / 2, this->position.y + 20 / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
}