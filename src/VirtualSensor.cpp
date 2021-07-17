
#include "VirtualSensor.h"


VirtualSensor::VirtualSensor(ODEJoint* j, GsVec pos)
{
	overSample = 0.9;
	sensorPos = pos;
	val = GsVec(0,0,0);

	joint = j;
}

void VirtualSensor::update()
{
	if(lastUpdate>2)
	{
		val = val*0.8f;
		active = false;
	}
	else
	{
		active = true;
	}
	lastUpdate++;
}
bool VirtualSensor::checkCollision(ODEContact* cont)
{
	if( cont->involves( joint->getBodyID() ))
	{

		GsVec dif = cont->cp - getPos();
		//dif.y=0;
		if(dif.len()<0.1f)
		{
			lastUpdate=0;
			val = val*overSample + cont->f*(1.0f-overSample);
			return true;

		}
	}
	return false;
}


