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

class MGroupbox : public MBaseElement
{
public:
	MBaseElement* parent;
	bool inside;
	int x_offset, y_offset, y_blocks;
	float x_blocks;
	std::string caption;
	MGroupbox::MGroupbox(MBaseElement* parent, std::string caption, int x_offset, int y_offset, float x_blocks, int y_blocks, bool inside);
	virtual void Draw() override;
};