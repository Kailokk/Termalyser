#include <iostream>

#include "InputMenu.h"


int main(int argc, char *argv[])
{
	//Instantiates a menu system
	InputMenu menu;

	//Checks for correct arguments, and attaches provided path if given
	switch (argc)
	{
	case (1):
			break;
	case(2):
		*menu.path = argv[1];
		break;

	default:
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " << argc - 1 << " Arguments" << std::endl;
		return 1;
		break;
	}

	//Create input menu with added file if arg provided

	if (!menu.ShowMenu())
	{
		std::cout << "Program Terminaited";
		return 1;
	}

	std::cout << "Valid Path";
	
	//Load file for playing

	
	
	
	
	//Play file & visualisation

}