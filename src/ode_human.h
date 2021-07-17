
#pragma once

#include "common.h"

#include <ode/ode.h>
#include "ode_object.h"

class KnSkeleton;
class ODEWorld;
class ODEJoint;
class ODESphere;
class ODEBox;
class Curve;
class Ball;
class Model;
class RootController;
class VelocityController;
class ReferenceController;
class StepController;
class WalkController;
class ContactController;
class GravityController;
class IKController;
class JIKController;


class ODEHuman : public ODEObject
{
public:
	KnSkeleton* sk;
	SnGroup* grp;
	GsVec initialPos;
	GsVec pos;
	//some feed-forward torque we want the root to see...
	GsVec ffRootTorque;

	RootController*		 root_controller;
	VelocityController*  velocity_controller;
	ReferenceController* reference_controller;
	StepController*		 step_controller;
	WalkController*		 walk_controller;
	ContactController*	 contact_controller;
	GravityController*	 gravity_controller;
	IKController*		 ik_controller;
	JIKController*		 jik_controller;
	ODEHuman(ODEWorld* world, KnSkeleton* skel); 
	~ODEHuman(); 
	int checkContactState();
	void makeControllers();
	void update();
	void setDesiredVelocity(float saf, float cor);
	void setInitialPositionFromSkel();
	void setTotalVelocity(GsVec velocity);
	void attachRefSkel(KnScene* s);
	void setTorqueMultiplier(float val,float maxt);
	ODEJoint* joint(const char* name);
	ODEJoint* joint(int idx);
	int numJoints();
	GsVec getCOM();
	GsVec computeCOM();
	GsVec getPosition();
	GsVec getHeadPosition();
	GsVec getLeftFootPosition();
	GsVec getRightFootPosition();
	float getHipHeight(){return comHeight;}
	void setHipHeight(float am);
	bool balanced();
	int getContactState();
	ODEWorld* world;

	bool useComJ;
	bool useGravC;
	bool usePID;
	bool useRoot;
	bool collideFeetOnly;
	float comHeight;
	GsVec getStanceCenter();
	GsVec getVelocity();
	GsVec getHeading();


	float getMass();
	float computeMass();
	ODEJoint* hips();




private:
	float floor_height;
	float height_offset;
	float mass;
	void applyToSkel();
	GsArray<ODEJoint*> joints;
	
	SnGroup* _lines;
	//positions and desired positions of COM and COM projection
	Ball* com_model;
	Ball* com_d_model;
	Ball* com_proj_model;
	Ball* com_d_proj_model;
	GsVec com;
	GsVec com_d;
	GsVec com_proj;
	GsVec com_d_proj;

	GsMat local;
	GsQuat rot;
	GsQuat initialRot;

public:
	float getHeightOffset(){return height_offset;}
	void setHeightOffset(float ds){height_offset = ds;}
	GsQuat getRot();
	float getFloorHeight(){return floor_height;}
	GsVec getDesiredCOM(){return com_d;}
	void setDesiredCOM(GsVec p){com_d=p; com_d_proj = com_d; com_d_proj.y=floor_height;}
};