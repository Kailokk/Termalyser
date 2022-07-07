#pragma once
#include "VisualiserSettings.h"

#include <string>

#include "ftxui/component/component.hpp" 

bool ShowMenu(VisualiserSettings* settings);
bool CheckPathValid(std::string& path);
ftxui::Component Wrap(std::string name, ftxui::Component component);


//return render for each visualisation -----------------------------------------------------------------

//Oscilloscope
ftxui::Component Oscilloscope(float**& bufferPointer, bool& clearScreen);

//Dot Visualiser

//Spectrum Analyser
ftxui::Component SpectrumAnalyser(float**& bufferPointer, bool& showVisualisation)


//Bar Spectrum Analyser

//3D Spectrum Analyser