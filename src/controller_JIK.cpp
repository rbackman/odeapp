#include "controller_JIK.h"

#include "common.h"


static void IkManipCb ( SnManipulator* mnp,const GsEvent& ev, void* udata )
{
	JIKController* ik_cont = ( JIKController*)udata;
	GsVec p = mnp->translation();

}
static void rootManipCB ( SnManipulator* mnp,const GsEvent& ev, void* udata )
{
	JIKController* ik_cont = ( JIKController*)udata;
	GsVec p = mnp->translation();

}

GsMatn tr2diff(GsMat hand,GsMat goal)
{
	GsVec handP,goalP;
	hand.getrans(handP);
	goal.getrans(goalP);
	GsVec dp = goalP - handP;
	GsMatn diff(1,6);
	diff(0,0)=dp.x;
	diff(0,1)=dp.y;
	diff(0,2)=dp.z;

	GsVec rx = cross((GsVec)&hand.e11,(GsVec)&goal.e11);
	GsVec ry = cross((GsVec)&hand.e21,(GsVec)&goal.e21);
	GsVec rz = cross((GsVec)&hand.e31,(GsVec)&goal.e31);
	GsVec dr = rx+ry+rz;
	dr = dr*0.5f;

	diff(0,3)=dr.x;
	diff(0,4)=dr.y;
	diff(0,5)=dr.z;
	diff.transpose();
	return diff;
}


/*
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
*/

JIKController::JIKController(ODEHuman* human):Controller(human)
{
	makeIK();
}


JIKController::~JIKController(void)
{

}

void JIKController::setRoot(GsVec p)
{

}
void JIKController::setLeft(GsVec p)
{

}
void JIKController::setRight(GsVec p)
{

}

/*
void JIKController::setHeight(float ht)
{
refskel->root()->pos()->value(1,ht);
refskel->update_global_matrices();
_kns->update();
updateIK();
//float comHeight =  0.15 + refskel->root()->pos()->value().y - refskel->joint("LeftFoot")->gcenter().y;
//refskel->root()->pos()->value(1,comHeight);

}
void JIKController::setRightY(float y)
{
IkManipulator* ik = get_manipulator(_ikGroup,3);
GsVec p = ik->manip()->translation();
p.y = y;
if(ik->visible())ik->manip()->translation(p);
}
void JIKController::setLeftY(float y)
{
IkManipulator* ik = get_manipulator(_ikGroup,2);
GsVec p = ik->manip()->translation();
p.y = y;
if(ik->visible())ik->manip()->translation(p);
}
*/



void JIKController::solveIK(SnManipulator* manip,ODEJoint* endJoint, ODEJoint* baseJoint)
{

	GsVec endP = endJoint->getPosition();
	GsVec baseP = baseJoint->getPosition();

	float length = 0;
	ODEJoint* curJ = endJoint;
	while(curJ != baseJoint)
	{
		GsVec dj =curJ->parent->getPosition() - curJ->getPosition();
		curJ = curJ->parent;
		length += dj.len();
	}

	GsVec goalP = manip->translation();
	GsVec dist = goalP - baseJoint->getPosition();
	if(dist.len()>length)
	{
		dist.len(length-0.1f);
		goalP = baseJoint->getPosition()+dist;
	}

	GsVec dend = goalP - endJoint->getPosition();

	if(dend.len()<0.1)
	{
		gsout<<"got to goal \n";
		return;
	}

	n=7;
	m=6;
	J.resize(m,n);

	int i=0;
	while(curJ != baseJoint->parent)
	{
		GsVec p = curJ->getPosition();


		if(curJ->axis.x==1)
		{
			GsVec ax = GsVec(1,0,0);
			curJ->getOrientation().apply(ax);
			GsVec crsX = cross(ax,endP-p);
			J.set(0,i,crsX.x);
			J.set(1,i,crsX.y);
			J.set(2,i,crsX.z);
			J.set(3,i,ax.x);
			J.set(4,i,ax.y);
			J.set(5,i,ax.z);
			i++;
		}
		if(curJ->axis.y==1)
		{
			GsVec ay = GsVec(0,1,0);
			curJ->getOrientation().apply(ay);
			GsVec crsY = cross(ay,endP-p);
			J.set(0,i,crsY.x);
			J.set(1,i,crsY.y);
			J.set(2,i,crsY.z);
			J.set(3,i,ay.x);
			J.set(4,i,ay.y);
			J.set(5,i,ay.z);
			i++;
		}
		if(curJ->axis.z==1)
		{
			GsVec az = GsVec(0,0,1);
			curJ->getOrientation().apply(az);
			GsVec crsZ = cross(az,endP-p);
			J.set(0,i,crsZ.x);
			J.set(1,i,crsZ.y);
			J.set(2,i,crsZ.z);
			J.set(3,i,az.x);
			J.set(4,i,az.y);
			J.set(5,i,az.z);
			i++;
		}
		curJ = curJ->parent;
	}

	GsMat leftFootMat;
	quat2mat(endJoint->getOrientation(),leftFootMat);
	leftFootMat.translation(endJoint->getPosition());
	
	eff = tr2diff(leftFootMat,manip->mat());
	eff*= (float)0.5f;

	Jt = J;
	Jt.transpose();
	float dampC = 0.5f;

	JJt.mult(J,Jt);
	Idt.resize(n,n);
	Idt.identity();
	Idt*=dampC;
	JJTPG.add(JJt,Idt);
	if(!inverse(JJTPG,JJt_inv)){gsout<<"couldnt invert\n";return;}				 
	j_plus.mult(Jt,JJt_inv);
	//gsout<<"\n\nJPseudoI:\n"<<j_plus<<gsnl<<gsnl;
	theta.mult(j_plus,eff);


}

bool JIKController::evaluate()
{

	/*

	falloff = 5;
	int method = DAMPED;
	avoidCol = false;
	closestJ = 0;
	closestD = 1000;
	maxV = 0.5;

	s = h->translation(); 
	if(s.len()>LENGTH)
	{
	s.len(LENGTH);
	}

	endJ = App->sk->joint("LeftHand")->gcenter(); 

	dEnd = s - endJ;
	if(App->mainwin->ui_stopSolve->value())
	{
	if(dEnd.len()<App->mainwin->ui_tol->value())
	{
	//gsout<<"got to target"<<gsnl;
	return;
	}
	}
	dEnd.len((float)App->mainwin->ui_dEnd->value());

	for(int i=0;i<n;i++)
	{
	GsVec p = jnts[i].j->gcenter();

	GsVec axs;
	if(jnts[i].dof==0) axs = &jnts[i].j->gmat().e11;
	else if(jnts[i].dof==1) axs = &jnts[i].j->gmat().e21;
	else if(jnts[i].dof==2) axs = &jnts[i].j->gmat().e31;
	//p.z = 0;
	GsVec crsX = cross(axs,s-p);


	J.set(0,i,crsX.x);
	J.set(1,i,crsX.y);
	J.set(2,i,crsX.z);
	J.set(3,i,axs.x);
	J.set(4,i,axs.y);
	J.set(5,i,axs.z);



	}

	eff = tr2diff(App->sk->joint("LeftHand")->gmat(),App->JIKmanip->mat());
	eff*= (float)App->mainwin->ui_dEnd->value();


	Jt = J;
	Jt.transpose();

	switch(method)

	{
	case PSEUDO:
	{
	JJt.mult(J,Jt);


	if(!inverse(JJt,JJt_inv)){gsout<<"couldnt invert\n";return;}				 
	j_plus.mult(Jt,JJt_inv);
	theta.mult(j_plus,eff);
	if(debug)
	{
	gsout<<"Jt= \n"<<Jt;
	gsout<<"J= \n"<<J;
	gsout<<"J rows:"<<J.lin()<<" col: "<<J.col()<<gsnl;
	gsout<<"JJt rows:"<<JJt.lin()<<" col: "<<JJt.col()<<gsnl;
	gsout<<"JJt:\n \n"<<JJt<<gsnl<<gsnl;;
	gsout<<"JJt_inv: \n"<<JJt_inv<<gsnl<<gsnl;		
	}
	}	break;	
	case TRANSPOSE:
	{
	avoidCol=false;
	theta.mult(Jt,eff);

	}break;
	case DAMPED:
	{
	float dampC = (float)App->mainwin->ui_dampC->value();
	dampC = dampC*dampC;

	JJt.mult(J,Jt);
	Idt.resize(n,n);
	Idt.identity();
	Idt*=dampC;

	JJTPG.add(JJt,Idt);


	if(!inverse(JJTPG,JJt_inv)){gsout<<"couldnt invert\n";return;}				 
	j_plus.mult(Jt,JJt_inv);
	//gsout<<"\n\nJPseudoI:\n"<<j_plus<<gsnl<<gsnl;
	theta.mult(j_plus,eff);
	}break;


	}




	GsVec og = App->sk->joint("Hips")->gcenter();
	GsVec dg = App->JIKmanip->translation() - App->sk->joint("LeftHand")->gcenter();
	dg.len((float)App->mainwin->ui_JtransVel->value());
	GsVec newR = og+dg;
	if(newR.x>5)newR.x=5;
	if(newR.x<-5)newR.x=-5;
	if(newR.y>App->maxHipHeight)newR.y=App->maxHipHeight;
	if(newR.y<30)newR.y=30;
	if(newR.z>5)newR.z=5;
	if(newR.z<-5)newR.z=-5;

	if(App->mainwin->ui_hipsJacobian->value())App->sk->joint("Hips")->pos()->value(newR);
	//App->sk->joint("Hips")->pos()->value(og.x+theta.get(0,0),og.y+theta.get(0,1),og.z+theta.get(0,2));

	for(int i=0;i<n;i++)
	{

	float prevAng = jnts[i].j->euler()->value(jnts[i].dof);
	float newAng;

	newAng= prevAng + (float)theta.get(0,i);

	if(jnts[i].j->euler()->inlimits(jnts[i].dof,newAng))
	jnts[i].j->euler()->value(jnts[i].dof,newAng);


	}


	//	App->sk->update_global_matrices();
	//	App->mainwin->ui_viewer->_knsc->update();
	App->mainwin->ui_viewer->redraw();

	*/


	return true;
}


void JIKController::matchIK()
{
	rootManip->translation(h->joint("Spine1")->getPosition());
	leftManip->translation(h->joint("LeftFoot")->getCOMPosition());
	rightManip->translation(h->joint("RightFoot")->getCOMPosition());
}
void JIKController::makeIK()
{

	rootManip = new SnManipulator;

	GsModel* m = new GsModel();
	m->make_sphere(GsVec(0,0,0),0.5,10,1);
	rootManip->child(new SnModel(m));
	rootManip->callback(rootManipCB,this);

	rootManip->visible(true);

	grp->add(rootManip);

	GsModel* mbox = new GsModel;
	GsBox box;
	float size = 0.32f;
	box.set(GsVec(-size,-size*0.7f,-1.8f*size),GsVec(size,size*0.2f,1.8f*size));
	mbox->make_box(box);

	leftManip =  new SnManipulator;
	leftManip->child(new SnModel(mbox));
	grp->add(leftManip);

	rightManip =  new SnManipulator;
	rightManip->child(new SnModel(mbox));
	grp->add(rightManip);

	matchIK();

}

