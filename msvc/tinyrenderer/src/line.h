#pragma once
#include <algorithm>
#include "tgaimage.h"
using namespace std;

//avoid floating point multiplication and addition, thus should be faster
void inline BresenhamLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image)
{
	//from smaller number to larger number
	if (x0 > x1) swap(x0, x1);
	if (y0 > y1) swap(y0, y1);

	bool steep = false;	//if steep, x and y will be swapped
	if ((x1 - x0) < (y1 - y0))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}
	//avoid floating point number operation, slope/error multiple by (x1 - x0)*2
	//float slope = float(y1 - y0) / float(x1 - x0);
	int slope = (y1 - y0) * 2;
	//float error = 0.0f;
	int error = 0;
	for (int x = x0, y = y0; x < x1; ++x)
	{
		error += slope;
		//if(error >= 0.5f)
		if (error >= (x1 - x0))
		{
			++y;
			error -= (x1 - x0) * 2;
		}

		if (!steep)
		{
			image.set(x, y, color);
			//printf("x %d, y %d\n", x, y);
		}
		else
		{
			image.set(y, x, color);
			//printf("steep x %d, y %d\n", y, x);
		}
	}
}

void inline DrawLine(int x0, int y0, int x1, int y1, const TGAColor & color, TGAImage & image)
{
	int step = max(abs(x0 - x1), abs(y0 - y1));
	for (int i = 0; i < step; ++i)
	{
		int x = x0 + (x1 - x0) / (float)step * i;
		int y = y0 + (y1 - y0) / (float)step * i;
		//		printf("x %d, y %d\n", x, y);
		image.set(x, y, color);
	}
}