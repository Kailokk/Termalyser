#include "MenuSystem.h"
#include <filesystem>
#include "ftxui/component/screen_interactive.hpp"
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
	int menuSelection = 0;
	MenuOption menuOption;
	menuOption.on_enter = screen.ExitLoopClosure();
	auto menu = Menu(&menu_entries, &menuSelection, menuOption);
	menu = Wrap("Visualisation Mode", menu);
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
	settings->visMode = static_cast<VisualiserMode>(menuSelection);
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


