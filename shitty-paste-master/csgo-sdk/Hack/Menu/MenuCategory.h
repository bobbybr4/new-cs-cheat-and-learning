#pragma once
#include "MenuBaseElement.h"
#include "MenuControls.h"

class MBaseElement;
class MWindow;
class MCheckbox;
class MCategory;
class MColorPicker;
class MDropdown;
class MKeybind;
class MSavePanel;
class MSmallWindow;
class MTab;
class MTextbox;
class MGroupbox;
class MSlider;

class MCategory : public MBaseElement
{
public:
	MCategory() {}
	LPDIRECT3DTEXTURE9 texture;
	BYTE* tex_array;
	UINT texture_size;
	float res_x, res_y;
	MBaseElement* parent;
	static MCategory* CurrentCategory;
	std::vector<MBaseElement*> SaveChildren;
	bool active = false;
	MCategory(std::string name, MWindow * parent, BYTE* texture, UINT size, int res_x, int res_y);
	virtual void Draw() override;
};