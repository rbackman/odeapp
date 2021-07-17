#pragma once
#include "controller.h"
#include "curve.h"

//this controller determines where the next foot position should be and generates trajectories
//for the COM and the swing foot

class StepController : public Controller
{
enum 
{
LEFT_FOOT,
RIGHT_FOOT
};
private:
	GsVec getIPStepPos(int foot);

	SnManipulator* leftStepManip;
	SnManipulator* rightStepManip;

	Curve* leftFootTraj;
	Curve* rightFootTraj;

	float tLeft,tRight; //curve point for each foot
	
	GsVec leftFootStart;
	GsVec leftFootEnd;
	GsVec rightFootStart;
	GsVec rightFootEnd;

	float stepHeight;
	float speed;
	bool stepping;
	int startFoot;

public:
	StepController(ODEHuman* human);
	~StepController(void);
	
	void setStepHeight(float h){stepHeight = h; makeCurves();}
	void makeCurves();
	bool evaluate();

	bool doneStepping(){return !stepping;}
	
	void makeStep(int step); //the arg is the start foot to move

	GsVec getLeftFootPosition();
	GsVec getRightFootPosition();
	GsVec getLeftFootPosition(float t);
	GsVec getRightFootPosition(float t);

};
