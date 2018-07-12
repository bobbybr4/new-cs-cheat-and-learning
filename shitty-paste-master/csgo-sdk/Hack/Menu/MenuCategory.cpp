#include "MenuCategory.h"

MCategory* MCategory::CurrentCategory = nullptr;
MCategory::MCategory(std::string name, MWindow* parent, BYTE* texture, UINT size, int res_x, int res_y)
{
	this->name = name;
	this->parent = parent;
	this->position = parent->position;
	this->res_x = res_x;
	this->res_y = res_y;
	this->texture_size = size;
	this->tex_array = texture;
	//printf("%i", size);


	parent->AddChildControl(this);
}

void MCategory::Draw()
{
	MWindow* Parent = m_pMenu->MainWindow; //We could use m_pMenu->MainWindow-> instead, why tf tho
	int width = 400 / 4;

	int alpha = m_pMenu->alpha;
	int alpha_red = (alpha * 0.55);

	if (!this->texture)
		D3DXCreateTextureFromFileInMemoryEx(g_pRender->device, this->tex_array, this->texture_size, res_x, res_y, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &this->texture);

	this->position = parent->position;

	int height = m_pMenu->MainWindow->dragYoffset + 20;
	int index = -1;



	for (int i = 0; i < Parent->Children.size(); i++)
		if (Parent->Children.at(i) == (MBaseElement*)this)
		{
			position.x += width * i;
			index = i;
		}


	if (Parent->Clicked(Vector(this->position.x + 170, this->position.y , 0), Vector(width, height, 0)))
	{
		if (MCategory::CurrentCategory != nullptr)
			MCategory::CurrentCategory->active = false;

		this->active = true;
		MCategory::CurrentCategory = this;
	}

	/*if (index != (this->parent->Children.size() - 1))
	g_pRender->Line(this->position.x + 240, this->position.y, this->position.x + 240, this->position.y + height - 16, D3DCOLOR_ARGB(alpha, 41, 41, 41));*/

	if (this->active)
	{
		g_pRender->Line(this->position.x + 171, this->position.y + 24, this->position.x + 230, this->position.y + height - 16, m_pMenu->color);
	}
	else
	{
		g_pRender->Line(this->position.x + 171, this->position.y + 24, this->position.x + 230, this->position.y + height - 16, D3DCOLOR_ARGB(alpha, 41, 41, 41));
	}




	if (this->active)
		for (int i = Children.size() - 1; i >= 0; i--)
			Children[i]->Draw();



	//g_pRender->Sprite(this->texture, this->position.x + width / 2 - 32/*size/2 of the sprites*/, this->position.y, this->res_x, this->res_y, width / this->res_x, m_pMenu->MainWindow->dragYoffset / this->res_y, 0, WHITE(alpha));
	g_pRender->Text((char*)this->name.c_str(), this->position.x + 201, this->position.y, 1, g_pRender->Fonts.menu_icons, true, this->active ? m_pMenu->color : D3DCOLOR_ARGB(alpha, 41, 41, 41), D3DCOLOR_ARGB(alpha, 20, 20, 20));
}