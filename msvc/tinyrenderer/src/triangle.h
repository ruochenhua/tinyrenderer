#pragma once
#include "common.h"
using namespace std;
using namespace glm;

void inline DrawTriangle(ivec2 t[3], const TGAColor& color, TGAImage& image)
{
	//find bounding box
	ivec2 min, max;
	max = ivec2(-99999, -99999);
	min = ivec2(99999, 99999);
	for (int i = 0; i < 3; ++i)
	{
		if (t[i].x < min.x)
			min.x = t[i].x;
		if (t[i].y < min.y)
			min.y = t[i].y;

		if (t[i].x > max.x)
			max.x = t[i].x;
		if (t[i].y > max.y)
			max.y = t[i].y;
	}

	//0: a, 1: b, 2: c
	vec3 a(t[0].x, t[0].y, 0.0f);
	vec3 b(t[1].x, t[1].y, 0.0f);
	vec3 c(t[2].x, t[2].y, 0.0f);

	vec3 ab = b - a;
	vec3 bc = c - b;
	vec3 ca = a - c;

	for (int x = min.x; x <= max.x; ++x)
	{
		for (int y = min.y; y <= max.y; ++y)
		{
			vec3 p(x, y, 0);
			vec3 c1 = cross(p-a, ab);
			float sign_0 = c1.z;
			float sign_1 = cross(p-b, bc).z;
			float sign_2 = cross(p-c, ca).z;			

			if ((sign_0 >= 0.0f && sign_1 >= 0.0f && sign_2 >= 0.0f)
				|| (sign_0 <= 0.0f && sign_1 <= 0.0f && sign_2 <= 0.0f))
			{
				image.set(x, y, color);
			}
		}
	}
}