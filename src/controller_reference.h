#pragma once
#include "controller.h"

//This controller follows a reference KnSkeleton. It contains an array of pids linked between the ref
//joint and the corresponding ODEJoint. it assumes that all the joints in the ODEHuman have joints in the
//KnSkeleton with the same names.

//todo: remove the refskel and let the ODEHuman have prefered angles for each joint

class PID;
class ReferenceController: public Controller
{

public:
	ReferenceController(ODEHuman* human);
	~ReferenceController();
	bool evaluate();
	void attachRefSkel(KnSkeleton* s);
	
	//this sets the reference skeleton joint angles based on the joint angles of the ODEHuman
	void applyToRefSkel();

	//get a pid by its name. used by the UI
	PID* pid(const char* name);

	//bool useBubbleT;
	//void bubbleUpTorques();
		KnSkeleton* refSkel;

private:
	GsArray<PID*> pids;

};