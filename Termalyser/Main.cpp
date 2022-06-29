#include <iostream>

#include "MenuSystem.h"
#include "PlayAudio.h"

int main(int argc, char *argv[])
{
	// Check Correct Arguments Presented 
	if (argc < 2 || argc >2)
	{
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " 
			<< argc - 1 << " Arguments" << std::endl;
		return 1;
	}

	//Data Initialisation
	VisualiserSettings visSettings;
	visSettings.path = "";
	visSettings.path = argv[1];
	AudioData audioData;
	
	if (!CheckPathValid(visSettings.path))
	{
		std::cout << "Path provided did not contain a file" << std::endl;
		return 1;
	}

	//Display visualisation mode menu
	ShowMenu(&visSettings);
	
	std::string message;

	

	if (!PlayAudio(&visSettings.path, &message))
	{
		std::cout << "ERROR: " << message << std::endl;
		return 1;
	}
	

	//Load file for playing

	
	
	
	
	//Play file & visualisation

}