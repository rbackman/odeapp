#include "controller_walk.h"
#include "controller_ik.h"
#include "controller_contact.h"
#include "controller.h"
#include "controller_reference.h"

#include "app_main.h"


WalkController::WalkController(ODEHuman* human):Controller(human)
{
	
	
	

	
	speed = 0.01f;
	goalSpeed = 0.01f;


	comTraj=0;
	
	/*
	walkMotion = new KnMotion;
	if(walkMotion->load("../data/ManMotion.skm"))
	{
		walkMotion->connect(App->skref);
		walkFrame = 0;
	}
	else
	{
		gsout<<"couldn't load motion";
	}
	*/

	stepHeight = 0.2f;
	stepSpeed = 0.03f;
	transitionSpeed= 0.05f;;
	
	setState(WALK_IDLE);
	walking = false;
	headingArrow = new Arrow(human->getCOM(),GsVec(0,0,1));
	grp->add(headingArrow->getGrp());
	headingArrow->setVisible(false);
	headingArrow->setColor(1,0,0);
	
}


WalkController::~WalkController(void)
{

}

void WalkController::setDirection(float val) // val from -1 to 1
{
	angle = val*gspidiv2;
	headingArrow->rotate(0,angle,0);
}
void WalkController::setSpeed(float cal)  // val from -1 to 1
{
	goalSpeed = cal;
}
void WalkController::startWalking()
{
	activate();

	//headingArrow->setVisible(true);
	headingArrow->setColor(0,1,0);
	
	//idleToState(WALK_START,0.5);
	setState(WALK_START);
	walking = true;

	
	leftStart =  h->ik_controller->leftFootIKPos();
	rightStart = h->ik_controller->rightFootIKPos();

}
void WalkController::stopWalking()
{
	headingArrow->setVisible(false);
	headingArrow->setColor(1,1,0);
	goToState(WALK_STOP);

}
bool WalkController::evaluate()
{
	bool valid = true;


	/*
	if(walking)
	{
		walkFrame++;
		if(walkFrame>=240)walkFrame=0;
		walkMotion->apply_frame(walkFrame);
	}
	*/

switch(getState())
	{
	case WALK_START:
		{
		
	
			setState(WALK_START_WEIGHT_LEFT);
		}
		break;


	case WALK_START_WEIGHT_LEFT:
		{
		
		setState(WALK_WEIGHT_LEFT);
		h->contact_controller->goToContact(CONTACT_LEFT_FOOT);
		stepPhase=0;
		}break;


	case WALK_WEIGHT_LEFT:
		{
	
		if(h->contact_controller->leftFootPlanted() && h->contact_controller->leftFootContains(h->getCOM()))
		//	stepPhase+=transitionSpeed;
		//if(stepPhase>=1.0f)	
		{
			setState(WALK_START_STEP_RIGHT);
		}
		}break;
	case WALK_START_STEP_RIGHT: 
		{
		//h->step_controller->makeStep(STEP_RIGHT_FOOT);
		stepPhase = 0;
		setState(WALK_STEP_RIGHT); 
		}break;

	case WALK_STEP_RIGHT: 
		{
			stepPhase+=stepSpeed;
			if(stepPhase>=1.0f)
			{
				stepPhase = 1.0f;
				setState(WALK_START_WEIGHT_RIGHT); 
			}
			else
				h->ik_controller->setRight(GsVec(rightStart.x,rightStart.y+bump(stepPhase)*stepHeight,rightStart.z));
				//h->ik_controller->setRight(h->step_controller->getRightFootPosition(stepPhase) );
		
			
		
		}break;
	case WALK_START_WEIGHT_RIGHT: 
		{
	
		stepPhase =0;
		setState(WALK_WEIGHT_RIGHT);
		h->contact_controller->goToContact(CONTACT_RIGHT_FOOT);

		}break;


	case WALK_WEIGHT_RIGHT: 
		if(h->contact_controller->rightFootPlanted() && h->contact_controller->rightFootContains(h->getCOM()))
		//stepPhase+=transitionSpeed;
		//if(stepPhase>=1.0f)
		{
				setState(WALK_START_STEP_LEFT);
		}
		break;
	case WALK_START_STEP_LEFT: 
		{
			stepPhase = 0;
			setState(WALK_STEP_LEFT); 
		}
		break;
	case WALK_STEP_LEFT: 
		{
		stepPhase+=stepSpeed;
			if(stepPhase>=1.0f)
				setState(WALK_START_WEIGHT_LEFT); 
			else
				h->ik_controller->setLeft(GsVec(leftStart.x,leftStart.y+bump(stepPhase)*stepHeight,leftStart.z));
//				h->ik_controller->setLeft(h->step_controller->getLeftFootPosition(stepPhase));
		}break;
	case WALK_STOP:
		{
			/*lastComPos = curComPos;
			nextComPos = (h->getLeftFootPosition()+h->getRightFootPosition())/2;
			nextComPos.y = curComPos.y;
			*/

			setState(WALK_STOPPING);
		//	h->setDesiredCOM(curComPos); 
		}break;

	case WALK_STOPPING:
		{
			//curComPos = interp(lastComPos,nextComPos,getPhase());
			setNextState(WALK_IDLE);
			//h->setDesiredCOM(curComPos);
		} break;
	case WALK_IDLE:
	default:
		{
			headingArrow->setColor(1,0,0);
			walking = false;
			deactivate();
		} break;
	}

	return valid;
}


