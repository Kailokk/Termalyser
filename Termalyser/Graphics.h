#pragma once
#include "VisualiserSettings.h"
#include "ftxui/component/component.hpp" 
#include "ftxui/component/component_base.hpp" 



bool ShowMenu(VisualiserSettings* settings);
bool CheckPathValid(std::string& path);
Component Wrap(std::string name, Component component);

//return render for each visualisation mode ---------------------------------

//Oscilloscope
ftxui::Component Oscilloscope(float**& bufferPointer, bool& clearScreen);

//Dot Visualiser

//Spectrum Analyser

//Bar Spectrum Analyser

//3D Spectrum Analyser

