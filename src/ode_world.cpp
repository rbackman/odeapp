//ODEWorld.cpp

#include "ode_world.h"
#include <gsim/gs_output.h>
#include "app_main.h"

#define CAMERA_COLLIDE_BITS 10
#define RAY_COLLIDE_BITS 20

//These are ONLY here to allow the callback function to have access to them.
dWorldID theGlobalWorldID;
dJointGroupID theGlobalJointGroupID;
dReal g_rayIntersectionDepth=-1;

ODEWorld::ODEWorld(dReal gravity, dReal timeSpeed)
{
	
	timeScale = timeSpeed;
	rayIntersectionDepth = -1;
	//timer.Init();
	dInitODE ();

	theDynamicsWorldID = dWorldCreate();
	dWorldSetGravity(theDynamicsWorldID, 0, gravity, 0);
	theCollisionSpaceID = dSimpleSpaceCreate(0);
	//dVector3 center = {0, 0, 0};
	//dVector3 extents = {200, 100, 200};
	//theCollisionSpaceID = dQuadTreeSpaceCreate(0, center, extents, 5);

	theJointGroupID = dJointGroupCreate(0);
	

	//These are ONLY here to allow the callback function to have access to them.
	theGlobalWorldID = theDynamicsWorldID;
	theGlobalJointGroupID = theJointGroupID;

	dWorldSetERP(theDynamicsWorldID, (dReal)0.9);
	dWorldSetCFM(theDynamicsWorldID, (dReal)0.001);
	dWorldSetContactSurfaceLayer(theDynamicsWorldID,(dReal)0.001);
	dWorldSetAutoDisableSteps(theDynamicsWorldID, 5);
//	dWorldSetAutoDisableThreshold(theDynamicsWorldID, (dReal)0.1);
}
#include <gsim/gs_vec.h>
GsVec ODEWorld::GetGravity()
{
	dVector3 gravity;
	dWorldGetGravity(theDynamicsWorldID,gravity);
	return GsVec(gravity[0],gravity[1],gravity[2]);
}
ODEWorld::~ODEWorld()
{
	//dSpaceDestroy(theCollisionSpaceID);
	dWorldDestroy(theDynamicsWorldID);
	dJointGroupDestroy(theJointGroupID);
	dCloseODE();
}

void ODEWorld::ODELoop(dReal deltaTime)
{
	//if (deltaTime > 0.1 || deltaTime <= 0)
	//{
	//	return; //we don't want the simulation to "explode" in these situations
	//}

	//clear the previous list of contact forces
	contactPoints.remove(0,contactPoints.size());
	jointFeedbackCount = 0;

	deltaTime *= timeScale;

	// 1. Apply forces to bodies as necessary.
		//nothing for now...

	// 2. Adjust the joint parameters as necessary.
		//nothing for now...

	// 3,4. Call collision detection; add contacts to contact joint group.
		g_rayIntersectionDepth = -1; //reset this each time
		dSpaceCollide(theCollisionSpaceID, 0, &PotentialHitCallback);
		rayIntersectionDepth = g_rayIntersectionDepth;

	// 5. Take a Simulation step.
		  dWorldQuickStep (theDynamicsWorldID,deltaTime); 
		// dWorldStepFast1(theDynamicsWorldID, deltaTime, STEP_ITERATIONS);
		//dWorldStep(theDynamicsWorldID, deltaTime);


	//copy over the force information for the contact forces
	for (int i=0;i<jointFeedbackCount;i++)
	{
		ODEContact* c = contactPoints.get(i);
		c->f = GsVec(jointFeedback[i].f1[0], jointFeedback[i].f1[1], jointFeedback[i].f1[2]);
		//make sure that the force always points away from the static objects
		//if (contactPoints[i].rb1->isLocked() && !contactPoints[i].rb2->isLocked()){
			c->f = c->f * (-1);
			dBodyID tid = c->rb1;
			c->rb1 = c->rb2;
			c->rb2 = tid;
		//}
	}

	// 6. Remove all joints from the contact group.
		dJointGroupEmpty(theJointGroupID);
}

void PotentialHitCallback(void *data, dGeomID o1, dGeomID o2)
{
	if(App->human->collideFeetOnly)
	{
		for(int i=0;i<App->human->numJoints();i++)
		{
			ODEJoint* j = App->human->joint(i);
			if(j == App->human->joint("LeftFoot") || j == App->human->joint("RightFoot")|| j == App->human->joint("LeftToe")|| j == App->human->joint("RightToe"))
			{
				// ok to check these ones
			}
			else
			{
				if(o1 == j->box->GetGeomID() || o2 == j->box->GetGeomID())
				{
					//if either one of the bodies is not a foot then ignore the collision
					return;
				}
			}
		}
	}
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{ // colliding a space with someting
		dSpaceCollide2(o1, o2, data, &PotentialHitCallback);
		
		// now colliding all geoms internal to the space(s)
		if (dGeomIsSpace(o1))
		{
			dSpaceID o1_spaceID = (dSpaceID)o1; // this may not be valid
			dSpaceCollide(o1_spaceID, data, &PotentialHitCallback);
		}
		if (dGeomIsSpace(o2))
		{
			dSpaceID o2_spaceID = (dSpaceID)o2; // this may not be valid
			dSpaceCollide(o2_spaceID, data, &PotentialHitCallback);
		}
	}
	else
	{ // colliding two "normal" (non-space) geoms

		//use category/collide bitfields here

		//don't collide camera ray with camera sphere
		if (dGeomGetCollideBits(o1) == RAY_COLLIDE_BITS && 
			dGeomGetCollideBits(o2) == CAMERA_COLLIDE_BITS ||
			dGeomGetCollideBits(o1) == CAMERA_COLLIDE_BITS && 
			dGeomGetCollideBits(o2) == RAY_COLLIDE_BITS)
		{
			return;
		}

		int num_cp; // number of contact points
		dContactGeom cp_array[MAX_CONTACT_POINTS];
		num_cp = dCollide(o1, o2, MAX_CONTACT_POINTS, cp_array, sizeof(dContactGeom));

		dBodyID o1BodyID = dGeomGetBody(o1);
		dBodyID o2BodyID = dGeomGetBody(o2);

		// filter out collisions between joined bodies (except for contact joints)
		if (o1BodyID !=0 && o2BodyID != 0)
			if (num_cp > 0 && dAreConnectedExcluding(o1BodyID, o2BodyID, dJointTypeContact))
				
			{
			//	gsout << "ignored connected\n";			
				return;
			}
		//// checking whether one of the geoms is camera ray
		if (dGeomGetCollideBits(o1) == RAY_COLLIDE_BITS || 
			dGeomGetCollideBits(o2) == RAY_COLLIDE_BITS)
		{	
			//if a contact occurred...
			if(num_cp > 0)
			{
				//only keep smallest depth
				if (g_rayIntersectionDepth == -1 || cp_array[0].depth < g_rayIntersectionDepth)
				{
					g_rayIntersectionDepth = cp_array[0].depth;
				}
			}
		}
		else //we want to do physical reactions with all objects but the ray
		{
			// add these contact points to the simulation
			for(int i=0; i<num_cp; i++)
			{
				dContact tempContact;// = new dContact;
				tempContact.surface.mode = 0;

				tempContact.surface.mode = dContactApprox1 | dContactSlip1 | dContactSlip2 | dContactSoftCFM;
				tempContact.surface.slip1 = (dReal)0.0;
				tempContact.surface.slip2 = (dReal)0.0;

				//tempContact.surface.mode = dContactApprox1;
				//tempContact.surface.mode = dContactBounce | dContactSoftCFM;
				tempContact.surface.bounce = 0.0;
				tempContact.surface.bounce_vel = (dReal)0.0;
				tempContact.surface.soft_cfm = (dReal)0.01;

				//tempContact.surface.mu = dInfinity;

				tempContact.surface.mu = 100;
				tempContact.geom = cp_array[i];
				//tempContact.surface.mu2 = 0;

				dJointID j = dJointCreateContact(theGlobalWorldID, theGlobalJointGroupID, &tempContact);
				dJointAttach(j, dGeomGetBody(o1), dGeomGetBody(o2));
				ODEContact* c = new ODEContact;
				c->rb1 = dGeomGetBody(o1);
				c->rb2 = dGeomGetBody(o2);
				c->jid = j;
				c->cp = GsVec(tempContact.geom.pos[0],tempContact.geom.pos[1],tempContact.geom.pos[2]);
				dJointSetFeedback(j,&(App->world->jointFeedback[App->world->jointFeedbackCount]));
				App->world->jointFeedbackCount++;

				App->world->contactPoints.push(c);
				
			}
		}
	}
}

//This function is taken from the test_boxstack.cpp test file.
//void ODEWorld::PotentialHitCallback(void *data, dGeomID o1, dGeomID o2)
//{
//  int i;
//
//  // exit without doing anything if the two bodies are connected by a joint
//  dBodyID b1 = dGeomGetBody(o1);
//  dBodyID b2 = dGeomGetBody(o2);
//  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;
//
//  dContact contact[MAX_CONTACT_POINTS];   // up to MAX_CONTACTS contacts per box-box
//  for (i=0; i<MAX_CONTACT_POINTS; i++) {
//    contact[i].surface.mode = dContactBounce | dContactSoftCFM;
//    contact[i].surface.mu = dInfinity;
//    contact[i].surface.mu2 = 0;
//    contact[i].surface.bounce = 0.1;
//    contact[i].surface.bounce_vel = 0.1;
//    contact[i].surface.soft_cfm = 0.01;
//  }
//  if (int numc = dCollide (o1,o2,MAX_CONTACT_POINTS,&contact[0].geom,
//			   sizeof(dContact))) {
//    dMatrix3 RI;
//    dRSetIdentity (RI);
//    const dReal ss[3] = {0.02,0.02,0.02};
//    for (i=0; i<numc; i++) {
//      dJointID c = dJointCreateContact (theDynamicsWorldID,theJointGroupID,contact+i);
//      dJointAttach (c,b1,b2);
//    }
//  }
//}

dWorldID ODEWorld::GetWorldID()
{
	return theDynamicsWorldID;
}

dSpaceID ODEWorld::GetSpaceID()
{
	return theCollisionSpaceID;
}

dReal ODEWorld::GetRayIntersectionDepth()
{
	return rayIntersectionDepth;
}
