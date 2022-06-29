#include <iostream>

#include "MenuSystem.h"
#include <windows.h>

int main(int argc, char *argv[])
{
	//Instantiates a menu system
	VisualiserSettings visSettings;
	visSettings.path = "";
	AudioData audioData;



	// Check Correct Arguments Presented 
	if (argc < 2 || argc >2)
	{
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " 
			<< argc - 1 << " Arguments" << std::endl;
		return 1;
	}

	visSettings.path = argv[1];

	if (!CheckPathValid(visSettings.path))
	{
		std::cout << "Path provided did not contain a file" << std::endl;
		return 1;
	}

	ShowMenu(&visSettings);
	

	//Load file for playing

	
	
	
	
	//Play file & visualisation

}