#include "InputMenu.h"
#include <functional>

using namespace ftxui;

// Display a component nicely with a title on the left.
Component InputMenu::Wrap(std::string name, Component component)
{
	return Renderer(component, [name, component]
		{
			return hbox({
					   text(name) | size(WIDTH, EQUAL, 16),
					   separator(),
					   component->Render() | xflex,
				}) |
				xflex;
		});
}

bool InputMenu::ShowMenu()
{
	//Creates Screen
	auto screen = ScreenInteractive::FitComponent();

	//Renders Title
	auto title = Renderer([&]
		{
			return hbox({
					   text(titleText) | size(WIDTH, EQUAL, 80),
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


	auto input = Input(path, "File Path");
	input = Wrap("FilePath", input);

	InputMenu* thisMenu = this;

	

	std::string confirmButton_label = "Confirm";
	std::function<void()> Confirmation = [&] { thisMenu->Confirmation(thisMenu->screen); };
	ftxui::Closure func = Confirmation;
	auto confirm = Button(&confirmButton_label, func);
	screen.ExitLoopClosure();

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

	auto component = Renderer(layout, [&]
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

	screen.Loop(component);
	return returnValue;
}

void InputMenu::Confirmation(ScreenInteractive& screen)
{
	if (CheckValidPath())
	{
		returnValue = true;
		screen.ExitLoopClosure();
	}
	else
	{
		titleText = "Invalid Path Provided, please try again!";
	}
}

bool InputMenu::CheckValidPath()
{
	if (path->find('.') != std::string::npos)
	{
		if (path->find('/') != std::string::npos)
		{
			return true;
		}
	}
	return false;
}