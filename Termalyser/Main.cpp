#include <iostream>

#include "MenuSystem.h"


int main(int argc, char *argv[])
{
	//Instantiates a menu system
	VisualiserSettings visSettings;
	visSettings.path = "";

	//Checks for correct arguments, and attaches provided path if given
	switch (argc)
	{
	case (1):
			break;
	case(2):
		visSettings.path = argv[1];
		break;

	default:
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " << argc - 1 << " Arguments" << std::endl;
		return 1;
		break;
	}


	//Create input menu with added file if arg provided

	while (CheckPathValid(&visSettings))
	{
		ShowMenu(&visSettings);
	}

	
	


	//Load file for playing

	
	
	
	
	//Play file & visualisation

}