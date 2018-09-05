#pragma once

#include <glm/glm.hpp>

class CCamera
{
public:
	CCamera();
	~CCamera();

private:
	glm::vec3	mPos;

	float		mYaw;
	float		mPitch;
	float		mFOV;
};