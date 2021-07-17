//ODESphere.cpp

#include "ode_sphere.h"

ODESphere::~ODESphere()
{
	delete model;
	if (dynamic)
	{
		dBodyDestroy(thisBodyID);
	}
	dGeomDestroy(thisGeomID);
}
ODESphere::ODESphere(ODEWorld* world, bool body, GsVec pos, dReal rad)
: ODEObject(body, pos)
{
	radius = rad;
	model = new Ball(pos,rad);
	model->setVisible(true);
	// ODE stuff
	thisGeomID = dCreateSphere(world->GetSpaceID(), radius);
	dGeomSetPosition(thisGeomID,pos.x,pos.y, pos.z);
	if (dynamic)
	{
		thisBodyID = dBodyCreate(world->GetWorldID());
		dGeomSetBody(thisGeomID, thisBodyID);

		//multiply volume by 8 to get mass; measured in kilograms?
		dReal newMass = ((dReal)2*pow(radius, 3));
		dMass mass;
		dBodyGetMass(thisBodyID, &mass);
		dMassAdjust(&mass, newMass);
		dBodySetMass(thisBodyID,&mass);
		dBodySetPosition(thisBodyID, pos.x, pos.y, pos.z);
		dBodySetForce(thisBodyID, 0, 0, 0);
		dBodySetLinearVel(thisBodyID, 0, 0, 0);
		dBodySetAngularVel(thisBodyID, 0, 0, 0);
	
	}
	else
	{
		dGeomSetBody(thisGeomID, 0);
	}
	model->setPosition(pos);
}




dReal ODESphere::GetRadius()
{
	return radius;
}
