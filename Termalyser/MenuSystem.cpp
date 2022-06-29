#include "MenuSystem.h"

#include <fstream>
#include <filesystem>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>

#include "ftxui/component/component.hpp" 
#include "ftxui/component/captured_mouse.hpp"  
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

Component Wrap(std::string name, Component component)
{
	return Renderer(component, [name, component]
		{
			return hbox({
					   text(name) | size(WIDTH, EQUAL, 20),
					   separator(),
					   component->Render() | xflex,
				}) | xflex;
		});
}


//Creates menu and loops until user selects a preset
void ShowMenu(VisualiserSettings* settings)
{
	//Initialise Screen
	ScreenInteractive screen = ScreenInteractive::FitComponent();

	Component title = Renderer([&]
		{
			return hbox({
					   text("Please Select Your Visualisation Mode and Press Enter") | size(WIDTH, EQUAL, 60),

				}) | xflex;
		});



	const std::vector<std::string> menu_entries = {
	 "OSCILLOSCOPE",
	 "SPECTRUM ANALYSER",
	 "SPECTRUM PLOT",
	};
	int menu_selected = 0;
	MenuOption menuOption;
	menuOption.on_enter = screen.ExitLoopClosure();
	auto menu = Menu(&menu_entries, &menu_selected, menuOption);
	menu = Wrap("Visualisation \nMode", menu);

	auto component = Renderer(menu, [&]
		{
			return vbox(
				{
				title->Render(),
				separator(),
				menu->Render(),
				}) |
				xflex | size(WIDTH, GREATER_THAN, 40) | border;
		});

	screen.Loop(component);
	settings->visMode = static_cast<VisualiserMode>(menu_selected);
}


bool CheckPathValid(std::string& path)
{
	using namespace std::filesystem;
	if (std::filesystem::exists(path))
	{
		if (path.find(".") != std::string::npos)
		{
			return true;
		}
	}
	return false;
}


