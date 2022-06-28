#pragma once
#include <string>
#include "ftxui/component/component.hpp" `

enum VisualiserMode
{
	VISMODE_OSC,
	VISMODE_SPECT,
	VISMODE_TIMESPECT
};

struct VisualiserSettings
{
	std::string path;
	VisualiserMode visMode;
};

extern VisualiserSettings* visSetings;
extern bool FinishGraphicsLoop;
extern bool FinishThreadLoop;


void ShowMenu(VisualiserSettings* settings);
bool CheckPathValid(VisualiserSettings* visSettings);

