#pragma once

#include <gsim/kn_joint.h>
#include <ode/ode.h>
#include "ode_world.h"
#include "ode_joint.h"

//This generates the torques necessary to have an ODEJoint follow a KnJoint
//consider replacing this the ODEHuman needs to have stiffer motion


class PID 
{
private:
		ODEJoint* joint;	
public:

	PID(ODEJoint* jnt);
	~PID(void);
	bool evaluate();
	ODEJoint* getJoint(){return joint;}
	float Ki,Kd,Kp;
	float torqueMultiplier;
	float maxT;

private:


	//used for the integral component which I never really use so I may delete it.
	float previousError;
	float integral;
	float previousError2;
	float integral2;
	float previousError3;
	float integral3;

 

};