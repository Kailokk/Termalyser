#include "GraphicsSystem.h"

#include <filesystem>

#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

Component Wrap(std::string name, Component component)
{
	return Renderer(component, [name, component]
		{
			return hbox({
					   text(name) | size(WIDTH, EQUAL, 20),
					   separator(),
					   component->Render() | xflex,
				}) | xflex;
		});
}

bool ShowMenu(VisualiserSettings* settings)
{
	bool continueProgram = false;
	//Creates Screen
	ScreenInteractive screen = ScreenInteractive::FitComponent();
	//The output renderer
	Component output;
	//bool to close the screen
	bool clearScreen = false;
	//String at top of menu
	std::string titleBar = "Please select your options followed by pressing the confirm button.";
	//function called when pressing enter on the input or the confirm button, clears the screen if the path is valid
	std::function<void()> Confirmation = [&screen, &settings, &titleBar, &output, &clearScreen, &continueProgram]
	{
		if (CheckPathValid(settings->path))
		{
			clearScreen = true;
			continueProgram = true;
		}
		else
		{
			titleBar = "Please Enter A Valid Path!";
		}
	};
	//Function called when pressing the quit button, clears screen and sets the retun falue to flase
	std::function<void()> Quit = [&clearScreen, &continueProgram]
	{
		clearScreen = true;
		continueProgram = false;
	};

	//Renders Title
	Component title = Renderer([&]
		{
			return hbox({
					   text(titleBar) | size(WIDTH, EQUAL, 80),
				}) |
				xflex;
		});


	//Radiobox for visualisation option
	int visualisation_selected = 0;
	std::vector<std::string> visualisation_entries = {
		"Oscilloscope",
		"Frequency Response",
		"3D Frequency Plot",
		"Particle Oscilloscope",
	};
	Component radiobox = Radiobox(&visualisation_entries, &visualisation_selected);
	radiobox = Wrap("Visualisation", radiobox);

	//Input bar, which outputs to the pointer to the file path. additional option to allow enter for confirm
	InputOption inputOption;
	inputOption.on_enter = Confirmation;
	Component input = Input(settings->path, "File Path", inputOption);
	input = Wrap("FilePath", input);

	//Confirm button, continues the program if the file path is valid
	Component confirm = Button("Confirm", Confirmation);
	//Quit button, exits program 
	Component quit = Button("Quit", Quit);

	//Wraps the buttons into one section
	Component buttons = Container::Horizontal({
		quit, confirm,
		});

	//Wraps all sections into one component
	Component layout = Container::Vertical({
		 title,
		 radiobox,
		 input,
		 buttons
		});

	//Checks if the screen should clear, and if not renders the menu
	output = Renderer(layout, [&]
		{
			if (clearScreen)
			{
				return vbox({});
			}
			else
			{
				return vbox({
							title->Render(),
							separator(),
							radiobox->Render(),
							separator(),
							input->Render(),
							separator(),
							buttons->Render(),
					}) |
					xflex | size(WIDTH, GREATER_THAN, 100) | border;
			}
		});

	//Thread with a loop to check if the menu is closing, and if so waits a ms for the graphic loop below to update
	bool CheckForEnd = true;
	std::thread LoopEnd_Thread([&screen, &clearScreen, &CheckForEnd]
		{
			while (CheckForEnd)
			{
				if (clearScreen)
				{
					using namespace std::chrono_literals;
					std::this_thread::sleep_for(1ms);
					CheckForEnd = false;
					std::function<void()> close = screen.ExitLoopClosure();
					close();
				}
			}
		});
	//Loops on the menu component
	screen.Loop(output);
	LoopEnd_Thread.join();
	return continueProgram;
}

//Prelimenary check on provided filepath
bool CheckPathValid(std::string& path)
{
	if (std::filesystem::exists(path))
	{
		if (path.find(".") != std::string::npos)
		{
			return true;
		}
	}
	return false;
}


//Visualisations -------------------------------------------------------------


//Block Line Oscilloscope
ftxui::Component Oscilloscope(float**& bufferPointer, bool& showVisualisation)
{
	return Renderer([&]
		{
			if (!showVisualisation)
			{
				return vbox({});
			}
			else
			{
				auto my_Canvas = canvas([&](Canvas& c)
					{
						int canvasHeight = c.height();
						int halfCanvasHeight = canvasHeight / 2;
						int canvasWidth = c.width();
						const int stereoFrames = (FRAMES_PER_BUFFER / 2);

						c.DrawText(0, 0,std::to_string(canvasWidth));

						int stereoCombination[stereoFrames];
						int doubleIterator = 0;
						for (int i = 0; i < stereoFrames; i++)
						{
							stereoCombination[i] = (int)(((*bufferPointer)[doubleIterator] + (*bufferPointer)[doubleIterator + 1]) * halfCanvasHeight);
							doubleIterator += 2;
						}

						int multiplyer = FRAMES_PER_BUFFER / canvasWidth;

						auto SampleBuffer = [&](int x_not_scaled)
						{
							float x = x_not_scaled * FRAMES_PER_BUFFER / canvasWidth;
							int x1 = std::floor(x);
							int x2 = std::min(x1 + 1, stereoFrames);
							float y1 = stereoCombination[x1];
							float y2 = stereoCombination[x2];
						//	std::cout << out << std::endl;
							return static_cast<int>((x - x1) * y2 + (x2 - x) * y1) + halfCanvasHeight;
						};



						//Previous value to draw a line from
						int previousY = SampleBuffer(0);
						//draws lines along the center of the x axis, offsetting the y based on the buffers current contents
						for (int x = 1; x < canvasWidth - 1; x++)
						{
							float nextY = SampleBuffer(x);
						//	std::cout << "Coords: \n" << x - 1 << ","  << previousY << "\n" << x << "," << nextY << std::endl;
						//	c.DrawPointOn(x,nextY);
						//	c.DrawText(x,nextY);
							c.DrawBlockLine(x - 1, previousY, x, nextY);
							previousY = nextY;
						}
						
					});
				return my_Canvas | flex;
			}
		});
}


ftxui::Component ParticleOscilloscope(float**& bufferPointer, bool& showVisualisation)
{
	return Renderer([&]
		{
			if (!showVisualisation)
			{
				return vbox({});
			}
			else
			{
				auto my_Canvas = canvas([&](Canvas& c)
					{
						//Exits early if there is no screen visible
						if (c.width() == 0)
						{
							return;
						}

						//Linear Interpolation
						auto SampleBuffer = [&](int x_not_scaled)
						{
							float x = x_not_scaled * FRAMES_PER_BUFFER / c.width();
							int x1 = std::floor(x);
							int x2 = std::min(x1 + 1, FRAMES_PER_BUFFER - 1);
							float y1 = (*bufferPointer)[x1] * (c.height());
							float y2 = (*bufferPointer)[x2] * (c.height());
							return static_cast<int>((x - x1) * y2 + (x2 - x) * y1) + (c.height() / 2);
						};

						//Previous value to draw a line from
						int previousY = SampleBuffer(0);
						//draws lines along the center of the x axis, offsetting the y based on the buffers current contents
						for (int x = 1; x < c.width() - 1; x++)
						{
							float nextY = SampleBuffer(x);
							c.DrawBlockLine(x - 1, previousY, x, nextY);
							previousY = nextY;
						}
					});
				return my_Canvas | flex;
			}
		});
}
