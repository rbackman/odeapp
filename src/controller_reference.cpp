
#include "controller_reference.h"
#include "pid.h"

ReferenceController::ReferenceController(ODEHuman* human) : Controller(human)
{
refSkel = 0;
}
void ReferenceController::attachRefSkel(KnSkeleton* s)
{
	if(refSkel)refSkel->unref();
	refSkel = s;
	refSkel->ref();
	
	//useBubbleT = false;
	
	for(int i=0;i<h->numJoints();i++)
	{
		ODEJoint* j = h->joint(i);

		if(j->usePID)
		{
			if(refSkel)
			{
				KnJoint* knJoint = refSkel->joint(j->name());
				GsVec rot = GsVec(knJoint->euler()->value() );
				j->setAngles(rot);
			}

			pids.push(new PID( j ));
		}
	}
}


ReferenceController::~ReferenceController()
{
	refSkel->unref();
	pids.remove(0,pids.size());
}
PID* ReferenceController::pid(const char* name)
{
	for(int i=0;i<pids.size();i++)
	{
		if(strcmp(name,pids.get(i)->getJoint()->name())==0)
			return pids.get(i);
	}
	return 0;
}

/*
void ReferenceController::bubbleUpTorques()
{
	for (int i= h->numJoints()-1; i>=0; i--)
	{
		ODEJoint* j = h->joint(i);

		if (j != h->joint("LeftUpLeg") && j != h->joint("RightUpLeg")&&j != h->joint("LeftFoot") && j != h->joint("RightFoot"))
			if (j->parent && j->parent != h->hips())
				j->parent->addTorque(j->torque);
	}
}
*/
bool ReferenceController::evaluate()
{
	for(int i=0;i<pids.size();i++)
	{
		ODEJoint* joint = pids.get(i)->getJoint();
		if(joint->usePID)
		{
			if(refSkel)
			{
				KnJoint* knJoint = refSkel->joint(joint->name());
				float rx,ry,rz;
				gs_angles(gsXYZ,knJoint->quat()->value(),rx,ry,rz);
				joint->setAngles(GsVec(rx,ry,rz));
			}
			pids.get(i)->evaluate();
		}
	}
//	if(useBubbleT)bubbleUpTorques();

	return true;
}
void ReferenceController::applyToRefSkel()
{
	for(int i=0;i<h->sk->joints().size();i++)
	{
		refSkel->joints().get(i)->rot()->value(h->sk->joints().get(i)->rot()->value());
	}
	
	refSkel->root()->pos()->value(h->sk->root()->pos()->value());
	refSkel->update_global_matrices();
}
