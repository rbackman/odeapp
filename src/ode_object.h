

#ifndef __ODE_OBJECT_H__
#define __ODE_OBJECT_H__

#include "models.h"

#include <ode/ode.h>

#define PI 3.14159265358979323846f
#define	DEG2RAD(x) (((x)*PI)/180.0)
#define	RAD2DEG(x) (((x)*180.0)/PI)

class GsVec;
class GsQuat;

class Model;
class ODEObject 
{
public:
	//ODEObject();
	ODEObject(bool body, GsVec pos);
	virtual ~ODEObject();
	void update();
	void setColor(GsColor c);
	GsVec getPosition();
	GsQuat getOrientation();
	void setVelocity(GsVec velocity);
	bool Rotating();
	GsVec getRotationalVelocity();
	GsVec getVelocity();
	float getMass();
	dBodyID getBodyID();
	void SetBodyID(dBodyID bodyID);
	dGeomID GetGeomID();
	void SetGeomID(dGeomID geomID);
	bool isDynamic();
	
		Model* model;
protected:
	// ODE stuff
	dBodyID thisBodyID;
	dGeomID thisGeomID;
	bool dynamic;
};

#endif
