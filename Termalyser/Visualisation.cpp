#include "Visualisation.h"
#include "PlayAudio.h"

#include <chrono>
#include <vector>

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

struct coord
{
	int xAxis = 0;
	int yAxis = 0;
};

coord previous;

ftxui::Component Oscilloscope(float**& bufferPointer)
{

	return Renderer([&]
		{


			auto my_Canvas = canvas([&](Canvas& c)
				{
					//c.DrawBlockLine(8, c.height() / 2, c.width()-8, c.height() / 2);
					float spacing = (float)c.width()*4 /FRAMES_PER_BUFFER;
					float reduction = (float)FRAMES_PER_BUFFER / (c.width());
					
					
				//	c.DrawText(0, 20, std::to_string(c.width()));
					
					if (FRAMES_PER_BUFFER > c.width())
					{
						c.DrawText(0, 0, "Samples Bigger Than Width divided byh two");
						//if the possible coords is smaller than the samples
						//code that only checks a certain volume of samples 
					/*	float num = FRAMES_PER_BUFFER / c.width();
						
						int prevX = 0;
						for (int x = 0; x < FRAMES_PER_BUFFER; x += num)
						{
							float leftChn = (*bufferPointer)[x] * 20;
							float rightChn = (*bufferPointer)[x + 1] * 20;
							int comb = (int)(leftChn + rightChn) + 50;
							c.DrawPointLine(x, prev, x, comb);
							prev = comb;
						}
						*/
					}
					else if (FRAMES_PER_BUFFER < c.width())
					{
						int num = (int)((c.width() - FRAMES_PER_BUFFER) / FRAMES_PER_BUFFER);
						c.DrawText(0, 0, "Samples smaller Than Width divided by two");
						
						float windowSize = FRAMES_PER_BUFFER / c.width();

						auto in_to_out_frac = 1.0 / windowSize;



						//if the possible coords is bigger than the buffer size
						//code that spreads the samples wider than 1
					
						c.DrawText(0, 10, std::to_string(num));

						int prevX = 0;
						for (int x = 0; x < FRAMES_PER_BUFFER; x++)
						{
							float leftChn = (*bufferPointer)[x] * 20;
							float rightChn = (*bufferPointer)[x + 1] * 20;
							int comb = (int)(leftChn + rightChn) + 50;

							if (x < 1)
							{
								c.DrawPointLine(previous.xAxis, previous.yAxis, x, comb);
							}
							else
							{
								c.DrawPointLine(previous.xAxis, previous.yAxis, x + num, comb);
							}
							prev = comb;
							previous.xAxis = x;
							previous.yAxis = comb;
						}
					}
					else
					{
						
						c.DrawText(0, 0, "Samples perfect size");
						//code that writes the samples linearly
						/*int prevX = 0;
						for (int x = 0; x < FRAMES_PER_BUFFER; x++)
						{
							float leftChn = (*bufferPointer)[x] * 20;
							float rightChn = (*bufferPointer)[x + 1] * 20;
							int comb = (int)(leftChn + rightChn) + 50;
							c.DrawPointLine(previous.xAxis, previous.yAxis, x, comb);
							previous.xAxis = x;
							previous.yAxis = comb;
						}*/
					}
				});



			return my_Canvas | flex;

		});
}
/*
auto canv = Renderer([&]
	{
		auto c = Canvas(Terminal::Size().dimx, 100);
		int spacing = FRAMES_PER_BUFFER / c.width();
		for (int x = 0; x < FRAMES_PER_BUFFER; x++)
		{
			float leftChn = (*bufferPointer)[x] * 20;
			float rightChn = (*bufferPointer)[x + 1] * 20;
			int comb = (int)(leftChn + rightChn) + 50;

			if (x < FRAMES_PER_BUFFER)
			{
				c.DrawPointLine(x - spacing, comb, x - 1, prev);
				prev = comb;
			}
		}
		return canvas(std::move(c)) | flex;
	});

return hbox(
	{

		canv->Render()
	}) | flex;

})| border;
*/


