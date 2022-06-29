#include <iostream>

#include "MenuSystem.h"
#include <windows.h>

int main(int argc, char *argv[])
{
	//Instantiates a menu system
	VisualiserSettings visSettings;
	visSettings.path = "";

	// Check Correct Arguments Presented 
	if (argc < 2 || argc >2)
	{
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " 
			<< argc - 1 << " Arguments" << std::endl;
		return 1;
	}


	//Create input menu with added file if arg provided
	while (CheckPathValid(&visSettings))
	{
		ShowMenu(&visSettings);
		Sleep(1000);
		std::cout << "Its Working";
		Sleep(5000);
	}

	
	


	//Load file for playing

	
	
	
	
	//Play file & visualisation

}