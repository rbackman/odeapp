#include "ode_joint.h"
#include "pid.h"

void ODEJoint::createHinge(dWorldID worldID ,dBodyID body1, dBodyID body2, GsVec jointPoint, int jtype)
{
	jid =  dJointCreateHinge(worldID, 0);
	dJointAttach(jid, body1, body2);
	dJointSetHingeAnchor(jid, jointPoint.x, jointPoint.y, jointPoint.z);

	switch(jtype)
	{

	case HINGE_X:	dJointSetHingeAxis(jid, 1, 0, 0);break;
	case HINGE_Y:	dJointSetHingeAxis(jid, 0, 1, 0);break;
	case HINGE_Z:	dJointSetHingeAxis(jid, 0, 0, 1);break;
	default: dJointSetHingeAxis(jid, 1, 0, 0); gsout<<"hinge not xyz\n";break;
	}

	/* not using limits right now
	KnJointEuler* e = j->euler();
	gsout<<"joint: "<<j->name()<<gsnl;
	gsout<<"low limits: "<<e->lolim(0)<<" "<<e->lolim(1)<<" "<<e->lolim(2)<<" " <<gsnl;
	gsout<<"high limits: "<<e->uplim(0)<<" "<<e->uplim(1)<<" "<<e->uplim(2)<<" " <<gsnl;

	if(!e->frozen(0) && e->frozen(1) && e->frozen(2))
	{
	dJointSetHingeAxis(jid, 1, 0, 0);
	if(e->limits(0))
	{	
	dJointSetHingeParam(jid, dParamLoStop, e->uplim(0));
	dJointSetHingeParam(jid, dParamHiStop, e->lolim(0));
	}
	}
	else if(e->frozen(0) && !e->frozen(1) && e->frozen(2))
	{
	dJointSetHingeAxis(jid, 0, 1, 0);
	if(e->limits(1))
	{	
	dJointSetHingeParam(jid, dParamLoStop, e->lolim(1));
	dJointSetHingeParam(jid, dParamHiStop, e->uplim(1));
	}
	}
	else if(e->frozen(0) && e->frozen(1) && !e->frozen(2))
	{
	dJointSetHingeAxis(jid, 0, 0, 1);
	if(e->limits(2))
	{	
	dJointSetHingeParam(jid, dParamLoStop, e->lolim(2) );
	dJointSetHingeParam(jid, dParamHiStop, e->uplim(2));
	}
	}
	else
	{
	dJointSetHingeAxis(jid, 1, 0, 0);
	if(e->limits(0))
	{	
	dJointSetHingeParam(jid, dParamLoStop, e->lolim(0));
	dJointSetHingeParam(jid, dParamHiStop, e->uplim(0));
	}
	gsout<<"Error making hinge, used x axis: "<< j->name() <<gsnl;
	gsout<<"Joint dof: "<<e->frozen(0) <<" "<<e->frozen(1)<< " "<< e->frozen(2)<<gsnl;
	}
	*/


}
void ODEJoint::createBall(dWorldID worldID, dBodyID body1, dBodyID body2,GsVec jointPoint, int jtype)
{
	jid = dJointCreateBall(worldID, 0);
	dJointAttach(jid, body1, body2);
	dJointSetBallAnchor(jid, jointPoint.x, jointPoint.y, jointPoint.z);

	m_jid = dJointCreateAMotor(worldID,0);
	dJointAttach(m_jid, body1, body2);

	//dAMotorUser	 The AMotor axes and joint angle settings are entirely controlled by the user. This is the default mode.
	//dAMotorEuler	 Euler angles are automatically computed. The axis a1 is also automatically computed. The AMotor axes must be set correctly when in this mode, as described below. When this mode is initially set the current relative orientations of the bodies will correspond to all euler angles at zero.
	dJointSetAMotorMode (m_jid,dAMotorUser);
//	dJointSetAMotorMode (m_jid,dAMotorEuler);
	dJointSetAMotorNumAxes(m_jid, 3);

	//dJointSetAMotorAxis (dJointID, int anum, int rel,  dReal x, dReal y, dReal z);
	//0: The axis is anchored to the global frame.
	//1: The axis is anchored to the first body.
	//2: The axis is anchored to the second body.

	dJointSetAMotorAxis(m_jid, 0, 1, 1, 0, 0);
	dJointSetAMotorAxis(m_jid, 1, 1, 0, 1, 0);
	dJointSetAMotorAxis(m_jid, 2, 1, 0, 0, 1);


	float FMax = 0.001f;
	float jStop = 0.2f;
/*	dJointSetAMotorParam(m_jid, dParamLoStop,  -jStop);
	dJointSetAMotorParam(m_jid, dParamHiStop,   jStop);
	dJointSetAMotorParam(m_jid, dParamFMax,     FMax);
	dJointSetAMotorParam(m_jid, dParamBounce,   0.0);
	dJointSetAMotorParam(m_jid, dParamVel,      0.0);
	dJointSetAMotorParam(m_jid, dParamLoStop2, -jStop);
	dJointSetAMotorParam(m_jid, dParamHiStop2,  jStop);
	dJointSetAMotorParam(m_jid, dParamFMax2,    FMax);
	dJointSetAMotorParam(m_jid, dParamBounce2,  0.0);
	dJointSetAMotorParam(m_jid, dParamVel2,     0.0);
*/

	/*KnJointEuler* e = j->euler();
	if(!e->frozen(0) && !e->frozen(1) && !e->frozen(2))
	{

	dJointSetBallParam(jid, dParamLoStop, e->lolim(0));
	dJointSetBallParam(jid, dParamHiStop, e->uplim(0));
	dJointSetBallParam(jid, dParamLoStop2, e->lolim(1));
	dJointSetBallParam(jid, dParamHiStop2, e->lolim(1));
	dJointSetBallParam(jid, dParamLoStop2, e->lolim(2));
	dJointSetBallParam(jid, dParamHiStop2, e->lolim(2));
	}

	else
	{
	dJointSetBallParam(jid, dParamLoStop, e->lolim(0));
	dJointSetBallParam(jid, dParamHiStop, e->uplim(0));
	dJointSetBallParam(jid, dParamLoStop2, e->lolim(1));
	dJointSetBallParam(jid, dParamHiStop2, e->lolim(1));
	dJointSetBallParam(jid, dParamLoStop2, e->lolim(2));
	dJointSetBallParam(jid, dParamHiStop2, e->lolim(2));
	gsout<<"Error making ball: "<< j->name() <<gsnl;
	gsout<<"Joint dof: "<<e->frozen(0) <<" "<<e->frozen(1)<< " "<< e->frozen(2)<<gsnl;
	}*/


}
void ODEJoint::createUniversal(dWorldID worldID, dBodyID body1, dBodyID body2,GsVec jointPoint, int jtype)
{
	jid = dJointCreateUniversal(worldID, 0);
	dJointAttach(jid, body1, body2);
	dJointSetUniversalAnchor(jid, jointPoint.x, jointPoint.y, jointPoint.z);
	//KnJointEuler* e = j->euler();
	//	gsout<<"joint: "<<j->name()<<gsnl;
	//	gsout<<"low limits: "<<e->lolim(0)<<" "<<e->lolim(1)<<" "<<e->lolim(2)<<" " <<gsnl;
	//	gsout<<"high limits: "<<e->uplim(0)<<" "<<e->uplim(1)<<" "<<e->uplim(2)<<" " <<gsnl;

	switch(jtype)
	{
	case UNI_XY:
		{
			dJointSetUniversalAxis1(jid, 1, 0, 0);
			dJointSetUniversalAxis2(jid, 0, 1, 0);
		}break;
	case UNI_XZ:
		{
			dJointSetUniversalAxis1(jid, 1, 0, 0);
			dJointSetUniversalAxis2(jid, 0, 0, 1);
		}break;
	case UNI_YZ:
		{
			dJointSetUniversalAxis1(jid, 0, 1, 0);
			dJointSetUniversalAxis2(jid, 0, 0, 1);
		}break;
	default: 
		{
			dJointSetUniversalAxis1(jid, 1, 0, 0);
			dJointSetUniversalAxis2(jid, 0, 1, 0);

			//	gsout<<"Error making universal, used x axis: "<< j->name() <<gsnl;
			//	gsout<<"Joint dof: "<<e->frozen(0) <<" "<<e->frozen(1)<< " "<< e->frozen(2)<<gsnl;
		}break;
	}

	/*
	switch(jtype)
	{
	case UNI_XY:
	{

	if(e->limits(0))
	{	
	dJointSetUniversalParam(jid, dParamLoStop, e->uplim(0));
	dJointSetUniversalParam(jid, dParamHiStop, e->lolim(0));
	}
	if(e->limits(1))
	{
	dJointSetUniversalParam(jid, dParamLoStop2, e->uplim(1));
	dJointSetUniversalParam(jid, dParamHiStop2, e->lolim(1));
	}
	}break;
	case UNI_XZ:
	{

	if(e->limits(0))
	{	
	dJointSetUniversalParam(jid, dParamLoStop, e->uplim(0));
	dJointSetUniversalParam(jid, dParamHiStop, e->lolim(0));
	}
	if(e->limits(2))
	{	
	dJointSetUniversalParam(jid, dParamLoStop2, e->uplim(2));
	dJointSetUniversalParam(jid, dParamHiStop2, e->lolim(2));
	}
	}break;
	case UNI_YZ:
	{

	if(e->limits(1))
	{	
	dJointSetUniversalParam(jid, dParamLoStop, e->lolim(1));
	dJointSetUniversalParam(jid, dParamHiStop, e->uplim(1));
	}
	if(e->limits(2))
	{	
	dJointSetUniversalParam(jid, dParamLoStop2, e->lolim(2));
	dJointSetUniversalParam(jid, dParamHiStop2, e->uplim(2));
	}
	}break;
	default: 
	{

	dJointSetUniversalParam(jid, dParamLoStop, e->lolim(0));
	dJointSetUniversalParam(jid, dParamHiStop, e->uplim(0));
	dJointSetUniversalParam(jid, dParamLoStop2, e->lolim(1));
	dJointSetUniversalParam(jid, dParamHiStop2, e->uplim(1));
	gsout<<"Error making universal, used x axis: "<< j->name() <<gsnl;
	gsout<<"Joint dof: "<<e->frozen(0) <<" "<<e->frozen(1)<< " "<< e->frozen(2)<<gsnl;
	}break;
	}*/

}


ODEJoint::ODEJoint(ODEHuman* hm, KnJoint* j, GsVec dim, ODEJoint* prnt, int jtype)
{
	
	h = hm;
		ax=ay=az=0;
		torqueSample = 0.0f;
		recentSampleT=false;
	jointType = jtype;
	world = h->world;
	parent = prnt;
	joint = j; 
	jointName = joint->name();
	joint->euler()->thaw();
	jid=0;
	m_jid = 0;
	//these will be set later by the ui
	
	grav = false;
	vforce = true;
	vfMult = GsVec(1,1,1);
	boxDim = dim;
	box = new ODEBox(world, true, jointCenter() , dim);
	ball = new Ball(joint->gcenter(),0.1f);
	ball->setVisible(true);

	boxOffset =  joint->gcenter() -  jointCenter() ;


	switch(jointType)
	{
	case BALL:
		createBall(world->GetWorldID(),prnt->getBodyID(),getBodyID(),joint->gcenter(),jtype);
		break;
	case UNI_XY:
	case UNI_XZ:
	case UNI_YZ:
		createUniversal(world->GetWorldID(),getBodyID(),prnt->getBodyID(),joint->gcenter(),jtype);
	break;
	case HINGE_X:
	case HINGE_Y:
	case HINGE_Z:
		createHinge(world->GetWorldID(),getBodyID(),prnt->getBodyID(),joint->gcenter(),jtype);
		break;
	default: jid = 0;
	}


}
ODEJoint::ODEJoint(ODEHuman* hm, KnJoint* j, GsVec pos, GsVec dim, ODEJoint* prnt, int jtype)
{
	h = hm;
	jointType = jtype;
			torqueSample = 0.0f;
			recentSampleT=false;
	ax=ay=az=0;
	world = h->world;
	parent = prnt;
	joint = j; 
	joint->euler()->thaw();
	jointName = joint->name();
	gcenter = joint->gcenter();
	boxOffset = gcenter - jointCenter();
	boxDim = dim;

	jid=0;
	m_jid = 0;
	//these will be set later by the ui
	
	grav = false;
	vforce = true;
	vfMult = GsVec(1,1,1);

	box = new ODEBox(world, true, pos , boxDim);
	ball = new Ball(gcenter,0.1f);
	ball->setVisible(true);




	switch(jointType)
	{
	case BALL:
		createBall(world->GetWorldID(),getBodyID(),prnt->getBodyID(),gcenter,jtype);
		break;
	case UNI_XY:
	case UNI_XZ:
	case UNI_YZ:
		createUniversal(world->GetWorldID(),getBodyID(),prnt->getBodyID(),gcenter,jtype);
		break;
	case HINGE_X:
	case HINGE_Y:
	case HINGE_Z:
		createHinge(world->GetWorldID(),getBodyID(),prnt->getBodyID(),gcenter,jtype);
		break;
	default: jid = 0;
	}


}

void ODEJoint::clearTorque(){torque = GsVec(0,0,0);}
void ODEJoint::applyToJoint(KnJoint* j)
{
	//this stopped working.. I think it needs to use quats 
	switch(jointType)
	{
	case BALL:
		//jid = createBall(wrld->GetWorldID(),getBodyID(),prnt->getBodyID(),joint,jtype);
		break;
	case UNI_XY:
	case UNI_XZ:
		j->euler()->value(0, dJointGetUniversalAngle1(jid) );
		j->euler()->value(1, dJointGetUniversalAngle2(jid) );
		break;
	case UNI_YZ:

		break;
	case HINGE_X:
		j->euler()->value(0,dJointGetHingeAngle(jid));
		break;
	case HINGE_Y:
		j->euler()->value(1,dJointGetHingeAngle(jid));
		break;
	case HINGE_Z:
		j->euler()->value(2,dJointGetHingeAngle(jid));
		break;

	}
}


GsVec ODEJoint::getCOMPosition()
{
	return box->getPosition();
}
GsVec ODEJoint::getPosition()
{
	GsVec p;
	/*if(parent)
	{
	p= boxOffset;
	GsQuat rot = parent->joint->rot()->value();
	p = parent->joint->gcenter() + rot.apply(p);
	}
	else
	{*/
	p = boxOffset;
	GsQuat rot = box->getOrientation();
	p = box->getPosition() + rot.apply(p);
	//}

	return p;
}


void ODEJoint::update()
{

switch(jointType)
	{
	case BALL:
		//jid = createBall(wrld->GetWorldID(),getBodyID(),prnt->getBodyID(),joint,jtype);
		ax = dJointGetAMotorAngle(m_jid,0);
		ay = dJointGetAMotorAngle(m_jid,1);
		az = dJointGetAMotorAngle(m_jid,2);
		
		break;
	case UNI_XY:
	case UNI_XZ:
		ax = dJointGetUniversalAngle1(jid) ;
		az = dJointGetUniversalAngle2(jid) ;
		
		break;
	case UNI_YZ:

		break;
	case HINGE_X:
		ax = dJointGetHingeAngle(jid);

		break;
	case HINGE_Y:
		ay = dJointGetHingeAngle(jid);
		break;
	case HINGE_Z:
		az = dJointGetHingeAngle(jid);
	break;
}


	ball->setPosition(getPosition());


	
	if(recentSampleT)
		torque += sampleTorque;
	else
		sampleTorque.set(0,0,0);
	
	recentSampleT=false;


	switch(jointType)
	{
	case BALL:
		//jid = createBall(wrld->GetWorldID(),getBodyID(),prnt->getBodyID(),joint,jtype);
		dJointAddAMotorTorques(m_jid,torque.x, torque.y,torque.z);
		break;
	case UNI_XY:
		dJointAddUniversalTorques(jid, torque.x, torque.y);
	case UNI_XZ:
		dJointAddUniversalTorques(jid, torque.x, torque.z);
		break;
	case UNI_YZ:
		dJointAddUniversalTorques(jid, torque.y, torque.z);
		break;
	case HINGE_X:
		dJointAddHingeTorque(jid,torque.x);
		break;
	case HINGE_Y:
		dJointAddHingeTorque(jid,torque.y);
		break;
	case HINGE_Z:
		dJointAddHingeTorque(jid,torque.y);
		break;

	}

	torque = GsVec(0,0,0);
	box->update();

}


ODEJoint::~ODEJoint(void)
{
	delete box;
	if(jid)dJointDestroy(jid);
}

