#pragma once
#include "controller.h"

#include "common.h"
#include <gsim/gs_matn.h>

//this controller is responsible for solving the analytical ik to achieve the root and foot positions as close as possible 
//

class IkBody;
class KnScene;

class JIKController : public Controller
{
public:
	JIKController(ODEHuman* human);
	~JIKController(void);
	bool evaluate();
	void setRoot(GsVec p);
	void setLeft(GsVec p);
	void setRight(GsVec p);
	void solveIK(SnManipulator* manip,ODEJoint* endJoint, ODEJoint* baseJoint);
	/*
	void setRightY(float y);
	void setLeftY(float y);
	void setHeight(float h);
	void setRightVisible(bool vis);
	void setLeftVisible(bool vis);
	void setRootVisible(bool vis);
	*/

	void matchIK();

	
	
private:
	GsMatn J,Jt,JJt,JJTPG,JJt_inv,j_plus, Idt;
	GsMatn eff,theta;
	int n,m;

	bool manual;
	GsVec rootStart;
	GsVec rootOffset;
	
	SnManipulator* rootManip;
	SnManipulator* leftManip;
	SnManipulator* rightManip;


	void makeIK();


//stuff
	
};
