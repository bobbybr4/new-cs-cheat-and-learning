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

class MSlider : public MBaseElement
{
public:
	MSlider() {}

	bool bActive;
	MBaseElement* parent;
	float min, max, value;
	bool round;
	int width;
	MSlider(MBaseElement* parent, std::string name, std::string caption, float min, float max, bool round, MCategory* SaveCategory, std::string savename);
	virtual void Draw() override;
};