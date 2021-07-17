//ODEBox.cpp

#include "ode_box.h"
#include <math.h>
#include <gsim/gs_box.h>

ODEBox::ODEBox(ODEWorld* world, bool body, GsVec pos, GsVec sze)
: ODEObject(body, pos)
{
	size = sze;
	model = new Box(pos,sze);
	myDimensions[0] = size.x;
	myDimensions[1] = size.y;
	myDimensions[2] = size.z;

	// ODE stuff
	thisGeomID = dCreateBox(world->GetSpaceID(), myDimensions[0], myDimensions[1], myDimensions[2]);
	dGeomSetPosition(thisGeomID,pos.x,pos.y, pos.z);
	if (dynamic)
	{
		thisBodyID = dBodyCreate(world->GetWorldID());
		dGeomSetBody(thisGeomID, thisBodyID);

		//multiply volume by 8 to get mass; measured in kilograms?
		dReal newMass = myDimensions[0]*myDimensions[1]*myDimensions[2];
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


}

ODEBox::~ODEBox()
{
	if (dynamic)
	{
		dBodyDestroy(thisBodyID);
	}

	dGeomDestroy(thisGeomID);
}

dReal ODEBox::GetXDimension()
{
	return myDimensions[0];
}

dReal ODEBox::GetYDimension()
{
	return myDimensions[1];
}

dReal ODEBox::GetZDimension()
{
	return myDimensions[2];
}
