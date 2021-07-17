#pragma once
#include "controller.h"
#include "controller_step.h"
#include "curve.h"
#include "models.h"
#include <gsim/kn_motion.h>

class IKController;
//this controller is responsible for determining the current contact state and changing to a new one
//when desired.

class WalkController : public Controller
{

public:
	WalkController(ODEHuman* human);
	~WalkController(void);
	bool evaluate();
	void setDirection(float val); // val from -1 to 1
	void setSpeed(float cal);	  // val from -1 to 1
	void startWalking();
	void stopWalking();
	
	
	float stepHeight;
	float stepSpeed;
	float transitionSpeed;

	Arrow* headingArrow;
	Curve* comTraj;
private:

	//int walkFrame;

	//KnMotion* walkMotion;

	GsVec leftStart,rightStart;


	bool walking;
	float stepPhase;
	

	float angle;
	float speed;
	float goalSpeed;
	float nextSpeed;


	void updateState();
public:
	float getSpeed(){return speed;}
	float getGoalSpeed(){return goalSpeed;}
	bool isWalking(){return walking;}
	
};
