#include "Visualisation.h"
#include "PlayAudio.h"

#include <chrono>

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"  
#include "ftxui/screen/color.hpp"
using namespace ftxui;

std::chrono::steady_clock::time_point start = (std::chrono::steady_clock::now());

ftxui::Component Oscilloscope(float*** buffer, bool& clearScreen)
{
	return Renderer([&clearScreen, &buffer]
		{
			auto c = Canvas(FRAMES_PER_BUFFER, 100);

			//	std::string name = std::to_string(**buffer[0]);
			//	c.DrawText(0, 0, name);
				//std::cout << *buffer << std::endl;
			for (int x = 0; x < FRAMES_PER_BUFFER; x++)
			{
				if(x < FRAMES_PER_BUFFER)
				{
					//c.DrawBlock(x, 50, true);
					//c.DrawPointOn(x, 50);
					if (buffer != nullptr)
					{
						float left = **buffer[x];
						float right = **buffer[x + 1];
						int comb = (int)((left + right) * 10);

						c.DrawPointOn(x, ((int)(comb * 10)) + 50);
						//c.DrawPointOn(x, (int)(&buffer)[x] * -10);
					}
					else
					{
						std::cout << "Nullptr2" << std::endl;
					}
				}
				x += 1;
			}
			return canvas(std::move(c));
		});
}

ftxui::Component TestComponent(bool& clearScreen)
{
	return Renderer([&clearScreen]
		{

			if (clearScreen)
			{

			}
			else
			{
				struct Line
				{
					float x1;
					float y1;
				};

				std::vector<std::vector<int>> time(1, std::vector<int>(20));

				auto c = Canvas(1440, 1080);

				c.DrawText(0, 0, "A symmetrical graph filled");

				std::chrono::steady_clock::time_point stop = (std::chrono::steady_clock::now());
				float difference = ((float)std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()) / 1000.f;

				c.DrawPointOn(sin(difference), 50);

				return canvas(std::move(c));
			}
		});


}