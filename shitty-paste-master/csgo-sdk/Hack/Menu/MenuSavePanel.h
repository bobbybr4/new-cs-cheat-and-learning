#pragma once
#include "MenuBaseElement.h"
#include "MenuControls.h"
#include "dirent.h"

class MBaseElement;
class MSavePanel : public MBaseElement
{
public:
	MSavePanel() {}
	int SelectIndex;
	MBaseElement* parent;
	bool textboxactive = false;
	MSavePanel(MBaseElement* parent);
	std::string command;
	std::string current_config;
	void HandleKeys(int key);
	virtual void Draw() override;
};