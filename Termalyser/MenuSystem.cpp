#include "MenuSystem.h"

#include <fstream>
#include <thread>

#include "ftxui/component/component.hpp" 
#include "ftxui/component/captured_mouse.hpp"  
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"

using namespace ftxui;
bool FinishGraphicsLoop = false;
bool FinishThreadLoop = false;

Component Wrap(std::string name, Component component)
{
	return Renderer(component, [name, component]
		{
			return hbox({
					   text(name) | size(WIDTH, EQUAL, 16),
					   separator(),
					   component->Render() | xflex,
				}) | xflex;
		});
}

//Creates Screen
VisualiserSettings* visSettings;

void ShowMenu(VisualiserSettings* settings)
{
	visSettings = settings;

	auto screen = ScreenInteractive::FitComponent();

	//Renders Title
	auto title = Renderer([&]
		{
			return hbox({
					   text("Please choose your visualisation mode, and provide a valid file path") | size(WIDTH, EQUAL, 80),
				}) |
				xflex;
		});


	//Radiobox for visualisation
	int visualisation_selected = 0;
	std::vector<std::string> visualisation_entries = {
		"Oscilloscope",
		"Frequency Response",
		"3D Frequency Plot",
		"Particle Oscilloscope",
	};
	auto radiobox = Radiobox(&visualisation_entries, &visualisation_selected);
	radiobox = Wrap("Visualisation", radiobox);

	InputOption pathOption;
	pathOption.on_enter = screen.ExitLoopClosure();
	auto input = Input(&settings->path, "File Path", pathOption);
	input = Wrap("FilePath", input);


	

	std::string quitButton_label = "Play";
	std::function<void()> on_button_clicked_;
	auto quit = Button(&quitButton_label, screen.ExitLoopClosure());
	
	auto layout = Container::Vertical({
		title,
		 radiobox,
		 input,
	   quit
		});

	auto component = Renderer(layout, [&]
		{
			return vbox({
						title->Render(),
						separator(),
					   radiobox->Render(),
					   separator(),
					   input->Render(),
					   separator(),
					   quit->Render(),
				}) |
				xflex | size(WIDTH, GREATER_THAN, 80) | border;
		});
	
	screen.Loop(component);
	screen.Clear();
}


bool CheckPathValid(VisualiserSettings* visSettings)
{
	std::ifstream test(visSettings->path);
	if (!test)
	{
	return true;
	}
	return false;
}


