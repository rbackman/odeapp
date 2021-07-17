#pragma once
#include "controller.h"

//this controller is responsible for solving the analytical ik to achieve the root and foot positions as close as possible 
//

class IkBody;
class KnScene;

class IKController : public Controller
{
public:
	IKController(ODEHuman* human);
	~IKController(void);
	bool evaluate();
	void setRoot(GsVec p);
	void setSkel(GsVec p); //use setRoot if you want to move the manipulator..
	void setLeft(GsVec p);
	void setRight(GsVec p);
	void setRightY(float y);
	void setLeftY(float y);
	void setScene(KnScene* _kn);
	void matchIK();
	void resetSkel();
	void setHeight(float h);
	void setRightVisible(bool vis);
	void setLeftVisible(bool vis);
	void setRootVisible(bool vis);
	GsVec leftFootIKPos();
	GsVec rightFootIKPos();
private:
	bool manual;
	GsVec rootStart;
	GsVec rootOffset;
	KnSkeleton* refskel;
	KnScene* _kns;
	SnGroup* _ikGroup;
	IkBody* ikb;
	SnManipulator* rootManip;

	void makeIK();
	void updateIK();

//stuff
	
};
