#include "MenuDropdown.h"

MDropdown::MDropdown(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename, std::vector<std::string> Items)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(parent->size.x - 20, 17, 0);
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_DropDown;
	this->Items = Items;
	this->SelectedIndex = 0;
	this->IsOpen = false;

	/*Set save parent array*/


	parent->AddChildControl(this);


	if (SaveCategory != nullptr)
		SaveCategory->SaveChildren.emplace_back(this);
}

bool m_bActiveComboBox = false;
void MDropdown::Draw()
{
	this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 49;


	MTab* pTab = (MTab*)parent;


	for (int i = 0; i < this->parent->Children.size(); i++)
		if (this->parent->Children.at(i) != (MBaseElement*)this)
		{
			this->position.y += this->parent->Children.at(i)->size.y; //boxes are 13 px big, that means 7 difference is natural
		}
		else
			break;


	auto nSize = Items.size();

	int alpha = m_pMenu->alpha;



	int bar_height = 13;
	int y_pos = this->position.y;
	int width = this->size.x;

	bool bHovering = m_pMenu->MainWindow->InBounds(Vector(position.x, y_pos, 0), Vector(width, bar_height, 0));
	if (m_pMenu->MainWindow->BlockInput && m_pMenu->MainWindow->BlockingElement != (MBaseElement*)this)
		bHovering = false;
	if (!m_pMenu->MainWindow->BlockInput || m_pMenu->MainWindow->BlockInput && m_pMenu->MainWindow->BlockingElement == this)
	{
		if (bHovering && m_pMenu->MainWindow->bMouse1released && !m_bActiveComboBox && !IsOpen)
		{
			IsOpen = true;
			m_bActiveComboBox = !m_bActiveComboBox;
		}
	}


	//if (IsOpen)

	if (bHovering || IsOpen)
		g_pRender->FilledBox(this->position.x, y_pos, width, bar_height, D3DCOLOR_ARGB(alpha, 25, 25, 25));
	else
		g_pRender->GradientBox(this->position.x, y_pos, width, bar_height, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20), true);

	if (Items.size() > 0)
	{
		if (IsOpen)
		{
			m_bActiveComboBox = true;

			m_pMenu->MainWindow->BlockInput = true;
			m_pMenu->MainWindow->BlockingElement = this;
			auto Y = y_pos + bar_height;


			for (int i = 0; i < Items.size(); i++)
			{

				auto bHover = m_pMenu->MainWindow->InBounds(Vector(position.x, Y + i*bar_height, 0), Vector(size.x, bar_height, 0));
				if (bHover)
				{
					g_pRender->FilledBox(position.x, Y + i*bar_height, width, bar_height, D3DCOLOR_ARGB(alpha, 22, 22, 22));
				}
				else
					g_pRender->FilledBox(position.x, Y + i*bar_height, width, bar_height, D3DCOLOR_ARGB(alpha, 15, 15, 15));
				g_pRender->Text((char*)Items[i].c_str(), position.x + width / 2, Y + i*bar_height + 2 + bar_height / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));


		

				if (bHover && m_pMenu->MainWindow->bMouse1released)
				{
					m_pMenu->MainWindow->BlockInput = false;
					m_pMenu->MainWindow->BlockingElement = nullptr;
					m_bActiveComboBox = false;
					IsOpen = false;
					SelectedIndex = i;
				}
				if (m_pMenu->MainWindow->bMouse1pressed && !m_pMenu->MainWindow->InBounds(Vector(position.x, Y, 0), Vector(width, bar_height * Items.size(), 0)))
				{
					IsOpen = false;
					m_bActiveComboBox = false;
					m_pMenu->MainWindow->BlockInput = false;
					m_pMenu->MainWindow->BlockingElement = nullptr;
				}
			}
		}
	}


	if (this->SelectedIndex >= 0 && this->SelectedIndex < Items.size())
	{
		char buf[256];
		sprintf_s(buf, "%s: %s", name.c_str(), Items[this->SelectedIndex].c_str());

		g_pRender->Text((char*)buf, position.x + width / 2, y_pos + 2 + bar_height / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	}
	else
	{
		std::string meme = name;
		meme += ":";

		g_pRender->Text((char*)meme.c_str(), position.x + width / 2, y_pos + 2 + bar_height / 2 - g_pRender->menu_control_size / 2, centered, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));
	}
}