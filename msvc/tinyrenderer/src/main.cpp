#include "line.h"
#include "triangle.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);



int main(int argc, char** argv) {
	TGAImage image(200, 200, TGAImage::RGB);

	//DrawLine(0, 0, 30, 60, red, image);
	//BresenhamLine(0, 0, 40, 10, red, image);
	//BresenhamLine(40, 70, 0, 0, red, image);

	glm::ivec2 tri[] = 
	{ 
		glm::ivec2(10, 10), 
		glm::ivec2(190, 160),
		glm::ivec2(100, 30),
	};

	DrawTriangle(tri, red, image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../output.tga");
	return 0;
}

