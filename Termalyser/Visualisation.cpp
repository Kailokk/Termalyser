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

ftxui::Component Oscilloscope(float**& bufferPointer)
{
	return Renderer([&]
		{
			auto my_Canvas = canvas([&](Canvas& c)
				{
					if (c.width() == 0)
						return;

					auto sample_y = [&](int x_not_scaled)
					{
						float x = x_not_scaled * FRAMES_PER_BUFFER / c.width();
						int x1 = std::floor(x);
						int x2 = std::min(x1 + 1, FRAMES_PER_BUFFER - 1);
						float y1 = (*bufferPointer)[x1] * (c.height());
						float y2 = (*bufferPointer)[x2] * (c.height());
						return static_cast<int>((x - x1) * y2 + (x2 - x) * y1) + (c.height() / 2);
					};

					int previousY = sample_y(0);
					for (int x = 1; x < c.width() - 1; x++)
					{
						float nextY = sample_y(x);
						c.DrawBlockLine(x - 1, previousY, x, nextY);
						previousY = nextY;
					}
				});
			return my_Canvas | flex;
		});
}











































/*
template <std::size_t OUT, std::size_t IN>
std::array<float, OUT> sampling(const std::array<float, IN>& a)
{
	std::array<float, OUT> res;

	for (int i = 0; i != OUT - 1; ++i)
	{
		int index = i * (IN - 1) / (OUT - 1);
		float p = i * (IN - 1) % (OUT - 1);

		res[i] = ((p * a[index + 1]) + (((OUT - 1) - p) * a[index])) / (OUT - 1);
	}
	res[OUT - 1] = a[IN - 1]; // done outside of loop to avoid out of bound access (0 * a[IN])
	return res;
}



ftxui::Component Oscilloscope(float**& bufferPointer)
{

	return Renderer([&]
		{
			auto my_Canvas = canvas([&](Canvas& c)
				{
					const int screen = c.width();
					//c.DrawBlockLine(8, c.height() / 2, c.width()-8, c.height() / 2);
					//float spacing = (float)c.width()*4 /FRAMES_PER_BUFFER;
					//float reduction = (float)FRAMES_PER_BUFFER / (c.width());
					if (FRAMES_PER_BUFFER == c.width())
					{
						c.DrawText(0, 0, "Samples perfect size");
						//code that writes the samples linearly
						for (int x = 0; x < FRAMES_PER_BUFFER; x++)
						{
							float leftChn = (*bufferPointer)[x] * 20;
							float rightChn = (*bufferPointer)[x + 1] * 20;
							int comb = (int)(leftChn + rightChn) + 50;
							c.DrawPointLine(x - 1, previous.yAxis, x, comb);
							previous.yAxis = comb;
						}
					}
					else
					{
						c.DrawText(0, 0, "Samples imperfect size");
						std::vector<float> buffer(FRAMES_PER_BUFFER);
						for (int i = 0; i < FRAMES_PER_BUFFER; i++)
						{
							float leftChn = (*bufferPointer)[i] * 20;
							float rightChn = (*bufferPointer)[i + 1] * 20;
							buffer[i] = (int)(leftChn + rightChn) + 50;
						}

						std::vector<float> Output = ReSample(buffer, FRAMES_PER_BUFFER, c.width());
				
						for (int x = 0; x < c.width(); x++)
						{
							std::cout << std::to_string(Output.at(x)) << std::endl;
							c.DrawPointLine(x - 1, previous.yAxis, x, Output.at(x));
							previous.yAxis = Output.at(x);
						}
						//std::vector<float> outValues;
					}



				});
			return my_Canvas | flex;
		});
}
/*
* 
* std::vector<float> ReSample(std::vector<float> in, std::size_t outSize, std::size_t inSize)
{
	std::vector<float> output(outSize);

	for (int i = 0; i != outSize - 1; ++i)
	{
		int index = i * (inSize - 1) / (outSize - 1);
		float p = i * (inSize - 1) % (outSize - 1);

		output[i] = ((p * in[index + 1]) + (((outSize - 1) - p) * in[index])) / (outSize - 1);
	}
	output[outSize - 1] = in[inSize - 1]; // done outside of loop to avoid out of bound access (0 * a[IN])
	return output;
}
* 
* 
* 
* 
* 
* 
* 
* 
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


