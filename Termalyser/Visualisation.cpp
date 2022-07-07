#include "Visualisation.h"
#include "PlayAudio.h"
using namespace ftxui;

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
						//Exits early if there is no screen visible
						if (c.width() == 0)
						{
							return;
						}

						//Linear Interpolation
						auto sample_y = [&](int x_not_scaled)
						{
							float x = x_not_scaled * FRAMES_PER_BUFFER / c.width();
							int x1 = std::floor(x);
							int x2 = std::min(x1 + 1, FRAMES_PER_BUFFER - 1);
							float y1 = (*bufferPointer)[x1] * (c.height());
							float y2 = (*bufferPointer)[x2] * (c.height());
							return static_cast<int>((x - x1) * y2 + (x2 - x) * y1) + (c.height() / 2);
						};

						//Previous value to draw a line from
						int previousY = sample_y(0);
						//draws lines along the center of the x axis, offsetting the y based on the buffers current contents
						for (int x = 1; x < c.width() - 1; x++)
						{
							float nextY = sample_y(x);
							c.DrawBlockLine(x - 1, previousY, x, nextY);
							previousY = nextY;
						}
					});
				return my_Canvas | flex;
			}
		});
}
