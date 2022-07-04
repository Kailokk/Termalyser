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

int prev = 0;

ftxui::Component Oscilloscope(float**& bufferPointer)
{

	return Renderer([&]
		{
			auto canv = Renderer([&]
				{
					auto c = Canvas(FRAMES_PER_BUFFER, 100);


					int spacing = c.width() / 2;

					c.DrawText(0, 0, std::to_string(spacing));
					

					for (int x = 0; x < FRAMES_PER_BUFFER; x++)
					{
						float leftChn = (*bufferPointer)[x] * 20;
						float rightChn = (*bufferPointer)[x + 1] * 20;
						int comb = (int)(leftChn + rightChn) + 50;

						

						if (x < FRAMES_PER_BUFFER)
						{
							c.DrawPointLine(x, comb, x -1, prev);
							prev = comb;
						}
					}
					return canvas(std::move(c));
				});

			return flexbox(
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