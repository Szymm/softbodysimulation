#pragma once

#include <glm/glm.hpp>
#include <vector>

struct SMassPoint
{
	glm::vec3	pos;
	glm::vec3	vel;
	glm::vec3	f;
	float		mass;
	bool anchor;
};

struct SSpring
{
	SMassPoint* pPoints[2];
	float ks, l;
};

class CPhysicsSystem
{
public:
	CPhysicsSystem();
	~CPhysicsSystem();

	void CreateRope(float segmentLength, int segmentCount);

	std::vector<glm::vec3>& GetMassPointPositions() { return mMassPointPositions; }

	void Update();

private:
	void EulerIntegration(float dt, SMassPoint *mp);
	void VerletIntegration(float dt);
	void RK4Integration(float dt);

private:
	std::vector<SMassPoint*> mMassPoints;
	std::vector<SSpring*> mSprings;
	std::vector<glm::vec3> mMassPointPositions;

	double mPrevTime;
	double mCurrentTime;
};