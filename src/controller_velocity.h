#pragma once
#include "controller.h"
#include <gsim/gs_matn.h>
#include <gsim/kn_joint.h>

//This controller trys to manipulate the center of mass velocity of the character by calculating the
//torques needed in the stance leg/s to achieve a specific COM velocity.
class VelocityController : public Controller
{
public:
	VelocityController(ODEHuman* human);
	~VelocityController(void);
	bool evaluate();
	void setVelocity(float sag,float side);
	
	float jkP;
	float jkD;
	float jkPc;
	float jkDc;
	float velDSagittal;
	float velDCoronal;

private:
	void computeBalanceTorques();
	void FixFeet();
	GsVec getVirtualForce();
	GsVec forceContribution(ODEJoint* start, ODEJoint* end, GsVec pos);
	GsVec compTorque(ODEJoint* j, GsVec force);

		
};
