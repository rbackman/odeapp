#include "controller_step.h"
#include "curve.h"
#include "ode_joint.h"
#include "models.h"
#include "controller_IK.h"

static void stepManipCB ( SnManipulator* mnp,const GsEvent& ev, void* udata )
 {
	
	StepController* step = (StepController*)udata;
	step->makeCurves();
	GsVec p = mnp->translation();
 }
void StepController::makeCurves()
	{
		//make the trajectory curves
		rightFootTraj->clear();
		leftFootTraj->clear();

	leftFootStart	= h->getLeftFootPosition();
	leftFootEnd		=	leftStepManip->translation();
	leftFootEnd.y = leftFootStart.y;
	rightFootStart	= h->getRightFootPosition();
	rightFootEnd	= rightStepManip->translation();
	rightFootEnd.y = rightFootStart.y;

	rightFootTraj->addPoint ( rightFootStart );
	rightFootTraj->addPoint	( rightFootStart    +	GsVec(0,stepHeight,0) );
	rightFootTraj->addPoint	( rightFootEnd		+	GsVec(0,stepHeight,0) );
	rightFootTraj->addPoint	( rightFootEnd );

	leftFootTraj->addPoint ( leftFootStart );
	leftFootTraj->addPoint	( leftFootStart		+		GsVec(0,stepHeight,0) );
	leftFootTraj->addPoint	( leftFootEnd	    +	GsVec(0,stepHeight,0) );
	leftFootTraj->addPoint	( leftFootEnd );

	}
StepController::StepController(ODEHuman* human):Controller(human)
{
	leftFootTraj = new Curve; 
	rightFootTraj = new Curve; 

	deactivate(); //dont activate until make step is called;
	stepHeight = 0.5f;
	GsModel* m = new GsModel;
	GsBox box;
	 float size = 0.3f;
	  box.set(GsVec(-size,-size*0.7f-0.2f,-2*size),GsVec(size,size*0.2f-0.2f,2*size));
	  m->make_box(box);

	leftStepManip = new SnManipulator;
	leftStepManip->child(new SnModel(m));
	GsVec leftStepPos = h->joint("LeftFoot")->getPosition();// + GsVec(0,0,1);
	
	leftStepManip->translation(leftStepPos);
	leftStepManip->callback(stepManipCB,this);
	grp->add(leftStepManip);

	rightStepManip = new SnManipulator;
	rightStepManip->child(new SnModel(m));
	GsVec rightStepPos = h->joint("RightFoot")->getPosition();// + GsVec(0,0,1);


	rightStepManip->translation(rightStepPos);
	rightStepManip->callback(stepManipCB,this);
	grp->add(rightStepManip);



	grp->add(leftFootTraj->_line);
	grp->add(rightFootTraj->_line);

	speed = 0.01f;
	
	visible(true);
	

}
GsVec StepController::getIPStepPos(int foot)
{

	GsVec step;
	GsVec comPos = h->getCOM();
	GsVec footPos;
	
	if(foot==RIGHT_FOOT) footPos = h->joint("RightFoot")->getCOMPosition();
	else footPos = h->joint("LeftFoot")->getCOMPosition();


	GsVec v = h->getVelocity();
	float h = fabs(comPos.y - footPos.y);
	step.x = v.x * sqrt(h/9.8f + v.x * v.x / (4*9.8f*9.8f)) * 1.3f;
	step.z = v.z * sqrt(h/9.8f + v.z * v.z / (4*9.8f*9.8f)) * 1.1f;	
	step.y = 0;
	//relative to the foot joint
	return step;

}
void StepController::makeStep(int step)
{
	activate();
	visible(true);
	startFoot = step;

	if(step == STEP_LEFT_FOOT)
	{
		setState(STEP_START_MOVING_LEFT);
	}
	else if(step == STEP_RIGHT_FOOT)
	{
		setState(STEP_START_MOVING_RIGHT);
	}

	

}
GsVec StepController::getRightFootPosition()
{
	return leftFootTraj->eval_bezier(tLeft);
}
GsVec StepController::getRightFootPosition(float t)
{
	return leftFootTraj->eval_bezier(t);
}
GsVec StepController::getLeftFootPosition()
{
	return rightFootTraj->eval_bezier(tRight);
}
GsVec StepController::getLeftFootPosition(float t)
{
	return rightFootTraj->eval_bezier(t);
}
StepController::~StepController(void)
{
	delete leftFootTraj;
	delete rightFootTraj;
}

bool StepController::evaluate()
{
	bool valid = Controller::evaluate();
	
	if(isActive())
	{
		switch(getState())
		{
		case CONTROLLER_IDLE:
			gsout<<"done stepping, step controller deactivated\n";
			deactivate();
		break;
		case STEP_RIGHT_FOOT:
		{
			tRight+=0.01f;
			if(tRight>1)
			{
				tRight=1;
				if(startFoot==STEP_RIGHT_FOOT)setState(STEP_START_MOVING_LEFT);
				else setState(CONTROLLER_IDLE);
			}
			h->ik_controller->setRight(getRightFootPosition());
		}break;

		case STEP_LEFT_FOOT:
		{
			tLeft+=0.01f;
			if(tLeft>1)
			{
				tLeft=1;
				if(startFoot==STEP_LEFT_FOOT)setState(STEP_START_MOVING_RIGHT);
				else setState(CONTROLLER_IDLE);
			}
			h->ik_controller->setLeft(getLeftFootPosition());
		}break;

		case STEP_START_MOVING_RIGHT:
		{
			tRight=0;
			setState(STEP_RIGHT_FOOT);
			h->ik_controller->setLeftVisible(false);
			h->ik_controller->setRightVisible(true);
		}break;
		case STEP_START_MOVING_LEFT:
		{
			tLeft = 0;
			setState(STEP_LEFT_FOOT);
			h->ik_controller->setRightVisible(false);
			h->ik_controller->setLeftVisible(true);
		}break;
		case STEP_START_WEIGHT_LEFT:

		break;
		case STEP_START_WEIGHT_RIGHT:

		break;
		case STEP_DONE_MOVING_LEFT:

			break;
		case STEP_DONE_MOVING_RIGHT:

			break;
		default:
			gsout<<"warning bad state for step controller "<<getState()<<gsnl;
		}
	}
	

	//rightFootTraj->clear();
	//leftFootTraj->clear();
	//if(h->balanced())
	//{
	//	return true;
	//}
	//else
	//{
		/*
		switch(h->getContactState())
		{
		case LEFT_FOOT: 
			{
				grp->visible(true);
				GsVec stepStart = joint("RightFoot")->getCOMPosition();
				stepStart.y=0;
				GsVec step = getIPStepPos(RIGHT_FOOT);
				float len = GsVec(step - stepStart).len();
				rightStepTrans->get().setrans(stepStart + step);
			
				rightFootTraj->addPoint(stepStart);
				rightFootTraj->addPoint(stepStart + GsVec(0,len*0.5f,0));
				rightFootTraj->addPoint(stepStart + step + GsVec(0,len*0.5f,0));
				rightFootTraj->addPoint(stepStart + step);

			}
			break;
		case RIGHT_FOOT: 
			{
				grp->visible(true);
				GsVec stepStart = joint("LeftFoot")->getCOMPosition();
				stepStart.y=0;
				GsVec step = getIPStepPos(LEFT_FOOT);
				float len = GsVec(step - stepStart).len();
				leftStepTrans->get().setrans(stepStart + step);
				leftFootTraj->addPoint(stepStart);
				leftFootTraj->addPoint(stepStart + GsVec(0,len*0.5f,0));
				leftFootTraj->addPoint(stepStart +step + GsVec(0,len*0.5f,0));
				leftFootTraj->addPoint(stepStart +step);
			}
		break;
		case BOTH_FEET: 
			{
				/*
				GsVec stancePos = joint("LeftFoot")->getPosition();
				stancePos.y=0;
				GsVec step = getIPStepPos(RIGHT_FOOT);
				float len = GsVec(step - stancePos).len();
				rightStepTrans->get().setrans(stancePos +step);
			
				rightFootTraj->addPoint(stancePos);
				rightFootTraj->addPoint(stancePos + GsVec(0,len*0.5f,0));
				rightFootTraj->addPoint(stancePos +step + GsVec(0,len*0.5f,0));
				rightFootTraj->addPoint(stancePos +step);


				stancePos = joint("RightFoot")->getPosition();
				stancePos.y=0;
				step = getIPStepPos(LEFT_FOOT);
				len = GsVec(step - stancePos).len();
				leftStepTrans->get().setrans(stancePos +step);
			
				leftFootTraj->addPoint(stancePos);
				leftFootTraj->addPoint(stancePos + GsVec(0,len*0.5f,0));
				leftFootTraj->addPoint(stancePos +step + GsVec(0,len*0.5f,0));
				leftFootTraj->addPoint(stancePos +step);
				
			}
			break;
		//}
	}*/
return valid;
}


