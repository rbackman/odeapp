#include "controller_velocity.h"
#include "ode_human.h"
#include "ode_joint.h"
#include "controller_contact.h"

#include <gsim/kn_skeleton.h>
#include "common.h"

VelocityController::VelocityController(ODEHuman* human):Controller(human)
{
	h=human;
	velDSagittal = 0;
	velDCoronal = 0;
		jkP = 1;
	jkD = 1;
}

VelocityController::~VelocityController(void)
{

}
void VelocityController::setVelocity(float sag,float side)
{
	velDSagittal = sag;
	velDCoronal = side;
}


GsVec VelocityController::getVirtualForce()
{
	//this is the desired acceleration of the center of mass
//	GsVec d = h->getCOM() - h->getStanceCenter();
	GsVec d = h->getCOM() - h->getDesiredCOM();
	GsVec v = h->getVelocity();

	GsVec desA;
	double comOffsetCoronal = 0;

	float magnify = 1.0f;
	//if(h->getContactState()==CONTACT_LEFT_FOOT || h->getContactState()==CONTACT_RIGHT_FOOT)
	//	magnify = 1.6f;

	float erSagP = d.z * jkP*magnify;
	float erSagV = (v.z - velDSagittal) * jkD*magnify;
	float erCorP = d.x * jkPc*magnify;
	float erCorV = (v.x - velDCoronal) * jkPc*magnify;
	//gsout<<"error Sag P:"<<erSagP<<"\t erSagV: "<<erSagV<<gsnl;
	//gsout<<"error Cor P:"<<erCorP<<"\t erSagV: "<<erCorV<<gsnl;
	desA.z =  erSagP + erSagV ;
	desA.x =  erCorV + erCorP;
	desA.y = d.y * jkP  - v.y * jkD; //dampen the y component of velocity
	/*if (doubleStanceMode == true){
		GsVec errV = characterFrame.inverseRotate(doubleStanceCOMError*-1);
		desA.x = (-errV.x + comOffsetCoronal) * 20 + (velDCoronal - v.x) * 9;
		desA.z = (-errV.z + comOffsetSagittal) * 10 + (velDSagittal - v.z) * 150;
	}*/

	//and this is the force that would achieve that - make sure it's not too large...
	GsVec fA = (desA) * h->getMass(); 
	fA.x = boundToRange(fA.x, -200, 200);
	fA.z = boundToRange(fA.z, -200, 200);
	fA.y = boundToRange(fA.y, -200, 200);
	//fA.y = 0;
	//now change this quantity to world coordinates...
	//	fA = characterFrame.rotate(fA);
//	fA = h->getRot().apply(fA);
	fA = h->getRot().conjugate().apply(fA);
	return fA;

}

GsVec VelocityController::forceContribution(ODEJoint* start, ODEJoint* end, GsVec pos)
{
GsVec f;
ODEJoint* currentJoint = start->parent;
bool looking = true;
while(looking)
{
	f += (currentJoint->getCOMPosition() - pos)*currentJoint->getMass();
	if(currentJoint->parent == 0 || currentJoint->parent == end)looking = false;
	else currentJoint = currentJoint->parent;
}
f /= h->getMass();
return f;
}
void VelocityController::FixFeet()
{
	if(h->getContactState()==CONTACT_LEFT_FOOT || h->getContactState()==CONTACT_BOTH_FEET)
	{
		ODEJoint* lf = h->joint("LeftFoot");
		
		lf->torque.z = boundToRange(lf->torque.z,-100,100);
		lf->torque.x = boundToRange(lf->torque.x,-100,100);
		if(lf->Rotating() ) 
		{
			
			//gsout<<"left Foot Fixed\n";
			lf->clearTorque();
		}
	}
	if(h->getContactState()==CONTACT_RIGHT_FOOT || h->getContactState()==CONTACT_BOTH_FEET)
	{
		ODEJoint* rf = h->joint("RightFoot");
		rf->torque.z = boundToRange(rf->torque.z,-100,100);
		rf->torque.x = boundToRange(rf->torque.x,-100,100);
		
		if(rf->Rotating() ) 
		{
			rf->clearTorque();
			//gsout<<"right Foot Fixed\n";
		}
	}
}
GsVec VelocityController::compTorque(ODEJoint* j, GsVec force)
{
	GsVec t;
	if(j->vforce)
	{
		t = forceContribution(j,0,j->getPosition());
		//t += forceContribution(joint("Shoulder"),0,j->getPosition());
		t = cross(t,force);
		t = vecMult(t,j->vfMult); //this is so the joint has a chance to chance to modify the torque before its applied
		j->addSampleTorque(t);
	}
	return t;

}
void VelocityController::computeBalanceTorques(){
//applying a force at the COM induces the force f. The equivalent torques are given by the J' * f, where J' is
// dp/dq, where p is the COM.
	
	GsVec force = getVirtualForce();
	
	GsVec t;
	ODEJoint* j;
	
	if( h->getContactState() != CONTACT_RIGHT_FOOT && h->getContactState() != CONTACT_FLYING )
		{
			if(!h->joint("LeftFoot")->Rotating() && h->contact_controller->leftFootPlanted())
			{
				
				j = h->joint("LeftFoot");
				compTorque(j,force);
			}
			else
			{
				//gsout<<"left foot rotation\n";
			}
			j = h->joint("LeftLeg");
			compTorque(j,force);

			j = h->joint("LeftUpLeg");
			GsVec t = compTorque(j,force);
		}
			
	if(h->getContactState() != CONTACT_LEFT_FOOT && h->getContactState() != CONTACT_FLYING )
		{
		//calculate leg torques one joint at a time each call to forceContribution recursively adds 
		//to the torque accumulator that finally gets applied to the joint. 
		
			if(!h->joint("RightFoot")->Rotating() && h->contact_controller->rightFootPlanted())
			{
				j = h->joint("RightFoot");
				compTorque(j,force);
			}
			else
			{
				//gsout<<"right foot rotation\n";
			}

		j = h->joint("RightLeg");
		compTorque(j,force);


		j = h->joint("RightUpLeg");
		compTorque(j,force);

	}

	
//use the swing foot to help balance.	
	if(h->getContactState() == CONTACT_LEFT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_LEFT)
{
	j = h->joint("RightUpLeg");
	t = forceContribution(j,0,j->getPosition());
	GsVec ft = cross(t,force);
	//ft.z=-ft.z*5;
	j->addTorque(ft);

	j = h->joint("RightLeg");
	t = forceContribution(j,0,j->getPosition());
	ft = cross(t,force);
//	ft.z=-ft.z;
	j->addTorque(ft);

}

if(h->getContactState() == CONTACT_RIGHT_FOOT || h->getContactState() == CONTACT_TRANSITION_TO_RIGHT)
{
	j = h->joint("LeftUpLeg");
	t = forceContribution(j,0,j->getPosition());
	GsVec ft = cross(t,force);
	//ft.z=-ft.z*5;
	j->addTorque(ft);

	j = h->joint("LeftLeg");
	t = forceContribution(j,0,j->getPosition());
	ft = cross(t,force);
	//ft.z=-ft.z;
	j->addTorque(ft);

}


	if(h->getContactState()!=CONTACT_FLYING)
	{
		/// one joint at a time
		j = h->joint("Spine2");
		t = forceContribution(j,0,h->hips()->getPosition());
		if(j->vforce)j->addTorque(vecMult(cross(t,force),j->vfMult));

		j = h->joint("Shoulder");
		t = forceContribution(j,h->joint("Spine1"),h->joint("Spine1")->getPosition());
		if(j->vforce)j->addTorque(vecMult(cross(t,force),j->vfMult));
	}
	
}




bool VelocityController::evaluate()
{
	
	computeBalanceTorques();
//FixFeet();
return true;
}