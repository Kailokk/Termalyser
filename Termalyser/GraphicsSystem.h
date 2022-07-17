#pragma once
#include "VisualiserSettings.h"

#include <string>

#include "ftxui/component/component.hpp" 

bool ShowMenu(VisualiserSettings* settings);
bool CheckPathValid(std::string& path);
ftxui::Component Wrap(std::string name, ftxui::Component component);


//return render for each visualisation -----------------------------------------------------------------
int LerpBuffer(int x_not_scaled, float* stereoCombination, ftxui::Canvas& c);
//Oscilloscope
ftxui::Component Oscilloscope(float**& bufferPointer, bool& showVisualisation);

//Dot Visualiser
ftxui::Component ParticleOscilloscope(float**& bufferPointer, bool& showVisualisation);



//Spectrum Analyser

//Bar Spectrum Analyser

//3D Spectrum Analyser