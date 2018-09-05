#include "Physics.h"
#include <GLFW/glfw3.h>

#include <iostream>

const glm::vec3 GRAVITY_F(0.0f, -9.81, 0.0f);

// -------------------------------------------------------------------------------------------
CPhysicsSystem::CPhysicsSystem()
{
	mCurrentTime = glfwGetTime();
	mPrevTime = mCurrentTime;
}

// -------------------------------------------------------------------------------------------
CPhysicsSystem::~CPhysicsSystem()
{
	// TODO: deallocate mass points and springs
}

// -------------------------------------------------------------------------------------------
void CPhysicsSystem::CreateRope(float segmentLength, int segmentCount)
{
	const glm::vec3 STARTING_POINT(0.0f, 0.5f, 0.0f);

	const float K = 50;
	const float MASS = 0.1f;

	// Build rope to the right
	for (int i = 0; i < segmentCount; i++)
	{
		// Create mass point
		SMassPoint* mp = new SMassPoint();
		mp->anchor = (i == 0);
		mp->pos = STARTING_POINT + glm::vec3(i * segmentLength, 0.0f, 0.0f);
		mp->mass = MASS;

		mMassPoints.push_back(mp);
		mMassPointPositions.push_back(mp->pos);

		// Create spring
		if (i > 0)
		{
			SSpring* s = new SSpring();

			s->ks = K;
			s->l = segmentLength;
			s->pPoints[0] = mMassPoints[mMassPoints.size() - 2];
			s->pPoints[1] = mMassPoints[mMassPoints.size() - 1];

			mSprings.push_back(s);
		}
	}
}

// -------------------------------------------------------------------------------------------
void CPhysicsSystem::Update()
{
	std::vector<SMassPoint*>::iterator mpIt;
	std::vector<SSpring*>::iterator sIt;

	// external forces on each mass point (gravity)
	for (mpIt = mMassPoints.begin(); mpIt != mMassPoints.end(); ++mpIt)
	{
		if (!(*mpIt)->anchor)
		{
			(*mpIt)->f = GRAVITY_F;
		}
	}

	// spring forces on mass points connected by them
	for (sIt = mSprings.begin(); sIt != mSprings.end(); ++sIt)
	{
		glm::vec3 p12 = (*sIt)->pPoints[1]->pos - (*sIt)->pPoints[0]->pos;
		float diff = glm::length(p12) - (*sIt)->l;
		if (diff != 0)
		{
			glm::vec3 f = -(*sIt)->ks * glm::normalize(p12) * diff;
			(*sIt)->pPoints[0]->f -= f;
			(*sIt)->pPoints[1]->f += f;
		}
	}

	// update movement
	mCurrentTime = glfwGetTime();
	double dt = mCurrentTime - mPrevTime;
	mPrevTime = mCurrentTime;
	int i = 0;
	for (mpIt = mMassPoints.begin(); mpIt != mMassPoints.end(); ++mpIt, ++i)
	{
		if (!(*mpIt)->anchor)
		{
			EulerIntegration(dt, *mpIt);
			mMassPointPositions[i] = (*mpIt)->pos;
		}
	}
}

// -------------------------------------------------------------------------------------------
void CPhysicsSystem::EulerIntegration(float dt, SMassPoint *mp)
{
	mp->pos = mp->pos + mp->vel * dt;
	mp->vel = mp->vel + (mp->f / mp->mass) * dt;
}

// -------------------------------------------------------------------------------------------
void CPhysicsSystem::VerletIntegration(float dt)
{
}

// -------------------------------------------------------------------------------------------
void CPhysicsSystem::RK4Integration(float dt)
{
}
