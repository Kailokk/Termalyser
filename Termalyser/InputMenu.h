#pragma once
#include <string>
#include "ftxui/component/component.hpp" 

#include "ftxui/component/captured_mouse.hpp"  
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"
enum Visualisation
{
};

class InputMenu
{

private:

	std::string titleText = "Please enter your settings";

	bool CheckValidPath();
	ftxui::Component Wrap(std::string name, ftxui::Component component);

	void Confirmation(ftxui::ScreenInteractive& screen);
	bool PathValid;
	bool returnValue = false;

public: 

	std::string* path;
	//Visualisation vis;

	//InputMenu();

	bool ShowMenu();
	
	

};

