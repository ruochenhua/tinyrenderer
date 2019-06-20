#include "line.h"
#include "triangle.h"
#include "camera.h"
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

//draw obj model with camera
void test_3()
{
	CCamera cam(vec3(-1, -1, 1), vec3(-1, -1, 0), vec3(0, 1, 0));
	mat4 model_view = cam.LookAt();
	mat4 projection = cam.Projection(3.f, 300.f, 90.f, 0.0f);
	mat4 view_port = cam.ViewPort();
	mat4 MVP = (view_port * projection * model_view);	//glm::mat use coloum-major

	TGAImage image(500, 500, TGAImage::RGB);
	objl::Loader obj_loader;
	bool loadout = obj_loader.LoadFile("../../../obj/african_head/african_head.obj");
	if (!loadout)
		return;

	vec3 light_dir(0, 0, 1);

	vector<float> z_buffer(SCREEN_HEIGHT * SCREEN_WIDTH, FLT_MIN);

	for (int i = 0; i < obj_loader.LoadedMeshes.size(); ++i)
	{
		const objl::Mesh& curMesh = obj_loader.LoadedMeshes[i];

		for (int j = 0; j < curMesh.Indices.size(); j += 3)
		{
			objl::Vertex ver_0 = curMesh.Vertices[curMesh.Indices[j]];
			objl::Vertex ver_1 = curMesh.Vertices[curMesh.Indices[j + 1]];
			objl::Vertex ver_2 = curMesh.Vertices[curMesh.Indices[j + 2]];

			vec4 ivertex[3];
			ivertex[0] = MVP * vec4(ver_0.Position.X, ver_0.Position.Y, ver_0.Position.Z, 1.0f);
			ivertex[1] = MVP * vec4(ver_1.Position.X, ver_1.Position.Y, ver_1.Position.Z, 1.0f);
			ivertex[2] = MVP * vec4(ver_2.Position.X, ver_2.Position.Y, ver_2.Position.Z, 1.0f);

			
			//lighting
			//calculate normal
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

				//DrawTriangleWithColor(ivertex, tri_color, image);
				DrawTriangleWithZBuffer(ivertex, tri_color, z_buffer, image);
			}
		}
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("../output.tga");
}


int main(int argc, char** argv) 
{	
	test_3();

	return 0;
}

