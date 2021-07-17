//ODEHuman.cpp

#include "ode_human.h"

#include "common.h"
#include "ode_world.h"
#include "ode_sphere.h"
#include "ode_box.h"
#include "ode_object.h"
#include "ode_joint.h"
#include "controller_root.h"
#include "controller_velocity.h"
#include "controller_reference.h"
#include "controller_step.h"
#include "controller_walk.h"
#include "controller_contact.h"
#include "controller_ik.h"
#include "controller_gravity.h"
#include "controller_jik.h"

#include "pid.h"
#include "controller.h"
#include "curve.h"
#include <string.h>
#include "models.h"

#include <math.h>
#define POS(x) sk->joint(x)->gcenter()
#define JOINT(x) sk->joint(x)


float getLength(KnJoint* j)
{
	if(j->child(0))
	{
		GsVec dis = j->child(0)->gcenter()-j->gcenter();
		return dis.len();
	}
	return 0;
}

ODEHuman::ODEHuman(ODEWorld* wrld, KnSkeleton* skel): ODEObject(false, GsVec(0,0,0))
{

	velocity_controller = 0;
	root_controller = 0;
	reference_controller = 0;
	step_controller = 0;
	
	walk_controller = 0;
	contact_controller = 0;
	gravity_controller = 0;
	ik_controller=0;
	jik_controller=0;
	sk = skel;

	floor_height = 0;
	//sk=new KnSkeleton;
	//bool ok = sk->load("../src/Man.s");
	//if ( !ok ) gsout.fatal("Could not load skeleton file in ODEHuman!");
	world = wrld;

	_lines = new SnGroup;
	GsVec origin = sk->root()->pos()->value();

	//All dimensions are x,y,z
	float size = 1.0f;

	GsVec hipsDimensions	 =	GsVec(1.4f*size,	0.8f*getLength(JOINT("Spine1")),	1.0f*size);
	GsVec shoulderDimensions =	GsVec(1.8f*size,	0.7f*getLength(JOINT("Spine1")),	1.0f*size);
	GsVec headDimensions	 =  GsVec(0.7f,			0.7f,								 0.7f);
	GsVec upperArmDimensions =	GsVec(0.4f*size,	1.2f*getLength(JOINT("LeftUpArm")),	0.4f*size);
	GsVec lowerArmDimensions =	GsVec(0.3f*size,	1.2f*getLength(JOINT("LeftArm")),	0.3f*size);
	GsVec upperLegDimensions =	GsVec(0.5f*size,	1.2f*getLength(JOINT("LeftUpLeg")), 0.6f*size);
	GsVec lowerLegDimensions =	GsVec(0.4f*size,	1.2f*getLength(JOINT("LeftUpLeg")), 0.5f*size);
	GsVec footVec = JOINT("LeftToe")->gcenter() - JOINT("LeftHeelTip")->gcenter();

	GsVec footDimensions	 =	GsVec(0.6f*size,	0.3f*size,		footVec.len());
	GsVec toeDimensions	     =	GsVec(0.6f*size,	0.3f*size,		getLength(JOINT("LeftToe")));

	height_offset = 0;

	//ODEJoint::ODEJoint(ODEHuman* hm, KnJoint* j, GsVec size, ODEJoint* parent, int jointType)

	joints.push(new ODEJoint(this, JOINT("Spine1"),	hipsDimensions,0,-1));
	joints.push(new ODEJoint(this, JOINT("Spine2"),	hipsDimensions,hips(),UNI_XZ));
	joints.push(new ODEJoint(this, JOINT("Shoulder"),	shoulderDimensions,joint("Spine2"),UNI_XZ));
	joints.push(new ODEJoint(this, JOINT("Head"),		headDimensions,joint("Shoulder"),HINGE_X));
	joints.push(new ODEJoint(this, JOINT("LeftUpArm"),upperArmDimensions,joint("Shoulder"),UNI_XZ));
	joints.push(new ODEJoint(this, JOINT("RightUpArm"),upperArmDimensions,joint("Shoulder"),UNI_XZ));
	joints.push(new ODEJoint(this, JOINT("LeftArm"),	lowerArmDimensions,joint("LeftUpArm"),HINGE_X));
	joints.push(new ODEJoint(this, JOINT("RightArm"),	lowerArmDimensions,joint("RightUpArm"),HINGE_X));

	joints.push(new ODEJoint(this, JOINT("LeftUpLeg"),upperLegDimensions,hips(),UNI_XZ));//,BALL));
	joints.push(new ODEJoint(this, JOINT("RightUpLeg"),upperLegDimensions,hips(),UNI_XZ));//,BALL));
	joints.push(new ODEJoint(this, JOINT("LeftLeg"),	lowerLegDimensions,joint("LeftUpLeg"),HINGE_X));
	joints.push(new ODEJoint(this, JOINT("RightLeg"),	lowerLegDimensions,joint("RightUpLeg"),HINGE_X));

	GsVec footCenter = interp( JOINT("LeftToe")->gcenter(), JOINT("LeftHeelTip")->gcenter(), 0.5f) ;
	joints.push(new ODEJoint(this, JOINT("LeftFoot"),footCenter,	footDimensions,joint("LeftLeg"),UNI_XZ));
	footCenter = interp(JOINT("RightToe")->gcenter(),JOINT("RightHeelTip")->gcenter(),0.5f);
	joints.push(new ODEJoint(this, JOINT("RightFoot"),footCenter,footDimensions,joint("RightLeg"),UNI_XZ));

	if( TOES)
	{
		joints.push(new ODEJoint(this, JOINT("LeftToe"),toeDimensions,joint("LeftFoot"),HINGE_X));
		joints.push(new ODEJoint(this, JOINT("RightToe"),toeDimensions,joint("RightFoot"),HINGE_X));
	}
	mass = computeMass();
	computeCOM();
	dJointSetHingeParam(joint("LeftLeg")->jid, dParamLoStop, 0);
	dJointSetHingeParam(joint("RightLeg")->jid, dParamLoStop, 0);

	grp = new SnGroup();
	grp->separator(true);

	thisBodyID = hips()->box->getBodyID();
	const dReal* initialQ = dBodyGetQuaternion(hips()->getBodyID());
	initialRot = GsQuat(initialQ[0],initialQ[1],initialQ[2],initialQ[3]);
	ODEJoint* j = joints.get(0);

	for(int i=0;i<joints.size();i++)
	{
		grp->add(joints.get(i)->box->model->getGrp());
		grp->add(joints.get(i)->ball->getGrp());
	}

	com_model	 = new Ball(getCOM(),0.1f); com_model->setVisible(true);
	com_proj_model = new Ball(getCOM(),0.1f); com_proj_model->setVisible(true);
	com_d_model	 = new Ball(getCOM(),0.1f); com_d_model->setVisible(true);  com_d_model->setColor(1,0,0);
	com_d_proj_model = new Ball(getCOM(),0.1f); com_d_proj_model->setVisible(true); com_d_proj_model->setColor(1,0,0);

	com_d = getCOM();
	grp->add(com_model->getGrp());
	grp->add(com_proj_model->getGrp());
	grp->add(com_d_model->getGrp());
	grp->add(com_d_proj_model->getGrp());
	grp->add(_lines);
}
GsQuat ODEHuman::getRot()
{
	rot = hips()->getOrientation();
	return rot;
}
ODEHuman::~ODEHuman()
{
	for(int i=0;i<joints.size();i++)
	{ delete joints.get(i);}

	joints.remove(0,joints.size());
	if(walk_controller){delete walk_controller; walk_controller=0;}
	if(velocity_controller){delete velocity_controller; velocity_controller = 0;}
	if(root_controller){delete root_controller; root_controller = 0;}
	if(reference_controller){delete reference_controller; reference_controller = 0;}
	if(step_controller){delete step_controller; step_controller = 0;}
	if(gravity_controller){delete gravity_controller; gravity_controller = 0;}

	if(contact_controller){delete contact_controller; contact_controller = 0;}
	if(ik_controller){delete ik_controller; ik_controller = 0;}
	if(jik_controller){delete jik_controller; jik_controller=0;}
	grp->remove_all();
}

void ODEHuman::makeControllers()
{
	if(walk_controller){delete walk_controller; walk_controller=0;}
	if(velocity_controller){delete velocity_controller; velocity_controller = 0;}
	if(root_controller){delete root_controller; root_controller = 0;}
	if(reference_controller){delete reference_controller; reference_controller = 0;}
	if(step_controller){delete step_controller; step_controller = 0;}
	if(gravity_controller){delete gravity_controller; gravity_controller = 0;}

	if(contact_controller){delete contact_controller; contact_controller = 0;}
	if(ik_controller){delete ik_controller; ik_controller=0;}
	if(jik_controller){delete jik_controller; jik_controller=0;}
	gravity_controller = new GravityController(this);
	contact_controller = new ContactController(this);
	velocity_controller = new VelocityController(this); 
	root_controller = new RootController(this);	
	reference_controller = new ReferenceController(this); 
	step_controller = new StepController(this); 

	walk_controller = new WalkController(this);
	ik_controller = new IKController(this);
	jik_controller = new JIKController(this);

	//grp->add(jik_controller->getGroup());
	//grp->add(step_controller->getGroup());

	grp->add(velocity_controller->getGroup());
	grp->add(root_controller->getGroup());
	grp->add(reference_controller->getGroup());
	grp->add(gravity_controller->getGroup());
	grp->add(walk_controller->getGroup());
	grp->add(contact_controller->getGroup());
	grp->add(ik_controller->getGroup());

}
void ODEHuman::setHipHeight(float am)
{
	comHeight = am;
	ik_controller->setHeight(comHeight);
}
void ODEHuman::attachRefSkel(KnScene* s)
{
	reference_controller->attachRefSkel(s->skeleton());
	ik_controller->setScene(s);
}

GsVec ODEHuman::getHeading()
{
	GsVec dir = GsVec(0,0,1);
	GsQuat q = hips()->getOrientation();
	dir = q.apply(dir);
	dir.y=0;
	dir.normalize();
	return dir;
}
void ODEHuman::setDesiredVelocity(float sag, float cor)
{
	velocity_controller->setVelocity(sag,cor);
}
int ODEHuman::numJoints(){return joints.size();}
ODEJoint* ODEHuman::hips(){return joint(0);}
float ODEHuman::getMass() {return mass;}
float ODEHuman::computeMass()
{
	mass=0;
	for(int i=0; i<joints.size(); i++)
	{
		mass += joints.get(i)->getMass();
	}
	return mass;
}

void ODEHuman::applyToSkel()
{
	for(int i=0;i<joints.size();i++)
	{
		joints.get(i)->applyToJoint(sk->joint(joints.get(i)->name()));
	}
	sk->root()->rot()->value(hips()->box->getOrientation());
	GsVec ps = hips()->getPosition();
	sk->root()->pos()->value(ps);
}

GsVec ODEHuman::getVelocity()
{
	GsVec COMVel;
	float curMass = 0;
	float totalMass = 0;
	for (int i=0; i <joints.size(); i++){
		curMass = joints.get(i)->getMass();
		totalMass += curMass;
		COMVel += joints.get(i)->getVelocity()*curMass;
	}

	COMVel /= totalMass;

	return COMVel;

}

ODEJoint* ODEHuman::joint(const char* name)
{
	for(int i=0;i<joints.size();i++)
	{
		if(strcmp(joints.get(i)->joint->name() ,name)==0)
		{
			return joints.get(i);
		}
	}
	//gsout<<"error joint "<<name<<" not found\n";
	return 0;
}
ODEJoint* ODEHuman::joint(int idx){ return joints.get(idx);}


int ODEHuman::getContactState(){return contact_controller->getContactState();}

void ODEHuman::update()
{
	computeCOM();


	setDesiredCOM(contact_controller->getContactCenter());
	//ik_controller->setRoot(contact_controller->getRootPos());

	contact_controller->evaluate();
	walk_controller->evaluate();
	//step_controller->evaluate();

	if(useComJ)velocity_controller->evaluate();
	ik_controller->evaluate();

	if(usePID)reference_controller->evaluate();
	if(useRoot)root_controller->evaluate();




	com_proj = com; com_proj.y = floor_height;
	com_d_proj = com_d; com_d_proj.y = floor_height;

	com_model->setPosition(com);
	com_d_model->setPosition(com_d);
	com_proj_model->setPosition(com_proj);
	com_d_proj_model->setPosition(com_d_proj);


	//spply the accumulated torque to the joint and set to zero for next round
	for(int i=0;i<joints.size();i++)
	{
		joints.get(i)->update();
	}
	//transfer joint rotation to internal KnSkeleton
	//applyToSkel();
}


void ODEHuman::setInitialPositionFromSkel()
{
	initialPos = POS("Hips");  pos = initialPos;
	for(int i=0;i<joints.size();i++) joints.get(i)->setPosition();
	setTotalVelocity(GsVec(0,0,0));
}

void ODEHuman::setTotalVelocity(GsVec velocity)
{
	for(int i=0;i<joints.size();i++) joints.get(i)->box->setVelocity(velocity);
}




GsVec ODEHuman::getPosition()
{
	const dReal* bodyPosition;
	bodyPosition = dBodyGetPosition(hips()->getBodyID());
	pos = GsVec(bodyPosition[0], bodyPosition[1],bodyPosition[2]); 
	return  pos;
}

GsVec ODEHuman::getCOM(){return com;}
GsVec ODEHuman::computeCOM()
{
	GsVec ncom;
	float totalMass = 0;
	int numBodies = joints.size();
	for(int i=0;i<numBodies;i++)
	{
		ODEObject* obj = joints.get(i)->box;
		totalMass += obj->getMass();
		ncom += obj->getMass()* obj->getPosition(); 
	}
	com = ncom/(totalMass);


	return com;
}


GsVec ODEHuman::getLeftFootPosition(){return joint("LeftFoot")->getCOMPosition();}
GsVec ODEHuman::getRightFootPosition(){return joint("RightFoot")->getCOMPosition();}



