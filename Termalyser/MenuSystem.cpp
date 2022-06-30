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
	//Creates Screen
	auto screen = ScreenInteractive::FitComponent();
	Component output;
	std::string titleBar = "Please select your options followed by pressing the confirm button.";

	auto blank = Renderer([&]
		{
			Canvas c = Canvas(0, 0);
			return canvas(std::move(c));
		});

	//Renders Title
	auto title = Renderer([&]
		{
			return hbox({
					   text(titleBar) | size(WIDTH, EQUAL, 80),
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

	bool clearScreen = false;

	std::function<void()> Confirmation = [&screen, &settings, &titleBar, &output, &clearScreen]
	{
		if (CheckPathValid(settings->path))
		{
			std::function<void()> close = screen.ExitLoopClosure();
			close();
		}
		else
		{
			titleBar = "Please Enter A Valid Path!";
		}
	};

	InputOption option;
	option.on_enter = Confirmation;

	auto input = Input(settings->path, "File Path");
	input = Wrap("FilePath", input);

	std::string confirmButton_label = "Confirm";

	auto confirm = Button(&confirmButton_label, Confirmation);
	std::string quitButton_label = "Quit";
	std::function<void()> on_button_clicked_;
	auto quit = Button(&quitButton_label, screen.ExitLoopClosure());
	auto buttons = Container::Horizontal({
		quit, confirm,
		});



	auto layout = Container::Vertical({
		title,
		 radiobox,
		 input,
	   buttons
		});


	output = Renderer(layout, [&]
		{
				return vbox({
							title->Render(),
							separator(),
							separator(),
						   radiobox->Render(),
						   separator(),
						   input->Render(),
						   separator(),
						   buttons->Render(),
					}) |
					xflex | size(WIDTH, GREATER_THAN, 80) | border;
		});

	screen.Loop(output);
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


