#include "controller_IK.h"
#include "ik_manipulator.h"
#include "ik_body.h"

#include "app_main.h"
#include "app_main_win.h"


//ik manipulator callbacks
static void IkManipCb ( IkManipulator* ikm, void* udata )
{
	IKController* ik_cont = (IKController*)udata;

	App->mainwin->setUIFromSkel();
	//if(app->human)app->human->applyToBodies();
}
static void rootManipCB ( SnManipulator* mnp,const GsEvent& ev, void* udata )
{
	IKController* ik_cont = ( IKController*)udata;
	GsVec p = mnp->translation();
	//float x,y,z;
	//gs_angles_xyz(mnp->mat(),x,y,z,'c');

	ik_cont->setSkel(p);
	ik_cont->evaluate();
	App->mainwin->setUIFromSkel();

}

//IK Controller class
IKController::IKController(ODEHuman* human):Controller(human)
{
	ikb=0;
}

bool IKController::evaluate()
{
	bool valid = Controller::evaluate();

	updateIK();

	return valid;
}

void IKController::updateIK()
{
	/*if(h->getContactState()==CONTACT_LEFT_FOOT) setRightVisible(true);
	else setRightVisible(false);

	if(h->getContactState()==CONTACT_RIGHT_FOOT) setLeftVisible(true);
	else setLeftVisible(false);
	*/

	if(ikb)update_manipulators (_ikGroup );
	_kns->update();
}



void IKController::setRightVisible(bool vis)
{
	get_manipulator(_ikGroup,3)->visible(vis);
}
void IKController::setLeftVisible(bool vis)
{
	get_manipulator(_ikGroup,2)->visible(vis);
}
void IKController::setRootVisible(bool vos)
{
	rootManip->visible(false);
}

void IKController::setScene(KnScene* _kn)
{
	_kns = _kn;
	refskel = _kn->skeleton();
	makeIK();
}


void IKController::setSkel(GsVec p)
{
	refskel->root()->pos()->value(p);
	refskel->update_global_matrices();
	_kns->update();
}
void IKController::setRoot(GsVec p)
{
	p.y = refskel->root()->pos()->value(1);
	setSkel(p);
	updateIK();
}
void IKController::setHeight(float ht)
{

	refskel->root()->pos()->value(1,ht);
	refskel->update_global_matrices();
	_kns->update();
	updateIK();
	//float comHeight =  0.15 + refskel->root()->pos()->value().y - refskel->joint("LeftFoot")->gcenter().y;
	//refskel->root()->pos()->value(1,comHeight);

}
void IKController::setRightY(float y)
{
	IkManipulator* ik = get_manipulator(_ikGroup,3);
	GsVec p = ik->manip()->translation();
	p.y = y;
	if(ik->visible())ik->manip()->translation(p);
}
void IKController::setLeftY(float y)
{
	IkManipulator* ik = get_manipulator(_ikGroup,2);
	GsVec p = ik->manip()->translation();
	p.y = y;
	if(ik->visible())ik->manip()->translation(p);
}
void IKController::setLeft(GsVec p)
{
	IkManipulator* ik = get_manipulator(_ikGroup,2);
	ik->manip()->translation(p);
}
void IKController::setRight(GsVec p)
{
	IkManipulator* ik = get_manipulator(_ikGroup,3);
	ik->manip()->translation(p);
}


GsVec IKController::leftFootIKPos()
{
	IkManipulator* ik = get_manipulator(_ikGroup,2);
	return ik->manip()->translation();
}
GsVec IKController::rightFootIKPos()
{
	IkManipulator* ik = get_manipulator(_ikGroup,3);
	return ik->manip()->translation();
}

void IKController::matchIK(){match_manipulators(_ikGroup);}
void IKController::makeIK()
{
	refskel->init_values();
	refskel->root()->pos()->value(0,START_HEIGHT,0);
	rootManip = new SnManipulator();
	_ikGroup = new SnGroup;
	grp->add(_ikGroup);

	GsModel* m = new GsModel();
	m->make_sphere(GsVec(0,0,0),0.5,10,1);
	rootManip->child(new SnModel(m));
	rootManip->translation(refskel->root()->gcenter());
	rootManip->callback(rootManipCB,this);

	//grp->add(rootManip);

	ikb=new IkBody;
	ikb->init(refskel,"LeftHand RightHand LeftFoot RightFoot");

	add_manipulators (_ikGroup, ikb, _kns , IkManipCb, this, 0 );
	ikb->add_lines (_ikGroup );
	update_manipulators ( _ikGroup );
	rootStart  = refskel->root()->gcenter();
	rootOffset = GsVec(0,(float)App->mainwin->ui_walk_hheight->value(),0);
	rootManip->translation(rootStart+rootOffset);
	refskel->root()->pos()->value(rootStart+rootOffset);
	updateIK();

	setRightVisible(true);
	setLeftVisible(true);
}

void IKController::resetSkel()
{
	refskel->init_values();
	setRoot(rootStart);
	matchIK();
	setRoot(rootStart+rootOffset);
	updateIK();
}
IKController::~IKController(void)
{
	_ikGroup->remove_all();

}