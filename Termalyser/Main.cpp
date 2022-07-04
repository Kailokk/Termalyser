#include <iostream>
#include <thread>
#include <chrono>

#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"  
#include "ftxui/screen/color.hpp"

#include "MenuSystem.h"
#include "PlayAudio.h"
#include "Visualisation.h"



int main(int argc, char* argv[])
{




	// Check Correct Arguments Presented
	if (argc > 2)
	{
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed "
			<< argc - 1 << " Arguments" << std::endl;
		return 1;
	}

	//Data Initialisation
	VisualiserSettings visSettings;
	AudioData audioData;

	//Checking if a path was provided, and adding if so
	if (argc > 1)
	{
		visSettings.path = argv[1];
	}
	else
	{
		visSettings.path = "";
	}
	
	//Display visualisation mode menu, exits if menu returns false
	if (!ShowMenu(&visSettings))
	{
		return 1;
	}

	//Error Message returned from audio player
	std::string message;

	//Pointer to the audio buffer, for the graphic to read
	float* buffer;

	ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::FitComponent();
	//visSettings.path = args;

	bool refresh_ui_continue = true;
	std::thread Audio_Thread([&screen, &visSettings, &message, &buffer, &refresh_ui_continue]
		{
			using namespace std::chrono_literals;
			//Play audio, exiting on error with specific message. Passes out a pointer for the audio buffer
			if (!PlayAudio(&visSettings.path, &message, &buffer))
			{
				refresh_ui_continue = false;
				auto func = screen.ExitLoopClosure();
				func();
				std::cout << "ERROR: " << message << std::endl;
				return;
			}
			refresh_ui_continue = false;
			std::this_thread::sleep_for(50ms);
			auto func = screen.ExitLoopClosure();
			screen.Clear();
			func();

			std::cout << "Audio Playback finished" << std::endl;
		});


	std::chrono::steady_clock::time_point lastFrame = (std::chrono::steady_clock::now());
	std::thread Graphics_Thread([&screen, &refresh_ui_continue, &lastFrame]
		{

			while (refresh_ui_continue)
			{
				std::chrono::steady_clock::time_point currentFrame = (std::chrono::steady_clock::now());
				float difference = ((float)std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - lastFrame).count()) / 1000.f;
				if (difference < (1.0f / 60.0f))
				{
					continue;
				}
				screen.PostEvent(ftxui::Event::Custom);
				lastFrame = (std::chrono::steady_clock::now());
			}
		});
	switch (visSettings.visMode)
	{
	case VISMODE_SCOPE:

		break;
	case VISMODE_PARTICLESCOPE:

		break;
	case VISMODE_SPECT:

		break;
	case VISMODE_TIMESPECT:

		break;
	default:
		break;
	}


	Audio_Thread.join();
	Graphics_Thread.join();
	std::cout << "Finished";
	std::cin.get();
	return 0;
}


