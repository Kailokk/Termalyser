#include <iostream>

#include "InputMenu.h"


int main(int argc, char** argv)
{
	
	//Checks for correct arguments
	switch (argc)
	{
	case (1):
			//create it with no extra field
			break;
	case(2):
		//Create menu with arg2
		break;

	default:
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed " << argc - 1 << " Arguments" << std::endl;
		return 1;
		break;
	}
	
	




	if (argc < 2)
	{
		//Create blank input menu
	}
	else
	{
		
	}

	//Create input menu with added file if arg provided



	
	//Load file for playing

	
	
	
	
	//Play file & visualisation

}