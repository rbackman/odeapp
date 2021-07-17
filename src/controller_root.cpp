#include "controller_root.h"
#include "ode_human.h"
#include <gsim/gs_euler.h>
#include "ode_joint.h"
#include "pid.h"
#include "controller_contact.h"

RootController::RootController(ODEHuman* human):Controller(human)
{
		h = human;
		Kp=0.9f;
		Kd=0.3f;

		//rootPID = new PID(h->world,human->hips());
		//rootPID->setGlobal();
		//rootTransform = new SnTransform;
		//rootPID->attachTfm(rootTransform);
}

RootController::~RootController(void)
{
}

bool RootController::evaluate()
{
	bool valid = true;
	
	/*GsQuat q= h->hips()->box->getOrientation();
	GsVec t;
	gs_angles(gsXYZ,q,t.x,t.y,t.z);
	int contactState = h->getContactState();
	//t = 50*t;
	gsout<<"root torque: "<<t<<gsnl;
	if(contactState==LEFT_FOOT)
	{
		joint("LeftUpLeg")->addTorque(-t);
	}
	if(contactState==RIGHT_FOOT)
	{
		joint("RightUpLeg")->addTorque(-t);
	}*/

	computeHipTorques(h->hips()->getOrientation(),1.0,GsVec());

return valid;
}

#define SGN(x) x<0 ? -1 : 1;
GsVec RootController::computePDTorque(GsQuat qRel, GsQuat qRelD, GsVec wRel, GsVec wRelD){
	GsVec torque;
	

	// , ControlParams* cParams


	//the torque will have the form:
	// T = kp*D(qRelD, qRel) + kd * (wRelD - wRel)

	//Note: There can be problems computing the proper torque from the quaternion part, because q and -q 
	//represent the same orientation. To make sure that we get the correct answer, we'll take into account
	//the sign of the scalar part of qErr - both this and the v part will change signs in the same way if either 
	//or both of qRel and qRelD are negative
//	Quaternion qErr = qRel.getComplexConjugate() * qRelD;
	GsQuat qErr = qRel.conjugate();
	qErr = qErr * qRelD;

	GsVec qErrV = GsVec(qErr.x,qErr.y,qErr.z);
	float qErrs = qErr.w;
	//qErr.v also contains information regarding the axis of rotation and the angle (sin(theta)), but I want to scale it by theta instead
	float sinTheta = qErrV.len();
	if (sinTheta>1)
		sinTheta = 1;
	if (sinTheta<=0){
		//avoid the divide by close-to-zero. The orientations match, so the proportional component of the torque should be 0
	}else{
		float absAngle = 2.0f * (float)asin(sinTheta);
		torque = qErr.axis();
		torque *= 1.0f/sinTheta * (float)absAngle * (-Kp) * (float)SGN(qErrs);
//		torque = qErr.v/sinTheta * absAngle * (-cParams->kp) * SGN(qErr.s);
	}

	//qErr represents the rotation from the desired child frame to the actual child frame, which
	//means that the torque is now expressed in child coordinates. We need to express it in parent coordinates!
	torque = qRel.apply(torque);
	//the angular velocities are stored in parent coordinates, so it is ok to add this term now
	torque += (wRelD - wRel) * (-Kd);
	torque *= 1.0f; //cParams->strength;

	//now the torque is stored in parent coordinates - we need to scale it and apply torque limits
//	scaleAndLimitTorque(&torque, cParams, qRel.getComplexConjugate());

	//and we're done...
	
	return torque;
}
void RootController::computeHipTorques(GsQuat qRootD, float stanceHipToSwingHipRatio, GsVec ffRootTorque){
	//compute the total torques that should be applied to the root and swing hip, keeping in mind that
	//the desired orientations are expressed in the character frame
	if(!(h->getContactState()== CONTACT_LEFT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_LEFT||
		h->getContactState()==CONTACT_RIGHT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_RIGHT))
		return;

	GsVec rootTorque;
	GsVec swingHipTorque;
	float rootStrength = 1.0f;
	//this is the desired orientation in world coordinates
	GsQuat qRootDW;

//	if (SimGlobals::forceHeadingControl == false){
		//qRootD is specified in the character frame, so just maintain the current heading
//		qRootDW = characterFrame * qRootD;
//	}else{
		//qRootDW needs to also take into account the desired heading
	qRootDW = qRootD; //Quaternion::getRotationQuaternion(desiredHeading, PhysicsGlobals::up) * qRootD;
//	}

	ODEJoint* stanceHip;
	ODEJoint* swingHip;

	if(h->getContactState() == CONTACT_TRANSITION_TO_LEFT || h->getContactState() == CONTACT_TRANSITION_TO_RIGHT)
	{
		stanceHipToSwingHipRatio = h->contact_controller->getPhase();
	}

	if(h->getContactState()==CONTACT_LEFT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_LEFT)
	{
		swingHip = h->joint("RightUpLeg");
		stanceHip = h->joint("LeftUpLeg");
	}

	if(h->getContactState()==CONTACT_RIGHT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_RIGHT)
	{
		swingHip = h->joint("LeftUpLeg");
		stanceHip = h->joint("RightUpLeg");
	}

	if (rootStrength < 0)
		rootStrength = 0;
	if (rootStrength > 1)
		rootStrength = 1;


	//so this is the net torque that the root wants to see, in world coordinates
	rootTorque = computePDTorque(h->hips()->getOrientation(), qRootDW, h->hips()->getRotationalVelocity(), GsVec(0,0,0));

	rootTorque += ffRootTorque;

	
	swingHipTorque = swingHip->torque;

	//we need to compute the total torque that needs to be applied at the hips so that the final net torque on the root is rootTorque
	GsVec rootMakeupTorque;
	for (int i=0;i<h->numJoints();i++)
		if (h->joint(i)->parent == h->hips())
		{	

			rootMakeupTorque -= h->joint(i)->torque;
		}
	rootMakeupTorque -= rootTorque;

	//add to the root makeup torque the predictive torque as well (only consider the effect of the torque in the lateral plane).
	//Vector3d rootPredictiveTorque(0, 0, rootPredictiveTorqueScale*9.8*d.x);
	//rootMakeupTorque += characterFrame.rotate(rootPredictiveTorque);

	//assume the stance foot is in contact...
	GsVec stanceHipTorque;
	
	
	stanceHipTorque = stanceHip->torque;


	//now, based on the ratio of the forces the feet exert on the ground, and the ratio of the forces between the two feet, we will compute the forces that need to be applied
	//to the two hips, to make up the root makeup torque - which means the final torque the root sees is rootTorque!
	stanceHipTorque += rootMakeupTorque * stanceHipToSwingHipRatio * rootStrength;
	swingHipTorque += rootMakeupTorque * (1-stanceHipToSwingHipRatio) * rootStrength;



	if( stanceHipDamping > 0 ) 
	{
		GsVec wRel = h->hips()->getRotationalVelocity() - stanceHip->getRotationalVelocity();
		float wRelLen = wRel.len();
		if (wRelLen > stanceHipMaxVelocity ) wRel = wRel * (stanceHipMaxVelocity/wRelLen);
		stanceHipTorque -=  wRel * (stanceHipDamping * wRelLen);
	}

	//now transform the torque to child coordinates, apply torque limits and then change it back to world coordinates
/*
	GsQuat qStanceHip = stanceHip->getOrientation();
	stanceHipTorque = qStanceHip.conjugate().apply(stanceHipTorque);
	//limitTorque(&stanceHipTorque, &controlParams[stanceHipIndex]);
	stanceHipTorque = qStanceHip.apply(stanceHipTorque);

	Quaternion qSwingHip = character->getJoint(swingHipIndex)->getChild()->getOrientation();
	swingHipTorque = qSwingHip.getComplexConjugate().rotate(swingHipTorque);
	limitTorque(&swingHipTorque, &controlParams[swingHipIndex]);
	swingHipTorque = qSwingHip.rotate(swingHipTorque);
*/

	//and done...
	//gsout<<"stance torque: "<<stanceHipTorque<<gsnl;
	stanceHip->addTorque(stanceHipTorque);
	swingHip->addTorque(swingHipTorque);

}

