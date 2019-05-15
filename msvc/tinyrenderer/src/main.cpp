#include "line.h"
#include "triangle.h"
#include "../thirdparty/objloader/OBJ_Loader.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

//draw line
void test_0()
{
	TGAImage image(200, 200, TGAImage::RGB);
	DrawLine(0, 0, 30, 60, red, image);
	BresenhamLine(0, 0, 40, 10, red, image);
	BresenhamLine(40, 70, 0, 0, red, image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../output.tga");
}

//draw triangle
void test_1()
{
	TGAImage image(200, 200, TGAImage::RGB);
	vec3 tri[] =
	{
		vec3(10, 10, 0),
		vec3(190, 160, 0),
		vec3(100, 30, 0),
	};

	DrawTriangleWithColor(tri, red, image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../output.tga");
}

//draw obj model with lighting
void test_2()
{
	TGAImage image(500, 500, TGAImage::RGB);	
	objl::Loader obj_loader;
	bool loadout = obj_loader.LoadFile("../../../obj/african_head/african_head.obj");
	if (!loadout)
		return;

	//找到mesh的最大和最小点，用于进行合适的缩放
	float max_x = -999999.9f, max_y = -999999.9f;
	float min_x = 999999.9f, min_y = 999999.9f;

	vec3 light_dir(0, 0, 1);
	
	for (int i = 0; i < obj_loader.LoadedMeshes.size(); ++i)
	{
		const objl::Mesh& curMesh = obj_loader.LoadedMeshes[i];
		for (int j = 0; i < curMesh.Vertices.size(); ++i)
		{
			max_x = std::max(max_x, curMesh.Vertices[i].Position.X);
			max_y = std::max(max_y, curMesh.Vertices[i].Position.Y);

			min_x = std::min(min_x, curMesh.Vertices[i].Position.X);
			min_y = std::min(min_y, curMesh.Vertices[i].Position.Y);
		}
	}

	float width_scale = max_x - min_x;
	float height_scale = max_y - min_y;

	int scale = int(500.0f / std::max(width_scale, height_scale));
	vec3 mid_point((max_x + min_x) * scale *0.5f, (max_y + min_y) * scale *0.5f, 0.0f);
	vec3 move_vec = (vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f) - mid_point) * 0.5f;

	vector<float> z_buffer(SCREEN_HEIGHT * SCREEN_WIDTH, FLT_MIN);

	for (int i = 0; i < obj_loader.LoadedMeshes.size(); ++i)
	{
		const objl::Mesh& curMesh = obj_loader.LoadedMeshes[i];	

		for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			objl::Vertex ver_0 = curMesh.Vertices[curMesh.Indices[j]];
			objl::Vertex ver_1 = curMesh.Vertices[curMesh.Indices[j + 1]];
			objl::Vertex ver_2 = curMesh.Vertices[curMesh.Indices[j + 2]];

			vec3 ivertex[3];
			ivertex[0] = vec3(ver_0.Position.X * scale, ver_0.Position.Y * scale, 0.0f) + move_vec;
			ivertex[1] = vec3(ver_1.Position.X * scale, ver_1.Position.Y * scale, 0.0f) + move_vec;
			ivertex[2] = vec3(ver_2.Position.X * scale, ver_2.Position.Y * scale, 0.0f) + move_vec;			

			//lighting
			vec3 edge_01(ver_1.Position.X - ver_0.Position.X, ver_1.Position.Y - ver_0.Position.Y, ver_1.Position.Z - ver_0.Position.Z);
			vec3 edge_02(ver_2.Position.X - ver_0.Position.X, ver_2.Position.Y - ver_0.Position.Y, ver_2.Position.Z - ver_0.Position.Z);

			vec3 normal = normalize(cross(edge_01, edge_02));
			float intensity = dot(normal, light_dir);

			if (intensity > 0)
			{
				TGAColor tri_color = TGAColor(int(/*rand() %*/ 256 * intensity),
					int(/*rand() %*/ 256 * intensity),
					int(/*rand() %*/ 256 * intensity),
					255);

			//	DrawTriangleWithColor(ivertex, tri_color, image);
				DrawTriangleWithZBuffer(ivertex, tri_color, z_buffer, image);
			}
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../output.tga");
}

int main(int argc, char** argv) 
{	
	test_2();

	return 0;
}

