#include "Visualisation.h"
#include "PlayAudio.h"

#include <chrono>

#include "ftxui/component/component.hpp"  // for Renderer, CatchEvent, Horizontal, Menu, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/canvas.hpp"  
#include "ftxui/screen/color.hpp"

using namespace ftxui;

std::chrono::steady_clock::time_point start = (std::chrono::steady_clock::now());

ftxui::Component Oscilloscope(float**& bufferPointer)
{

	return Renderer([&]
		{
			auto canv = Renderer([&]
				{
					auto c = Canvas(Terminal::Size().dimx, (Terminal::Size().dimy) * 4);


					c.DrawText(0, 0, std::to_string(Terminal::Size().dimy));
					for (int x = 0; x < FRAMES_PER_BUFFER; x++)
					{
						float leftChn = (*bufferPointer)[x] * Terminal::Size().dimy * 4;
						float rightChn = (*bufferPointer)[x + 1] * Terminal::Size().dimy * 4;

						int comb = (int)(leftChn + rightChn) + 50;

						c.DrawBlockLine(x, c.height() / 2, x, +comb);
						x += 1;
					}
					return canvas(std::move(c));
				});

			return hbox(
				{
					canv->Render()
				}) | border;


		});
}

ftxui::Component TestComponent()
{

	return Renderer([&]
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
		});

}