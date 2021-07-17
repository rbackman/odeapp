# pragma once



//# include <gsim/fl_skeleton_win.h>

#include "common.h"


#include "controller.h"

class AppMainWin;
class ODEWorld;
class ODEHuman;
class ODEObject;
class ODESphere;
class AppFileManager;


// this class contains pointers to the main
// classes in the application, so that they
// can be accessed from anywhere in the code
class AppMain
 { public :
    AppMain ();
    AppMainWin* mainwin;
	AppFileManager* files;

	ODEWorld* world; 
	ODEHuman* human;
	ODEObject* ground;
	KnSkeleton* skref;
	KnSkeleton* sk;
	GsVec rootStart;
	GsVec rootOffset;
	GsArray<ODEObject*> objects;

	bool skelVis;
	bool shaded;
	bool odeRunning;
	void initScene();
	void setupOde();
	void updateODE();

	void dropBalls();

	
 };

// there is only one instance of AppMain, which
// is allocated in app_main.cpp and which can be
// accessed with the pointer below:
extern AppMain* App; 

