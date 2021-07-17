

#ifndef __ODEBOX_H__
#define __ODEBOX_H__

#include "ode_object.h"
#include "ode_world.h"
#include "ode_box.h"

#include <ode/ode.h>
#include "models.h"

class ODEBox : public ODEObject
{
public:
	ODEBox(ODEWorld* world, bool body, GsVec pos, GsVec size);
	~ODEBox();
	dReal GetXDimension();
	dReal GetYDimension();
	dReal GetZDimension();
	GsVec size;
protected:
	dReal myDimensions[3];
};

#endif
