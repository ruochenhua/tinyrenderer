#include "camera.h"
#include <math.h>
const float PI = 3.1415926f;

using namespace glm;

CCamera::CCamera(const glm::vec3& center, const glm::vec3& eye, const glm::vec3& up)
	: m_Center(center), m_Eye(eye), m_Up(up)
{

}

mat4 CCamera::LookAt() const
{
	vec3 z = normalize(m_Eye - m_Center);	//z轴视线（前）方向
	vec3 x = normalize(cross(m_Up, z));		//x轴左方向
	vec3 y = normalize(cross(z, x));		//y轴上方向

	mat4 model_view = identity<mat4>();
	mat4 trans = identity<mat4>();

	for (int i = 0; i < 3; ++i)
	{
		model_view[0][i] = x[i];
		model_view[1][i] = y[i];
		model_view[2][i] = z[i];

		trans[i][3] = -m_Center[i];
	}

	model_view = transpose(model_view * trans);
	return model_view;
}

mat4 CCamera::Projection(float near, float far, float fov, float aspect_ratio) const
{
	mat4 projection = identity<mat4>();

	float t = -(far / (far - near));

	float s = 1.0f / (tan(fov * 0.5f * PI / 180.0f));
	projection[0][0] = s;
	projection[1][1] = s;

	projection[2][2] = t;
	projection[2][3] = -1;
	projection[3][3] = t * near;

	return projection;
}

//map (-1, 1) to (0, screen_width/height)
mat4 CCamera::ViewPort() const
{
	mat4 view_port = identity<mat4>();

	view_port[0][0] = SCREEN_WIDTH / 2;
	view_port[1][1] = SCREEN_HEIGHT / 2;


	view_port[2][2] = 0.5f;
	
	return view_port;
}