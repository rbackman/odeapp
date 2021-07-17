#include "controller.h"
#include "ode_human.h"
#include "ode_joint.h"
#include <gsim/sn_group.h>
Controller::Controller(ODEHuman* human)
{
active = true;
h=human;
grp = new SnGroup;
grp->separator(true);
phase = 0;
changingStates = false;
setState(CONTROLLER_IDLE);
}

Controller::~Controller(void)
{
	grp->remove_all();
	grp->unref();
}

void Controller::visible(bool v)
{
	grp->visible(v);
}
bool Controller::evaluate()
{
//	if(!active)return false;
	bool result = true;
	if(changingStates)
	{
		delay-=TIME_STEP;
		if(delay<=0)
		{
			changingStates = false;
			state = nextState;
			delay=0;
		}
	}
	
	//phase+= timeStep;
	//if(phase>=1)deactivate();

	for(int i=0;i<sub_controllers.size();i++)
	{
		Controller* c = sub_controllers.get(i);
		if(c->active)
			if(!c->evaluate())result = false;
	}
return result;
}