#include "GraphicsSystem.h"
#include "PlayAudio.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

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
		visSettings.path = "C:\\Users\\kailo\\OneDrive\\Desktop\\Desktop\\Mix.wav";
	}

	//Display visualisation mode menu, exits if menu returns false
	if (!ShowMenu(&visSettings))
	{
		return 1;
	}

	//Error Message returned from audio player
	std::string message;

	//Pointer to the audio buffer, for the graphic to read
	float bufferStartingVal[FRAMES_PER_BUFFER];
	float* initBufferPointer = &(bufferStartingVal[0]);
	initBufferPointer[FRAMES_PER_BUFFER - 1] = 1;
	float** bufferPointer = nullptr;

	//Screen object which loops an ftxui component
	ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
	bool refresh_ui_continue = true;

	std::thread Audio_Thread([&screen, &visSettings, &message, &bufferPointer, &refresh_ui_continue]
		{
			//Play audio, exiting on error with specific message. Passes out a pointer for the audio buffer
			if (!PlayAudio(&visSettings.path, &message, &bufferPointer))
			{
				refresh_ui_continue = false;
				auto func = screen.ExitLoopClosure();
				func();
				std::cout << "ERROR: " << message << std::endl;
				return;
			}
			refresh_ui_continue = false;
			auto exit = screen.ExitLoopClosure();
			exit();
		});

	std::thread Graphics_Thread([&bufferPointer, &screen, &refresh_ui_continue]
		{
			while (bufferPointer == nullptr)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(1ns);
			}
			screen.Loop(Oscilloscope(bufferPointer, refresh_ui_continue));
		});
	
	//Loop which refreshes screen continuously at a provided fps
	std::chrono::steady_clock::time_point lastFrame = (std::chrono::steady_clock::now());
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

	Audio_Thread.join();
	Graphics_Thread.join();
	return 0;
}