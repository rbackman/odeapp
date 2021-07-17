#pragma once

#include "ode_box.h"
#include "common.h"
#include "ode_human.h"

class PID;
enum joint_types //hinge or universal joint
{
	HINGE_X,
	HINGE_Y,
	HINGE_Z,
	UNI_XY,
	UNI_XZ,
	UNI_YZ,
	BALL,
};
class ODEJoint
{
private:

	const char* jointName;
	void createHinge(dWorldID worldID ,dBodyID body1, dBodyID body2, GsVec jointPoint, int jtype);
	void createBall(dWorldID worldID, dBodyID body1, dBodyID body2,GsVec jointPoint, int jtype);
	void createUniversal(dWorldID worldID, dBodyID body1, dBodyID body2,GsVec jointPoint, int jtype);

public:
	ODEJoint(ODEHuman* wrld, KnJoint* j, GsVec dim, ODEJoint* prnt, int jtype);
	ODEJoint(ODEHuman* hm, KnJoint* j, GsVec pos, GsVec dim, ODEJoint* prnt, int jtype); // for custom placement of box
	~ODEJoint(void);

	const char* name(){return jointName;}


	void update();
	void clearTorque();

	//apply the rotationg of the ODEJoint to the euler angles of a KnJoint
	void applyToJoint(KnJoint* j); 
	//a controller can apply a torque that is accumulated in 'torque' and is applied to the joint on each frame
	GsVec torque;
	GsVec sampleTorque;
	float torqueSample;
	bool recentSampleT;
	void addSampleTorque(GsVec t){recentSampleT=true; sampleTorque = sampleTorque*torqueSample + t*(1-torqueSample);}
	void addTorque(GsVec t){torque+=t;}


	//the global position of the joint
	GsVec getPosition();
	//the position of the center of mass of the joint
	GsVec getCOMPosition();
	//should be the same as the com if the mass is alligned with the joint 
	GsVec jointCenter(){return (joint->child(0)->gcenter() + joint->gcenter())/2.0f;}
	//align the box with the center of the joint
	void setPosition(){box->model->setPosition(jointCenter());}

	//for now all joints have just a box for a body
	dBodyID getBodyID(){return box->getBodyID();}
	float getMass(){return box->getMass();}
	GsQuat getOrientation(){return box->getOrientation();}
	GsVec getRotationalVelocity(){return box->getRotationalVelocity();}
	GsVec getVelocity(){return box->getVelocity();}

	bool Rotating(){return box->Rotating();}


KnJoint* joint;

	int jointType;

	ODEHuman* h;
	ODEWorld* world;
	ODEBox* box;
	Ball* ball; //for displaying the joint position;
	ODEJoint* parent;

	dJointID jid;
	dJointID m_jid; //for ball and socket a seperate motor;

	bool usePID;
	bool grav;
	bool vforce;

	GsVec gcenter;
	GsVec vfMult;
	GsVec boxDim;
	float ax,ay,az;
	GsVec boxOffset;
	GsVec axis;

	void setAxis(float i,float j, float k){axis = GsVec(i,j,k); }
	void setAngles(GsVec ang){ax=ang.x;ay=ang.y;az=ang.z;}
};
