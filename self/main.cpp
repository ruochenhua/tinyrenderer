#include "tgaimage.h"
#include <algorithm>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

using namespace std;

void DrawLine(int x0, int y0, int x1, int y1, const TGAColor& color, TGAImage& image)
{
	int step = max(abs(x0 - x1), abs(y0 - y1));
	for(int i = 0; i < step; ++i)
	{
		int x = x0 + (x1 - x0) / (float)step * i;
		int y = y0 + (y1 - y0) / (float)step * i;
//		printf("x %d, y %d\n", x, y);
		image.set(x, y, color);
	}
}

int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);

	DrawLine(50, 40, 89, 21, red, image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

