#include "controller_gravity.h"


GravityController::GravityController(ODEHuman* human):Controller(human)
{

}

GravityController::~GravityController(void)
{

}

bool GravityController::evaluate()
{
bool valid = Controller::evaluate();
	
	for (int i=0; i< h->numJoints(); i++)
	{
		ODEJoint* j = h->joint(i);
		//if (j->grav)
		//	addJointTorquesEquivalentToForce(j, GsVec() ,	GsVec(0, j->getMass()*GRAVITY, 0), 0);
	}

return valid;
}


