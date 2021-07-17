#pragma once
#include "controller.h"

//This controller is specific for the hips to deal with single stance mode where torques get 
//large

class PID;
class SnTransform;

class RootController : public Controller
{
public:
	RootController(ODEHuman* human);
~RootController(void);
	bool evaluate();
	float Kd,Kp;
	float stanceHipDamping,stanceHipMaxVelocity;

private:
	GsVec computePDTorque(GsQuat qRel, GsQuat qRelD, GsVec wRel, GsVec wRelD);
	void computeHipTorques(GsQuat qRootD, float stanceHipToSwingHipRatio, GsVec ffRootTorque);
	PID* rootPID;
	GsVec previousOffset;

	float maxT;
	SnTransform* rootTransform; //global frame for root to achieve
};
