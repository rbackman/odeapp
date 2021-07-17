#include "controller_contact.h"
#include "controller_ik.h"
#include "controller_jik.h"
#include "controller_step.h"
#include "models.h"




ContactController::ContactController(ODEHuman* human):Controller(human)
{
	overSample = 0.8f;
	contactState = CONTACT_HEELS;
	setState(CONTACT_FLYING);
	contactTransition = 0;
	contactTransitionSpeed = 0.05f;
	rootTransitionSpeed = TIME_STEP*0.5f;

	isBalanced = false;
	contact_model = new Ball(human->getCOM(),0.1f); contact_model->setVisible(true);
	contactPos = (h->getLeftFootPosition()+h->getRightFootPosition())/2.0f ;
	contactPos.y=0;
	lastContactPos = h->getCOM();
	lastContactPos.y=0;
	nextContactPos = contactPos;

	rootPos = h->sk->joint("Hips")->gcenter();
	nextRootPos = rootPos;
	lastRootPos = rootPos;
	rootMix = 0.6f;


	grp->add(contact_model->getGrp());
	_lines = new SnGroup;
	grp->add(_lines);
	_contactLines = new SnGroup;
	grp->add(_contactLines);


	//I just got these positions from printing them out probably not that accurate
	//can be caluclated with the foot offset and dimensions
	float lr = 0.3f;
	float f = 0.9f;
	float b = -0.2f;
	float y = -0.15f;

	sensors[LFL] = new VirtualSensor( h->joint("LeftFoot"),GsVec(-lr,y,f) );
	sensors[LFR] = new VirtualSensor( h->joint("LeftFoot"),GsVec(lr,y,f));
	sensors[LBL] = new VirtualSensor( h->joint("LeftFoot"),GsVec(-lr,y,b));
	sensors[LBR] = new VirtualSensor( h->joint("LeftFoot"),GsVec(lr,y,b));
	sensors[RFL] = new VirtualSensor( h->joint("RightFoot"),GsVec(-lr,y,f));
	sensors[RFR] = new VirtualSensor( h->joint("RightFoot"),GsVec(lr,y,f));
	sensors[RBL] = new VirtualSensor( h->joint("RightFoot"),GsVec(-lr,y,b));
	sensors[RBR] = new VirtualSensor( h->joint("RightFoot"),GsVec(lr,y,b));


}


bool ContactController::leftFootPlanted()
{
	for(int i=0;i<4;i++)
	{
		if(!sensors[i]->isActive())
		{
			h->joint("LeftFoot")->box->setColor(GsColor(1,0,0));
				return false;
		}
	}
	h->joint("LeftFoot")->box->setColor(GsColor(0,0,1));
	return true;
}
bool ContactController::rightFootPlanted()
{
	for(int i=4;i<8;i++)
	{
		if(!sensors[i]->isActive())
		{
			h->joint("RightFoot")->box->setColor(GsColor(1,0,0));
			return false;
		}
	}
	h->joint("RightFoot")->box->setColor(GsColor(0,0,1));
	return true;
}
bool ContactController::bothFeetPlanted()
{
	return leftFootPlanted() && rightFootPlanted();
}
ContactController::~ContactController(void)
{

}
bool ContactController::leftFootContains(GsVec p)
{

	GsVec np;
	GsPolygon pol;
	GsPolygon c_poly;
	GsModel* lf = h->joint("LeftFoot")->box->model->getModel();


	for (int i=0; i<lf->V.size(); i++ )
	{ 
		np = lf->V[i]*h->joint("LeftFoot")->box->model->getTfm()->get();
		pol.push ( GsVec2(np.x, np.z) );  // project to floor
	}

	pol.convex_hull ( c_poly );  // get convex hull


	if(c_poly.contains( GsPnt2(p.x,p.z) )) 
		return true;

	return false;

}
bool ContactController::rightFootContains(GsVec p)
{
	GsVec np;
	GsPolygon pol;
	GsPolygon c_poly;
	GsModel* rf = h->joint("RightFoot")->box->model->getModel();


	for (int i=0; i<rf->V.size(); i++ )
	{ 
		np = rf->V[i]*h->joint("RightFoot")->box->model->getTfm()->get();
		pol.push ( GsVec2(np.x, np.z) );  // project to floor
	}

	pol.convex_hull ( c_poly );  // get convex hull


	if(c_poly.contains( GsPnt2(p.x,p.z) )) 
		return true;

	return false;

}
bool ContactController::bothFeetContain(GsVec p)
{
	GsVec np;
	GsPolygon pol;
	GsPolygon c_poly;

	GsModel* lf = h->joint("LeftFoot")->box->model->getModel();
	GsModel* rf = h->joint("RightFoot")->box->model->getModel();

	for (int i=0; i<lf->V.size(); i++ )
	{ 
		p = lf->V[i]*h->joint("LeftFoot")->box->model->getTfm()->get();
		pol.push ( GsVec2(p.x, p.z) );  // project to floor
	}


	for (int i=0; i<rf->V.size(); i++ )
	{ 
		p = rf->V[i]*h->joint("RightFoot")->box->model->getTfm()->get() ;
		pol.push ( GsVec2(p.x, p.z) );  // project to floor
	}

	pol.convex_hull ( c_poly );  // get convex hull

	if(contact_poly.contains( GsPnt2(p.x,p.z) )) 
		return true;

	return false;

}

void ContactController::updateContactRegion()
{
	if(getState()== CONTACT_FLYING )return;

	_lines->remove_all();
	int i;
	GsPnt p;

	GsPolygon pol;
	GsModel* lf = h->joint("LeftFoot")->box->model->getModel();
	GsModel* rf = h->joint("LeftFoot")->box->model->getModel();
	if(getState() != CONTACT_RIGHT_FOOT)
	{
		for ( i=0; i<lf->V.size(); i++ )
		{ 
			p = lf->V[i]*h->joint("LeftFoot")->box->model->getTfm()->get();
			pol.push ( GsVec2(p.x, p.z) );  // project to floor
		}
	}

	if(getState() != CONTACT_LEFT_FOOT)
	{
		for ( i=0; i<rf->V.size(); i++ )
		{ 
			p = rf->V[i]*h->joint("RightFoot")->box->model->getTfm()->get() ;
			pol.push ( GsVec2(p.x, p.z) );  // project to floor
		}
	}
	pol.convex_hull ( contact_poly );  // get convex hull



	SnLines* line = new SnLines();
	line->color(GsColor(0,255,0));
	line->begin_polyline ();
	for(int j=0;j<contact_poly.size();j++)
	{
		line->push(contact_poly.get(j).x,0,contact_poly.get(j).y);
	}
	line->push(contact_poly.get(0).x,0,contact_poly.get(0).y);
	line->end_polyline ();
	_lines->add(line);
	GsVec com = h->getCOM();

	if(contact_poly.contains( GsPnt2(com.x,com.z) )) isBalanced = true;
	else isBalanced=false;

}

bool ContactController::balanced()
{
	if(getState()==CONTACT_FLYING)return false;
	return isBalanced;
}
GsVec ContactController::updateContactCenter()
{
	//if transitioning interpolate the last and next contact centers
	if(contactTransitioning)
	{
		if(contactTransition>1.0f)
		{contactTransition=1.0; contactTransitioning=false;}
		contactPos = interp( lastContactPos,nextContactPos,contactTransition);
	}
	return contactPos;
}
GsVec ContactController::updateRootCenter()
{

	if(rootTransitioning)
	{
		if(rootTransition >= 1)
		{
			rootTransitioning = false;
		}

		rootPos = interp( lastRootPos,nextRootPos,rootTransition);
		rootPos.y = h->getHipHeight();
	}
	return rootPos;
}
int ContactController::checkContactState()
{ 
	//this function is intended to determine the contact state from the character
	//it checks if the foot com is the right height from the floor and if they are rotating
	//for now it only checks for when the character lands the first time
	ODEJoint* leftFoot = h->joint("LeftFoot");
	ODEJoint* rightFoot = h->joint("RightFoot");

	GsVec lf = leftFoot->getCOMPosition();
	GsVec rf = rightFoot->getCOMPosition();
	float ly = lf.y;
	float ry = rf.y;
	float lymin = leftFoot->box->GetYDimension()/2;
	float rymin = rightFoot->box->GetYDimension()/2;
	if(ly<lymin && ry<rymin) 
	{
		if(!leftFoot->Rotating() && !rightFoot->Rotating() && h->getContactState() == CONTACT_FLYING)
		{
			gsout<<"Landed\n";
			goToContact(CONTACT_BOTH_FEET);
			h->step_controller->makeCurves();
			h->jik_controller->matchIK();
		}
	}

	/*
	else if(ly<lymin) 
	{
	if(contactState!=LEFT_FOOT)
	{
	goToContact(LEFT_FOOT);
	gsout<<"Left Leg support"<<gsnl;
	}
	}
	else if(ry<rymin)
	{
	if(contactState!=RIGHT_FOOT)
	{
	goToContact(RIGHT_FOOT);
	gsout<<"Right Leg support"<<gsnl;
	}

	}
	else 
	{
	if(contactState!=FLYING)
	{	
	goToContact(FLYING);
	gsout<<"FLYING"<<gsnl;
	}

	}*/
	//contactState = BOTH_FEET;
	return getState(); //BOTH_FEET;
	//return contactState;

}

void ContactController::goToContact(int nextContact)
{
	lastContactPos = contactPos;
	switch(nextContact)
	{
	case CONTACT_LEFT_FOOT:
		{
			if(leftFootContains(h->getCOM()))
				return;

			goToState(CONTACT_TRANSITION_TO_LEFT);
			nextContactPos = h->getLeftFootPosition();// + GsVec(-0.1f,0,-0.05f);
			nextRootPos = lerp(lastContactPos,nextContactPos,rootMix);
			nextRootPos.y = h->getHipHeight();
		}break;
	case CONTACT_RIGHT_FOOT:
		{
			if(rightFootContains(h->getCOM()))
				return;

			goToState(CONTACT_TRANSITION_TO_RIGHT);
			nextContactPos = h->getRightFootPosition();// + GsVec(0.1f,0,-0.05f);
			nextRootPos = lerp(lastContactPos,nextContactPos,rootMix);
			nextRootPos.y = h->getHipHeight();
		}break;
	case CONTACT_BOTH_FEET:
		{
			goToState(CONTACT_TRANSITION_TO_BOTH);
			nextContactPos = (h->getLeftFootPosition()+h->getRightFootPosition())/2.0f; //+ GsVec(0,0,-0.05f);
			nextRootPos = nextContactPos;
			nextRootPos.y = h->getHipHeight();

		}break;
	case CONTACT_FLYING: //hopefully nobody asks the character to go to the flying state ;-)
		{
			goToState(CONTACT_TRANSITION_TO_FLYING);
			nextContactPos = (h->getLeftFootPosition()+h->getRightFootPosition())/2.0f;//+ GsVec(0,0,-0.05f);
			lastContactPos = nextContactPos;
			nextRootPos = lerp(lastContactPos,nextContactPos,rootMix);
			nextRootPos.y = h->getHipHeight();
		}break;
	default:
		{
			gsout<<"Contact Controller: goToContact while in transition\n";
		}
	}
	nextContactPos.y=0;
	contactTransitioning = true;
	rootTransitioning = true;
	contactTransition = 0;
	rootTransition=0;
}
int ContactController::getContactState()
{
	return getState();
}
bool ContactController::evaluate()
{
	bool valid = Controller::evaluate();
	//automatically check what the contact state is when flying otherwise it is user determined






	for(int i=0;i<h->world->contactPoints.size();i++)
	{

		ODEContact* cont = h->world->contactPoints.get(i);

		for(int i=0;i<8;i++)
		{
			sensors[i]->checkCollision(cont);
		}

	}


	for(int i=0;i<8;i++)
	{
		sensors[i]->update();
	}

	_contactLines->remove_all();
	for(int i=0;i<8;i++)
	{
		if(sensors[i]->getVal().len()>1)
		{
			SnLines* line = new SnLines();
			line->color(GsColor(0,255,0));
			line->begin_polyline();

			line->push(sensors[i]->getPos());
			line->push(sensors[i]->getPos() +sensors[i]->getVal()*0.01f);

			line->end_polyline ();
			_contactLines->add(line);

			SnLines* line2 = new SnLines();
			line2->color(GsColor(0,255,0));
			line2->begin_polyline();

			line2->push(sensors[i]->getPos());

		
			line2->push(sensors[i]->getPos() - sensors[i]->getVal()*0.01f);

			line2->end_polyline ();
			_contactLines->add(line2);
		}

	}

	if(contactTransitioning)
	{
		contactTransition+=contactTransitionSpeed;
		if(contactTransition>1.0f) 
		{
			contactTransitioning = false;
		}
	}
	if(rootTransitioning)
	{
		rootTransition+=rootTransitionSpeed;
		if(rootTransition>1.0f) rootTransitioning = false;
	}	

	switch(getState())
	{

	case CONTACT_LEFT_FOOT:

		break;
	case  CONTACT_TRANSITION_TO_LEFT:
		if(!rootTransitioning)setState(CONTACT_LEFT_FOOT);
		break;
	case CONTACT_RIGHT_FOOT:

		break;
	case CONTACT_TRANSITION_TO_RIGHT:
		if(!rootTransitioning)setState(CONTACT_RIGHT_FOOT);
		break;
	case CONTACT_BOTH_FEET:

		break;
	case CONTACT_TRANSITION_TO_BOTH:
		if(!rootTransitioning)setState(CONTACT_BOTH_FEET);
		break;
	case CONTACT_FLYING:
		lastContactPos = contactPos;
		nextContactPos = contactPos;
		checkContactState(); 
		break;
	case CONTACT_TRANSITION_TO_FLYING:
		//you want to jump?
		break;
	default:
		gsout<<"Warning bad state "<<getState()<<gsnl;

	}
	checkContactState(); 
	updateContactCenter();
	updateRootCenter();
	updateContactRegion();
	contact_model->setPosition(contactPos);

	return valid;
}


