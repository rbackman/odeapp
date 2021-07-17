



# include "app_main.h"
# include "app_main_win.h"
#include "ode_world.h"
#include "ode_human.h"
#include "ode_sphere.h"
#include "ode_box.h"

#include "controller_root.h"
#include "controller_velocity.h"
#include "controller_reference.h"
#include "controller_gravity.h"
#include "controller_step.h"
#include "controller_walk.h"
#include "controller_ik.h"
#include "pid.h"
#include "curve.h"
#include "app_file_manager.h"


//# define GS_USE_TRACE1  // keyword tracing
# include <gsim/gs_trace.h>


# define CHKVAR(s) if(_vars->search(s)<0) { fltk::alert("Missing parameter [%s]!",s); exit(0); }
AppMain* App; // the only instance of AppMain


AppMain::AppMain ()
 {
	 world = 0;
	 human = 0;
	 ground = 0;
	skelVis = 0;



	 mainwin = new AppMainWin();
	 files = new AppFileManager();
   
	
	
   sk=new KnSkeleton;
	bool ok = sk->load("../data/Man.s");
   if ( !ok ) gsout.fatal("Could not load skeleton file!");
   sk->ref();
	sk->joint("Hips")->pos()->value(0,START_HEIGHT,0);
	sk->update_global_matrices();

	skref = new KnSkeleton;
	ok = skref->load("../data/ManRef.s");
   if ( !ok ) gsout.fatal("Could not load skeleton file!");
   	skref->ref();
   skref->root()->pos()->value(10,START_HEIGHT,0);
  
   skref->update_global_matrices();


	
	mainwin->ui_viewer->_kns->connect(sk);
	mainwin->ui_viewer->_kns->set_visibility(0,0,0,0);
	mainwin->ui_viewer->_kns->set_skeleton_radius(0.05f);

	mainwin->ui_viewer->_knsref->connect(skref);
	mainwin->ui_viewer->_knsref->set_visibility(skelVis,0,0,0);
	mainwin->ui_viewer->_knsref->set_skeleton_radius(0.05f);


	odeRunning = true;


	/*
	skwin = new FlSkeletonWin;
	skwin->viewer ( mainwin->ui_viewer );
   skwin->add ( sk );

   skwin->show();*/

 }



void AppMain::dropBalls()
{
	GsVec pp = human->getCOM();

for(float i=-1;i<1;i++)
	{
		for(float j=3;j<6;j++)
		{
			GsVec rp = GsVec(pp.x - 0.5f + i*1.5f+gs_random()*0.5f,8+j*1.5f, pp.z + gs_random()*2-1);
			ODESphere*	sphere = new ODESphere(world,true,rp,0.5);
			objects.push(sphere);
			mainwin->ui_viewer->_root->add(sphere->model->getGrp());
		}
	}
}

void AppMain::setupOde()
{

    if(!world) world = new ODEWorld(GRAVITY);

	if(human)
	{ 
		delete human; human = 0;
	}	

	
	human = new ODEHuman(world,sk); 

	human->makeControllers();

	skref->root()->pos()->value(1,10);
	human->attachRefSkel(mainwin->ui_viewer->_knsref); 
	


	skref->joint("LeftUpArm")->euler()->value(0,GS_TORAD(20));
	skref->joint("LeftUpArm")->euler()->value(2,GS_TORAD(5));

	skref->joint("RightUpArm")->euler()->value(0,GS_TORAD(20));
	skref->joint("RightUpArm")->euler()->value(2,GS_TORAD(-5));
	skref->joint("LeftArm")->euler()->value(0,GS_TORAD(-60));
	skref->joint("RightArm")->euler()->value(0,GS_TORAD(-60));
	
	human->setHipHeight((float)mainwin->ui_walk_hheight->value());

	mainwin->ui_viewer->_root->add(human->grp);
	
	

	if(!ground) 
	{
		ground = new ODEBox(world, false, GsVec(0,-1,0), GsVec(20, 2, 20) );
		ground->setColor(GsColor(0.5f,0.5f,0.5f));
		mainwin->ui_viewer->_root->add(ground->model->getGrp());
	}

}
void AppMain::initScene()
{
	setupOde();

	mainwin->show();
	mainwin->ui_viewer->view_all();
	mainwin->ui_viewer->redraw();
	files->loadDefault();
    files->loadCurrentState();
}

void AppMain::updateODE()
	{
		if(odeRunning)
		{
			if(human->walk_controller->isActive())
			{
				if(human->step_controller->isActive())
				{
					if(human->walk_controller->getState() == WALK_STEP_RIGHT) mainwin->stance_message("Step Right");
					if(human->walk_controller->getState() == WALK_STEP_LEFT) mainwin->stance_message("Step Left");
				}
				else if(human->walk_controller->getState() == WALK_WEIGHT_LEFT)
				mainwin->stance_message("Weight Left");
				else if(human->walk_controller->getState() == WALK_WEIGHT_RIGHT)
				mainwin->stance_message("Weight RIGHT");
				mainwin->message("walk phase: %g",human->walk_controller->getPhase());
				
			}

			human->update();
			switch(human->getContactState())
			{
				case CONTACT_BOTH_FEET: mainwin->stance_message("Both Feet");break;
				case CONTACT_LEFT_FOOT: mainwin->stance_message("Left Foot");break;
				case CONTACT_RIGHT_FOOT: mainwin->stance_message("Right Foot");break;
				case CONTACT_FLYING: mainwin->stance_message("Flying");break;
			}

			for(int i=0;i<objects.size();i++)
			{
				objects.get(i)->update();
			}
			world->ODELoop(TIME_STEP);
		}
	}
int main ( int argc, char** argv )
 {
	// Init the application:
	App = new AppMain;

	// Setup parameters and show the window:

	App->initScene();
	   // Enter fltk loop:
	  // fltk::run();
	while(true)
	{
	//	float deltaTime = App->timer.GetElapsedSeconds();
		App->updateODE();
		fltk::check();
		App->mainwin->ui_viewer->redraw();
	}
   return 0;
 }


