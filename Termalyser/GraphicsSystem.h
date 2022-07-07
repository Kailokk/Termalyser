#pragma once
#include "ftxui/component/component.hpp" 
#include "VisualiserSettings.h"
#include "VisualiserSettings.h"
#include "ftxui/component/component.hpp"  


bool ShowMenu(VisualiserSettings* settings);
bool CheckPathValid(std::string& path);



//return render for each visualisation -----------------------------------------------------------------

//Oscilloscope
ftxui::Component Oscilloscope(float**& bufferPointer, bool& clearScreen);

//Dot Visualiser

//Spectrum Analyser

//Bar Spectrum Analyser

//3D Spectrum Analyser