//ODEObject.cpp

#include "ode_object.h"
#include <gsim/sn_transform.h>
#include <gsim/gs_vec.h>
#include <gsim/sn_group.h>
#include <gsim/gs_quat.h>
#include <gsim/gs_mat.h>

ODEObject::ODEObject(bool dyn, GsVec pos) 
{
	dynamic = dyn;
	model = 0;
}

ODEObject::~ODEObject()
{if(model)delete model;}
void ODEObject::setColor(GsColor c)
{
	model->setColor(c.r,c.g,c.b);
}
float ODEObject::getMass()
{
	dMass mass;
	dBodyGetMass(thisBodyID,&mass);
	return (float)mass.mass;

}

GsVec ODEObject::getPosition()
{
if(dynamic)
{
	const dReal* p = dBodyGetPosition(thisBodyID);
	return GsVec(p[0],p[1],p[2]);
}
	const dReal* p = dGeomGetPosition(thisGeomID);
	return GsVec(p[0],p[1],p[2]);
}


GsQuat ODEObject::getOrientation()
{
	const dReal* Q = dBodyGetQuaternion(thisBodyID);
	return GsQuat(Q[0],Q[1],Q[2],Q[3]);
}
void ODEObject::update()
{
	if(dynamic)
	{
		model->setPosition(getPosition());
		model->setRotation(getOrientation());
	}
}

void ODEObject::setVelocity(GsVec velocity)
{
	if (dynamic)
	{
		const dReal *CurrentVel;
		CurrentVel=dBodyGetLinearVel(thisBodyID);
		float K=100.0f;
		GsVec Force = GsVec( K*(velocity.x-CurrentVel[0]) , K*(velocity.y-CurrentVel[1]) , K*(velocity.z-CurrentVel[2]) ); 
		dBodyAddForce(thisBodyID,Force.x,Force.y,Force.z);
		//dBodySetLinearVel(thisBodyID, velocity.x, velocity.y, velocity.z);
	}
	else // this object is only a geom
	{
		//geoms have no velocity, so this shouldn't be called
	}
}
bool ODEObject::Rotating()
{
	if(getRotationalVelocity().len()>1.0)return true;
	return false;
}
GsVec ODEObject::getRotationalVelocity()
{
const dReal* velocity;
	GsVec newVelocity;

	if (dynamic)
	{
		velocity = dBodyGetAngularVel(thisBodyID);
		newVelocity = GsVec( velocity[0], velocity[1], velocity[2]); 
	}
	else // this object is only a geom
	{
		//geoms have no velocity, so this shouldn't be called
		
	}
	return newVelocity;
}
GsVec ODEObject::getVelocity()
{
	const dReal* velocity;
	GsVec newVelocity;

	if (dynamic)
	{
		velocity = dBodyGetLinearVel(thisBodyID);

		newVelocity = GsVec( velocity[0], velocity[1], velocity[2]); 

		return newVelocity;
	}
	else // this object is only a geom
	{
		//geoms have no velocity, so this shouldn't be called
		return newVelocity;
	}
}

dBodyID ODEObject::getBodyID(){ return thisBodyID;}
void ODEObject::SetBodyID(dBodyID bodyID){thisBodyID = bodyID;}
dGeomID ODEObject::GetGeomID(){return thisGeomID;}
void ODEObject::SetGeomID(dGeomID GeomID){thisGeomID = GeomID;}
bool ODEObject::isDynamic(){return dynamic;}

