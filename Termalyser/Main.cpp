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
	/*
	// Check Correct Arguments Presented
	if (argc > 2)
	{
		std::cout << "Incorrect amount of arguments provided \nPlease pass nothing, or a path to an audio file \nYou passed "
			<< argc - 1 << " Arguments" << std::endl;
		return 1;
	}
	*/
	//Data Initialisation
	VisualiserSettings visSettings;
	AudioData audioData;
	/*
	//Checking if a path was provided, and adding if so
	if (argc > 1)
	{
		visSettings.path = argv[1];
	}
	else
	{
		visSettings.path = "";
	}
	*/

	visSettings.path = "D:\\4 Personal\\Programming\\Personal_Projects\\Termalyser\\x64\\Debug\\mix.wav";
	//Display visualisation mode menu, exits if menu returns false
	if (!ShowMenu(&visSettings))
	{
		return 1;
	}

	//Error Message returned from audio player
	std::string message;

	//Pointer to the audio buffer, for the graphic to read
	float* buffer = nullptr;
	float** bufferPtr;
	float*** bufferPtrPtr =  (float***)malloc(sizeof(float**));
	bufferPtr = &buffer;


	ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::FitComponent();

	bool close_Display = false;
	std::thread Audio_Thread([&screen, &visSettings, &message, &bufferPtr, &close_Display, &bufferPtrPtr]
		{
			using namespace std::chrono_literals;
			std::function<void()> CloseScreen = screen.ExitLoopClosure();
			//Play audio, exiting on error with specific message. Passes out a pointer for the audio buffer
			if (!PlayAudio(&visSettings.path, &message, bufferPtrPtr))
			{
				close_Display = false;
				CloseScreen();
				std::cout << "ERROR: " << message << std::endl;
				return;
			}
			close_Display = false;
			std::this_thread::sleep_for(1ms);
			CloseScreen();
			std::cout << "Audio Playback finished" << std::endl;
		});

	using namespace std::chrono;
	steady_clock::time_point lastFrame = (steady_clock::now());
	std::thread Graphics_Thread([&screen, &close_Display, &lastFrame, &buffer, &bufferPtrPtr, &bufferPtr]
		{
			std::this_thread::sleep_for(500ms);
				buffer = *bufferPtr;
			//	while (*bufferPtr == nullptr)
				//{
			//		std::cout << *bufferPtr << std::endl;
				//}
				//std::cout << *bufferPtr << std::endl;

				screen.Loop(Oscilloscope(bufferPtrPtr, close_Display));
		});

	while (!close_Display)
	{
		steady_clock::time_point currentFrame = (steady_clock::now());
		float difference = ((float)duration_cast<std::chrono::milliseconds>(currentFrame - lastFrame).count()) / 1000.f;
		if (difference < (1.0f / 60.0f))
		{
			continue;
		}
		screen.PostEvent(ftxui::Event::Custom);
		if (buffer != nullptr)
		{
			for (int i = 0; i < FRAMES_PER_BUFFER; i++)
			{
		//		std::cout << buffer[i] << " , " << buffer[i + 1] << std::endl;
				i += 1;
			}
			
		}
		lastFrame = (steady_clock::now());
	}

//	screen.Loop(TestComponent(close_Display));
	
/*
	switch (visSettings.visMode)
	{
	case VISMODE_SCOPE:
		screen.Loop(TestComponent(close_Display));
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
	*/

	Audio_Thread.join();
	Graphics_Thread.join();
	std::cout << "Finished";
	std::cin.get();
	return 0;
}


