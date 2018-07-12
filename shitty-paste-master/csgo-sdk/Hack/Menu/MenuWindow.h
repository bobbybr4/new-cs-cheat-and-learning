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

class MWindow : public MBaseElement
{
public:
	int categoryXoffset, dragYoffset;

	bool BlockInput = false;
	MBaseElement* BlockingElement = nullptr;

	bool bDragging;
	bool IsMainWindow = false;
	bool bMouse1pressed,
		bMouse2pressed,
		bMouse1released,
		bMouse2released;
	bool moved_in_that_frame = false;
	bool ToggleButton(ButtonCode_t code);
	bool ToggleButton(int code);
	bool GetKeyPress(unsigned int key);
	void ClickHandler();
	bool Clicked(Vector position, Vector bounds);
	bool InBounds(Vector position, Vector bounds);
	MWindow(std::string name, Vector pos, Vector size, bool IsMainWindow);
	POINT Cur;

	//void DrawMouseCursor(POINT Cur);
	virtual void Draw() override;
};