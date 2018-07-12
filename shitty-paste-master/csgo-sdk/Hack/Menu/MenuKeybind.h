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

class MKeybind : public MBaseElement
{
public:
	MKeybind() {}
	MBaseElement* parent;
	int Key;
	bool IsGettingKey;
	MKeybind::MKeybind(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename);
	virtual void Draw() override;
};