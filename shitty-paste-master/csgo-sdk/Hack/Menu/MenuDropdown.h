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

class MDropdown : public MBaseElement
{
public:
	MDropdown() {}

	MBaseElement* parent;
	std::vector<std::string> Items;
	int SelectedIndex;
	bool IsOpen;
	MDropdown(MBaseElement* parent, std::string name, std::string caption, MCategory* SaveCategory, std::string savename, std::vector<std::string> Items);
	virtual void Draw() override;
};