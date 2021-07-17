

#include "common.h"
#include "ode_joint.h"

class VirtualSensor
{
private:
	GsVec val;
	float overSample;
	GsVec sensorPos;
	float len;
	ODEJoint* joint;
	int lastUpdate;
	bool active;
public:
	VirtualSensor(ODEJoint* j, GsVec pos);

	//check if the sensor was activated on last simulation loop
	bool isActive(){return active;}
	//test if a contact applies to this sensor.. if it is close enough it assumes it is
	bool checkCollision(ODEContact* cont);
	void update();


	//get the current sensor value;
	GsVec getVal(){return val;}
	GsVec getPos(){return joint->getPosition() + joint->getOrientation().apply(sensorPos);}
};
