#pragma once

#define _HAS_CXX17 1 // c++17
#define _CRT_SECURE_NO_WARNINGS

#include "Common.h"
#include "sdk/SDK.h"

#include "hack/Offsets.h"
#include "hack/Basics/NetvarManager.h"
#include "hack/Basics/CHook.h"
#include "hack/Global.h"
#include "hack/Basics/Util.h"
#include "GameEvents.h"
#include "hack/Basics/Math.h"
#include "hack/Hooks.h"
#include "hack/Basics/Config.h"
#include "hack/Basics/ConfigLoader.h"

#include "hack/Helpers/Prediction.h"
#include "hack/Helpers/Autowall.h"
#include "hack/Helpers/DrawManager.h"
#include "hack/Helpers/Backtracking.h"
#include "hack/Helpers/Lagcompensation.h"
#include "hack/Helpers/Resolver.h"
#include "hack/Helpers/GrenadePrediction.h"
#include "hack/Helpers/Antiaim.h"

#include "hack/Components/Aimbot.h"
#include "hack/Components/Miscellaneous.h"
#include "hack/Components/Visuals.h"

#include "hack/Menu/Render.h"

// verry p, verry organized

enum UIControlTypes
{
	UIC_CheckBox = 1,
	UIC_Slider,
	UIC_KeyBind,
	UIC_DropDown,
	UIC_ColorPicker,
	UIC_Textbox
};

class MBaseElement
{
public:
	std::string name, caption;
	std::vector<MBaseElement*> Children;
	Vector position, size = Vector(0, 15, 0);
	bool visible = false;
	bool in_tab;
	MBaseElement() {}
	MBaseElement(std::string name, std::string caption, Vector pos, Vector size)
	{
		this->name = name;
		this->caption = caption;
		this->position = pos;
		this->size = size;
	}
	void AddChildControl(MBaseElement * Child)
	{
		//Children.push_back(Child);
		Children.emplace_back(Child);
	}
	std::string FileIdentifier;
	int FileControlType;

	virtual void Draw() = 0;
	Color White = Color(255, 255, 255, 255);
	Color Black = Color(0, 0, 0, 255);
	Color Yellow = Color(255, 255, 0, 255);
	Color DarkGrey = Color(35, 35, 35, 255);
	Color MidGrey = Color(44, 44, 44, 255);
	Color LightGrey = Color(106, 106, 106, 255);
	Color LightRed = Color(186, 0, 40, 255);
	Color pWhite = Color(0, 102, 51, 255);
	Color OutlineGrey = Color(86, 86, 86, 255);
};


#include "hack/Menu/Menu.h"

#include "hack/Menu/MenuCategory.h"
#include "hack/Menu/MenuWindow.h"

#include "hack/Menu/MenuSmallWindow.h"
#include "hack/Menu/MenuGroupbox.h"
#include "hack/Menu/MenuDropdown.h"
#include "hack/Menu/MenuColorpicker.h"
#include "hack/Menu/MenuKeybind.h"
#include "hack/Menu/MenuTextbox.h"
#include "hack/Menu/MenuSavePanel.h"
#include "hack/Menu/MenuCheckbox.h"
#include "hack/Menu/MenuSlider.h"
#include "hack/Menu/MenuTab.h"

#include "hack/Menu/MenuBaseElement.h"