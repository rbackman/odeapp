#pragma once
#include "controller.h"

//this controller is responsible for computing the gravity compensation torques
//that is depending on the orientation of the joints the force required to have the character
//stay in position without falling

//doesnt do anything now

class GravityController : public Controller
{
public:
	GravityController(ODEHuman* human);
	~GravityController(void);
	bool evaluate();
	float mag;
private:
};
