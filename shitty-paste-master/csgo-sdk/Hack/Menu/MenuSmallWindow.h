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

class MSmallWindow : public MBaseElement
{
public:

	bool bRender;
	bool bDragging;
	MBaseElement* parent;
	MSmallWindow(std::string name, Vector pos, Vector size, MBaseElement* parent, bool render_if_no_menu);
	virtual void Draw() override;
	int iXDif = 0;
	int iYDif = 0;
};