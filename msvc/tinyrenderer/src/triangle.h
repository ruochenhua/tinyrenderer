#pragma once
#include "common.h"
using namespace std;
using namespace glm;

//calcualte bary centric coords
vec3 BaryCentric(vec3 t[3], const vec3& p)
{
	vec3 v0 = t[2] - t[0];
	vec3 v1 = t[1] - t[0];
	vec3 v2 = p - t[0];

	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11*dot02 - dot01*dot12)*invDenom;
	float v = (dot00*dot12 - dot01*dot02)*invDenom;
	
	return vec3(1.0f-u-v, u, v);
}

void inline DrawTriangleWithColor(vec3 t[3], const TGAColor& color, TGAImage& image)
{	
	//find bounding box
	ivec2 min, max;
	max = ivec2(-99999, -99999);
	min = ivec2(99999, 99999);
	for (int i = 0; i < 3; ++i)
	{
		if (t[i].x < min.x)
			min.x = int(floorf(t[i].x));
		if (t[i].y < min.y)
			min.y = int(floorf(t[i].y));

		if (t[i].x > max.x)
			max.x = int(ceilf(t[i].x));
		if (t[i].y > max.y)
			max.y = int(ceilf(t[i].y));
	}

	for (int x = min.x; x <= max.x; ++x)
	{
		for (int y = min.y; y <= max.y; ++y)
		{
 			vec3 p(x, y, 0);			

			vec3 bc_coord = BaryCentric(t, p);
			if (bc_coord.x > 0.0f && bc_coord.y > 0.0f && bc_coord.z > 0.0f)
			{
				image.set(x, y, color);
			}
		}
	}
}


void inline DrawTriangleWithZBuffer(vec3 t[3], const TGAColor& color, vector<float>& z_buffer, TGAImage& image)
{
	if (z_buffer.size() != SCREEN_HEIGHT * SCREEN_WIDTH)
		return;

	//find bounding box
	ivec2 min, max;
	max = ivec2(-99999, -99999);
	min = ivec2(99999, 99999);
	for (int i = 0; i < 3; ++i)
	{
		if (t[i].x < min.x)
			min.x = int(floorf(t[i].x));
		if (t[i].y < min.y)
			min.y = int(floorf(t[i].y));

		if (t[i].x > max.x)
			max.x = int(ceilf(t[i].x));
		if (t[i].y > max.y)
			max.y = int(ceilf(t[i].y));
	}

	for (int x = min.x; x <= max.x; ++x)
	{
		for (int y = min.y; y <= max.y; ++y)
		{
			vec3 p(x, y, 0);

			vec3 bc_coord = BaryCentric(t, p);	//u, v, 1-u-v
			if (bc_coord.x > 0.0f && bc_coord.y > 0.0f && bc_coord.z > 0.0f)
			{
				//barycentic coord represent point p's relationship to other vertices of the triangle
				//p = u * a + v * b + (1-u-v)*c -> p.z = u*(a.z) + v*(b.z) + (1-u-z)*(c.z)
				float p_z = bc_coord.x * t[0][2] + bc_coord.y * t[1][2] + bc_coord.z * t[2][2];
				if (p_z < z_buffer[x * SCREEN_WIDTH + y])
				{
					z_buffer[x * SCREEN_WIDTH + y] = p_z;
					image.set(x, y, color);
				}
					
			}
		}
	}
}