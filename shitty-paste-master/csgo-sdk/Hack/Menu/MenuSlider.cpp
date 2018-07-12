#include "MenuSlider.h"


MSlider::MSlider(MBaseElement* parent, std::string name, std::string caption, float min, float max, bool round, MCategory* SaveCategory, std::string savename)
{
	this->parent = parent;
	this->name = name;
	this->size = Vector(parent->size.x - 20, 32, 0); //14 for font, then 2 space and 14 for slider 
	this->caption = caption;
	this->FileIdentifier = savename;
	this->FileControlType = UIControlTypes::UIC_Slider;
	this->min = min;
	this->max = max;
	this->round = round;
	this->value = 0;
	this->bActive = false;
	/*Set save parent array*/

	parent->AddChildControl(this);


	if (SaveCategory != nullptr)
		SaveCategory->SaveChildren.emplace_back(this);
}

void MSlider::Draw()
{

	this->position = this->parent->position;
	this->position.x += 10;
	this->position.y += 10;

	this->size.x = parent->size.x - 49;

	int old_size_y = this->size.y;
	int width = size.x;

	MTab* pTab = (MTab*)parent;


	for (int i = 0; i < this->parent->Children.size(); i++)
		if (this->parent->Children.at(i) != (MBaseElement*)this)
		{
			this->position.y += this->parent->Children.at(i)->size.y; //boxes are 15 px big, that means 5 difference is natural
		}
		else
			break;


	int bar_pos_y = this->position.y + g_pRender->menu_control_size + 2;
	int bar_height = 7;


	if (!m_pMenu->MainWindow->BlockInput)
	{
		if (m_pMenu->MainWindow->InBounds(Vector(position.x, bar_pos_y, 0), Vector(size.x, bar_height, 0)))
		{
			bActive = true;
		}
		if (bActive)
		{
			if (m_pMenu->MainWindow->bMouse1pressed)
			{


				POINT m = m_pMenu->MainWindow->Cur;
				float NewX;
				float Ratio;
				NewX = m.x - position.x;//-1
				if (NewX < 0)
					NewX = 0;
				if (NewX > width)
					NewX = width;
				Ratio = NewX / float(width);
				value = min + (max - min)*Ratio;
				if (this->round)
					value = (int)floorf(value);
			}
			else
				bActive = false;
		}
	}
	float Ratio = (value - min) / (max - min);

	float Location = Ratio * width;

	int alpha = m_pMenu->alpha;


	g_pRender->GradientBox(this->position.x, bar_pos_y, width, bar_height, D3DCOLOR_ARGB(alpha, 25, 25, 25), D3DCOLOR_ARGB(alpha, 20, 20, 20),  true);


	if (this->value != 0)
	{
		if (m_pMenu->MainWindow->InBounds(Vector(position.x, bar_pos_y, 0), Vector(size.x, bar_height, 0)) && !m_pMenu->MainWindow->BlockInput)
			g_pRender->GradientBox(position.x, bar_pos_y, Location, bar_height, m_pMenu->color, m_pMenu->color, false);
		else
			g_pRender->GradientBox(position.x, bar_pos_y, Location, bar_height, m_pMenu->color, m_pMenu->color, false);
		//	g_pRender->FilledBox(position.x, bar_pos_y, Location, bar_height, D3DCOLOR_ARGB(alpha, 0, 162, 232));
	}

	char buffer[24];
	if (this->round)
		sprintf_s(buffer, "%i", (int)value);
	else
		sprintf_s(buffer, "%.2f", value);


	g_pRender->Text((char*)buffer, position.x + width, position.y + 1, righted, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));




	g_pRender->Text((char*)name.c_str(), position.x, position.y + 1, lefted, g_pRender->Fonts.menu_control, true, WHITE(alpha), BLACK(alpha));



	//if (!m_pMenu->MainWindow->BlockInput)
	//if (m_pMenu->MainWindow->InBounds(position, size))
	//	g_Draw.StringA(g_Draw.font_menu, true, m_pMenu->MainWindow->position.x + m_pMenu->MainWindow->size.x / 2, m_pMenu->MainWindow->position.y + m_pMenu->MainWindow->size.y + m_pMenu->MainWindow->dragYoffset + m_pMenu->MainWindow->dragYoffset / 2-6, 255, 255, 255, 255, "%s", this->caption.c_str());
}