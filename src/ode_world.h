//ode_world.h


#ifndef __ODEWORLD_H__
#define __ODEWORLD_H__

#include <ode/ode.h>
#include "ode_contact.h"

#define MAX_CONTACT_FEEDBACK 64
#define MAX_CONTACT_POINTS 64
//#define TIME_MULTIPLIER 2 //don't go much above 5
#define STEP_ITERATIONS 10
//#define STEP_SIZE 0.03 //0.03 looks like real-time
class GsVec;
class ODEWorld 
{
public:
	ODEWorld(dReal gravity, dReal timeSpeed=1);
	~ODEWorld();

	void ODELoop(dReal deltaTime);
	//void ResetTimer(); //use this for window resizes



	dWorldID GetWorldID();
	dSpaceID GetSpaceID();
	GsVec GetGravity();
	dReal GetRayIntersectionDepth();
	
	GsArray<ODEContact*> contactPoints;


	// id of contact group
	dJointGroupID contactGroupID;
	//keep an array of contact points that is used for each pair of geom collisions
	dContact *cps;
	//this is the max number of contacts that are going to be processed between any two objects
	int maxContactCount;

	dJointFeedback jointFeedback[MAX_CONTACT_FEEDBACK];
	//this is the current number of contact joints, for the current step of the simulation
	int jointFeedbackCount;


private:
	dWorldID theDynamicsWorldID;
	dSpaceID theCollisionSpaceID;
	dJointGroupID theJointGroupID;
	dReal timeScale;
	dReal rayIntersectionDepth; //hack to keep track of ray penetration depth
};

//Helper function (this must not be a member of ODEWorld since we are passing its address as an argument)
void PotentialHitCallback(void *data, dGeomID o1, dGeomID o2);

#endif
