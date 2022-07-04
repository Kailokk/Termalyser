#pragma once

#include "VisualiserSettings.h"

#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab

//return render for each visualisation

//Oscilloscope

//Spectrum Analyser

//3D Spectrum Analyser


ftxui::Component Oscilloscope(float* buffer);

ftxui::Component TestComponent();