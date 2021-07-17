#pragma once
#include "controller.h"

#include "VirtualSensor.h"

//this controller is responsible for determining the current contact state and changing to a new one
//when desired. or by automatically detecting changes in contact based on foot rotation and position
//it maintains 4 pressure sensors on each foot that are computed from the rigid body collisions .

class ContactController : public Controller
{
	enum foot_modes
	{
		CONTACT_FIXED_FOOT,
		CONTACT_TOES,
		CONTACT_HEELS
	};
private:
	int contactState;

public:
	void goToToes()	{	contactState = CONTACT_TOES;	  }
	void goToHeels(){	contactState = CONTACT_HEELS;	  }
	void fixFoot()	{	contactState = CONTACT_FIXED_FOOT;}

	ContactController(ODEHuman* human);
	~ContactController(void);
	
	void goToContact(int i);
	int getContactState();
	
	//check if the COM projection is within the convex hull of the feet
	bool balanced();
	bool bothFeetContain(GsVec p);
	bool leftFootContains(GsVec p);
	bool rightFootContains(GsVec p);

	//determine if the foot is planted by checking all the sensors for recent activation
	bool leftFootPlanted();
	bool rightFootPlanted();
	bool bothFeetPlanted();

private:
	void updateContactRegion();
	int checkContactState();
	GsVec updateContactCenter();

	bool isBalanced;

	GsVec lastContactPos;
	GsVec nextContactPos;
	GsVec contactPos;
	float contactTransition;
	bool contactTransitioning;
	float contactTransitionSpeed;
	

	GsVec nextRootPos;
	GsVec lastRootPos;
	GsVec rootPos;
	float rootTransition;
	bool rootTransitioning;
	float rootMix; //1 puts root over foot completely 0 doesnt change root
	float rootTransitionSpeed;
	GsVec updateRootCenter();

	GsPolygon contact_poly;

	Ball* contact_model;
	SnGroup* _lines;
	SnGroup* _contactLines;
	
enum sensor_positions
{
	LFL,
	LFR,
	LBL,
	LBR,
	RFL,
	RFR,
	RBL,
	RBR
};
	VirtualSensor* sensors[8]; //these are the four virtual sensor values.
	float overSample;

public:
	void setTransitionSpeed(float s){contactTransitionSpeed = s; rootTransitionSpeed = s*0.5f;}
	GsVec getRootPos(){return rootPos;}
	GsVec getContactCenter(){return contactPos;}
	bool evaluate();
};
