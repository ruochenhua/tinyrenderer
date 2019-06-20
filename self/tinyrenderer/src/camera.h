#pragma once
#include "common.h"

class CCamera
{
public:
	CCamera(const glm::vec3& center, const glm::vec3& eye, const glm::vec3& up);

	glm::mat4 LookAt() const;
	glm::mat4 Projection(float near, float far, float fov, float aspect_ratio) const;
	glm::mat4 ViewPort() const;
private:
	glm::vec3 m_Center;
	glm::vec3 m_Eye;
	glm::vec3 m_Up;
};