#pragma once
#include "common.h"
#include "ode_human.h"
#include "ode_joint.h"

class SnGroup;
/*
A controller is a general class that has an instance of a human and does something to it
*/
enum controller_states
{
	CONTROLLER_IDLE,
	WALK_IDLE = CONTROLLER_IDLE,
	WALK_START,
	WALK_START_WEIGHT_LEFT,
	WALK_WEIGHT_LEFT,
	WALK_START_STEP_RIGHT,
	WALK_STEP_RIGHT, 
	WALK_START_WEIGHT_RIGHT,
	WALK_WEIGHT_RIGHT,
	WALK_START_STEP_LEFT,
	WALK_STEP_LEFT,
	WALK_STOP,
	WALK_STOPPING,
	CONTACT_LEFT_FOOT,
	CONTACT_TRANSITION_TO_LEFT,
	CONTACT_RIGHT_FOOT,
	CONTACT_TRANSITION_TO_RIGHT,
	CONTACT_BOTH_FEET,
	CONTACT_TRANSITION_TO_BOTH,
	CONTACT_FLYING,
	CONTACT_TRANSITION_TO_FLYING,
	STEP_RIGHT_FOOT,
	STEP_LEFT_FOOT,
	STEP_START_MOVING_RIGHT,
	STEP_START_MOVING_LEFT,
	STEP_START_WEIGHT_LEFT,
	STEP_START_WEIGHT_RIGHT,
	STEP_DONE_MOVING_LEFT,
	STEP_DONE_MOVING_RIGHT
};
class Controller 
{

private:
	bool active;
	int state;
	int nextState;
	float delay;
	float phase;
	bool changingStates;
protected:
	GsArray<Controller*> sub_controllers;
	SnGroup* grp; //for displaying anything you wish ;-)
	
public:
	Controller(ODEHuman* human);
	~Controller(void);

	bool evaluate();
	ODEHuman* h;
	SnGroup* getGroup(){return grp;}
	void visible(bool v); 
	bool isActive(){return active;}
	void deactivate(){active = false;}
	void activate(){active = true; phase = 0;}
	int getState(){return state;}

	void setState(int i){state = i; phase=0;}
	void setNextState(int i){nextState = i;}
	void goToState(int nstate, float sec)
	{
		changingStates = true;
		nextState = nstate; 
		delay = sec;
	}
	void idleToState(int nstate,float sec)
	{
		state = CONTROLLER_IDLE;
		goToState(nstate,sec);
	}
	void goToState(int nstate){goToState(nstate,0);}
	float getPhase(){return phase;}
};
