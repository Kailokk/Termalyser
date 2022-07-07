#include "MenuSystem.h"
#include <filesystem>
#include "ftxui/component/screen_interactive.hpp"
using namespace ftxui;

//Small function to create the sections with the component and some text describing it
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
bool ShowMenu(VisualiserSettings* settings)
{
	bool continueProgram = false;
	//Creates Screen
	ScreenInteractive screen = ScreenInteractive::FitComponent();
	//The output renderer
	Component output;
	//bool to close the screen
	bool clearScreen = false;
	//String at top of menu
	std::string titleBar = "Please select your options followed by pressing the confirm button.";
	//function called when pressing enter on the input or the confirm button, clears the screen if the path is valid
	std::function<void()> Confirmation = [&screen, &settings, &titleBar, &output, &clearScreen, &continueProgram]
	{
		if (CheckPathValid(settings->path))
		{
			clearScreen = true;
			continueProgram = true;
		}
		else
		{
			titleBar = "Please Enter A Valid Path!";
		}
	};
	//Function called when pressing the quit button, clears screen and sets the retun falue to flase
	std::function<void()> Quit = [&clearScreen, &continueProgram]
	{
			clearScreen = true;
			continueProgram = false;
	};


	//Renders Title
	Component title = Renderer([&]
		{
			return hbox({
					   text(titleBar) | size(WIDTH, EQUAL, 80),
				}) |
				xflex;
		});


	//Radiobox for visualisation option
	int visualisation_selected = 0;
	std::vector<std::string> visualisation_entries = {
		"Oscilloscope",
		"Frequency Response",
		"3D Frequency Plot",
		"Particle Oscilloscope",
	};
	Component radiobox = Radiobox(&visualisation_entries, &visualisation_selected);
	radiobox = Wrap("Visualisation", radiobox);

	//Input bar, which outputs to the pointer to the file path. additional option to allow enter for confirm
	InputOption inputOption;
	inputOption.on_enter = Confirmation;
	Component input = Input(settings->path, "File Path", inputOption);
	input = Wrap("FilePath", input);

	//Confirm button, continues the program if the file path is valid
	Component confirm = Button("Confirm", Confirmation);
	//Quit button, exits program 
	Component quit = Button("Quit", Quit);

	//Wraps the buttons into one section
	Component buttons = Container::Horizontal({
		quit, confirm,
		});


	//Wraps all sections into one component
	Component layout = Container::Vertical({
		 title,
		 radiobox,
		 input,
		 buttons
		});

	//Checks if the screen should clear, and if not renders the menu
	output = Renderer(layout, [&]
		{
			if (clearScreen)
			{
				return vbox({});
			}
			else
			{
				return vbox({
							title->Render(),
							separator(),
							radiobox->Render(),
							separator(),
							input->Render(),
							separator(),
							buttons->Render(),
					}) |
					xflex | size(WIDTH, GREATER_THAN, 100) | border;
			}
		});

	//Thread with a loop to check if the menu is closing, and if so waits a ms for the graphic loop below to update
	bool CheckForEnd = true;
	std::thread LoopEnd_Thread([&screen, &clearScreen, &CheckForEnd]
		{
			while (CheckForEnd)
			{
				if (clearScreen)
				{
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(1ms);
					CheckForEnd = false;
					std::function<void()> close = screen.ExitLoopClosure();
					close();
				}
			}
		});
	//Loops on the menu component
	screen.Loop(output);
	LoopEnd_Thread.join();
	return continueProgram;
}




bool CheckPathValid(std::string& path)
{
	if (std::filesystem::exists(path))
	{
		if (path.find(".") != std::string::npos)
		{
			return true;
		}
	}
	return false;
}


