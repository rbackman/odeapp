
# include <gsim/fl.h>
# include <gsim/fl_vars_win.h>
#include <gsim/gs_euler.h>

# include "app_main_win.h"
#include "controller.h"
#include "ode_human.h"
#include "ode_joint.h"
#include "pid.h"
#include "controller_root.h"
#include "controller_velocity.h"
#include "controller_reference.h"
#include "controller_walk.h"
#include "controller_contact.h"
#include "controller_gravity.h"
#include "controller_ik.h"
#include "app_file_manager.h"

# define CHKVAR(s) if(_vars->search(s)<0) { fltk::alert("Missing parameter [%s]!",s); exit(0); }

AppMainWin::AppMainWin ()
 {
   _vtwin = new FlVarsWin;
   ui_viewer->init ( this );		
 }

AppMainWin::~AppMainWin ()
 {
 }

void AppMainWin::refreshScene()
{
	App->shaded = ui_shaded->value();
	 if( App->shaded )
				 	ui_viewer->cmd(FlViewer::CmdAsIs);
	 else
				 	ui_viewer->cmd(FlViewer::CmdLines);
	
	
	
}
void AppMainWin::jointListSelected()
{
	int count = 0;
	for(int i=0;i<ui_jointList->children();i++)
		   {
			   if(ui_jointList->child(i)->selected())
			   {	
				   count++;
					ODEJoint* j = App->human->joint(ui_jointList->child(i)->label());
					if(j)
					{
						ui_multiSelect->label(j->joint->name());
						ui_jointGrav->value(j->grav);
						ui_jointVf->value(j->vforce);
						ui_jointJX->value(j->vfMult.x);
						ui_jointJY->value(j->vfMult.y);
						ui_jointJZ->value(j->vfMult.z);

						PID* p = App->human->reference_controller->pid(j->name());
						if(p)
						{
							ui_jointP->value(p->Kp);
							ui_jointD->value(p->Kd);
							ui_jointPD->value(j->usePID);
						}
					}
		
			   }

		}
	if(count>1)ui_multiSelect->label("Multiple Joints");
	
	ui_multiSelect->redraw();
}
void AppMainWin::setUIFromScene()
	{
		ODEHuman* h = App->human;

		ui_collide_feet_only->value(h->collideFeetOnly);
		ui_useComJ->value(h->useComJ);
		ui_useGravC->value(h->useGravC);
		ui_usePID->value(h->usePID);
		ui_showSkel->value(App->skelVis);
		ui_shaded->value(App->shaded);
	
		//ui_useBubbleT->value(h->reference_controller->useBubbleT);

		ui_kg->value(h->gravity_controller->mag);

		ui_walk_speed->value(h->walk_controller->getGoalSpeed());
	//	ui_walk_sway->value(0.5);
		ui_walk_hheight->value(h->getHipHeight());
		ui_jkd->value(h->velocity_controller->jkD);
		ui_jkp->value(h->velocity_controller->jkP);
		ui_jkdc->value(h->velocity_controller->jkDc);
		ui_jkpc->value(h->velocity_controller->jkPc);
		ui_vs->value(h->velocity_controller->velDSagittal);
		ui_vc->value(h->velocity_controller->velDCoronal);
		ui_use_root->value(h->useRoot);
		ui_root_kd->value(h->root_controller->Kd);
		ui_root_kp->value(h->root_controller->Kp);
		ui_root_damp->value(h->root_controller->stanceHipDamping);
		ui_root_maxV->value(h->root_controller->stanceHipMaxVelocity);
		ui_viewer->redraw();
	}
void AppMainWin::setSceneFromUI()
{
		ODEHuman* h = App->human;

		App->skelVis = ui_showSkel->value();
		App->shaded = ui_shaded->value();

		if(App->shaded) ui_viewer->cmd(FlViewer::CmdAsIs);
		else ui_viewer->cmd(FlViewer::CmdLines);

		ui_viewer->_knsref->set_visibility(App->skelVis,0,0,0);

		h->useComJ  = ui_useComJ->value();
		h->useGravC = ui_useGravC->value();
		h->usePID   = ui_usePID->value();
		h->useRoot = ui_use_root->value();
		h->collideFeetOnly = ui_collide_feet_only->value();
		//h->walk_controller->hipSway = (float)ui_walk_sway->value();
		h->gravity_controller->mag = (float)ui_kg->value();
		h->walk_controller->setSpeed( (float)ui_walk_speed->value() );
		h->velocity_controller->jkD	   = (float)ui_jkd->value();
		h->velocity_controller->jkP	   = (float)ui_jkp->value();
		h->velocity_controller->jkDc	   = (float)ui_jkdc->value();
		h->velocity_controller->jkPc    = (float)ui_jkpc->value();
		h->velocity_controller->velDSagittal = (float)ui_vs->value();
		h->velocity_controller->velDCoronal = (float)ui_vc->value();
	
		//	h->reference_controller->useBubbleT = ui_useBubbleT->value();
		
		h->root_controller->Kp = (float)ui_root_kp->value();
		h->root_controller->Kd = (float)ui_root_kd->value();
		h->root_controller->stanceHipDamping = (float)ui_root_damp->value();
		h->root_controller->stanceHipMaxVelocity = (float)ui_root_maxV->value() ;
		h->setHipHeight((float)ui_walk_hheight->value());

		h->contact_controller->visible(ui_contactVis->value());
}

void AppMainWin::jointListModified()
{
	for(int i=0;i<ui_jointList->children();i++)
		   {
			   if(ui_jointList->child(i)->selected())
			   {
					ODEJoint* j = App->human->joint(ui_jointList->child(i)->label());
					message( "%s manipulated" , j->joint->name() );
					j->grav = ui_jointGrav->value();
					j->vforce = ui_jointVf->value();
					j->vfMult.x = (float)ui_jointJX->value();
					j->vfMult.y = (float)ui_jointJY->value();
					j->vfMult.z = (float)ui_jointJZ->value();
					PID* p = App->human->reference_controller->pid(j->name());
					if(p)
					{
						p->Kp = (float)ui_jointP->value();
						p->Kd = (float)ui_jointD->value();
						j->usePID = ui_jointPD->value();
					}
		
			   }
		   }
}
void AppMainWin::show ()
 {
	//refreshScene();
   _vtwin->vars ( App->files->vars );
   message ( "Ready" );
   stance_message("Flying");
   ui_window->show();
 
 }

void AppMainWin::setUIFromSkel()
{
	//gsout<<"left hip:"<<JOINT_VAL("LeftUpLeg",0)<<"\t"<<JOINT_VAL("LeftUpLeg",2)<<gsnl;
	
	ui_jnt[0]->value(JOINT_VAL("LeftUpArm",0));
	ui_jnt[1]->value(JOINT_VAL("LeftUpArm",2));
	ui_jnt[2]->value(JOINT_VAL("RightUpArm",0));
	ui_jnt[3]->value(JOINT_VAL("RightUpArm",2));
	ui_jnt[4]->value(JOINT_VAL("LeftArm",0));
	ui_jnt[5]->value(JOINT_VAL("RightArm",0));		 
	ui_jnt[6]->value(JOINT_VAL("LeftUpLeg",1));	
	ui_jnt[7]->value(JOINT_VAL("LeftUpLeg",0));	
	ui_jnt[8]->value(JOINT_VAL("RightUpLeg",1));	
	ui_jnt[9]->value(JOINT_VAL("RightUpLeg",0));
	ui_jnt[10]->value(JOINT_VAL("LeftLeg",1));	
	ui_jnt[11]->value(JOINT_VAL("RightLeg",1));
	ui_jnt[12]->value(JOINT_VAL("LeftFoot",1));
	ui_jnt[13]->value(JOINT_VAL("RightFoot",1));
	ui_jnt[14]->value(JOINT_VAL("Head",0));
	ui_jnt[15]->value(JOINT_VAL("Spine1",0));
	ui_jnt[16]->value(JOINT_VAL("Spine1",2));
	ui_jnt[17]->value(JOINT_VAL("Spine2",0));
	ui_jnt[18]->value(JOINT_VAL("Spine2",2));
	ui_jnt[19]->value(JOINT_VAL("LeftToe",0));
	ui_jnt[20]->value(JOINT_VAL("RightToe",0));

	ui_viewer->redraw();
}

void AppMainWin::setSkelFromUI()
{
			  SET_JOINT("LeftUpArm",UI_JNT_VAL(0),0,UI_JNT_VAL(1));
			  SET_JOINT("RightUpArm",UI_JNT_VAL(2),0,UI_JNT_VAL(3));
			  SET_JOINT("LeftArm",UI_JNT_VAL(4),0,0);
			  SET_JOINT("RightArm",UI_JNT_VAL(5),0,0);
			  SET_JOINT("LeftUpLeg",UI_JNT_VAL(7),UI_JNT_VAL(6),0);
			  SET_JOINT("RightUpLeg",UI_JNT_VAL(9),UI_JNT_VAL(8),0);
			  SET_JOINT("LeftLeg",0,UI_JNT_VAL(10),0);
			  SET_JOINT("RightLeg",0,UI_JNT_VAL(11),0);
			  SET_JOINT("LeftFoot",0,UI_JNT_VAL(12),0);
			  SET_JOINT("RightFoot",0,UI_JNT_VAL(13),0);
			  SET_JOINT("Head",UI_JNT_VAL(14),0,0);
			  SET_JOINT("Spine1",UI_JNT_VAL(15),0,UI_JNT_VAL(16));
			   SET_JOINT("Spine2",UI_JNT_VAL(17),0,UI_JNT_VAL(18));
			   SET_JOINT("LeftToe",UI_JNT_VAL(19),0,0);
				SET_JOINT("RightToe",UI_JNT_VAL(20),0,0);
			  App->human->ik_controller->matchIK();
			  ui_viewer->_knsref->update();
			  ui_viewer->redraw();
}


const char* AppMainWin::selectedConfig()
{
	ui_config_browser->redraw();
	for(int i=0;i<ui_config_browser->children();i++)
	{
		if(ui_config_browser->child(i)->selected())
		{
			return ui_config_browser->child(i)->label();
		}
	}


	gsout<<"warning returning a garbage file\n";;
	return "garbage";
}

void AppMainWin::event ( AppEvent e )
{
	ODEHuman* h = App->human;
	AppFileManager* f = App->files;

	switch ( e )
	{
	case evRunOde: App->odeRunning = (bool)ui_ode_run->value();  break;
	case evOdeReset:
		App->files->saveCurrentState();
		App->setupOde();
		App->files->loadCurrentState();
	break;

	case evSkelAdjust:		setSkelFromUI(); break;
	case evControlMethods:	setSceneFromUI(); break;
	
	//case evManip:			h->ik_controller->setManual(ui_root_manip->value()); break;

	//FileManager
	case evSaveParm:		f->saveParameterNamed(ui_current_parm->value()); break;
	case evUpdateParm:		f->saveParameterNamed(ui_current_parm->value());  break;
	case evLoadParm :		f->loadParameterFiles();  break;
	case evDeleteParm:			 break;
	case evLoadConfig:		f->loadCurrentState();  break;
	case evSaveConfig:		f->saveCurrentState(); break;
	case evLoadDefault:		f->loadDefault();  break;

	case evParmListSelected:
		{
			const char* lb = selectedConfig();
			App->files->loadParameterNamed(lb);
			ui_current_parm->value(lb);
			setUIFromScene();
		}
		break;

	
	//Contact Controller
	case  evDoubleStance: h->contact_controller->goToContact(CONTACT_BOTH_FEET); break;
	case  evLeftStance: h->contact_controller->goToContact(CONTACT_LEFT_FOOT);break;
	case evRightStance: h->contact_controller->goToContact(CONTACT_RIGHT_FOOT);break;
	case evToeStance:	h->contact_controller->goToToes(); break;
	case evHeelStance: h->contact_controller->goToHeels(); break;
	case evFixedStance: h->contact_controller->fixFoot(); break;
	case evStartStep: h->step_controller->makeStep(STEP_LEFT_FOOT); break;
	//Walk Controller
	case evWalkAdjust:
		//h->walk_controller->hipSway = ((float)ui_walk_sway->value());
		h->walk_controller->setDirection((float)ui_walk_dir->value());
		
	//	h->step_controller->setSpeed((float)ui_step_speed->value());
		h->walk_controller->headingArrow->setVisible(ui_arrow->value());
//		h->step_controller->setStepDist((float)ui_step_dist->value());
		
		h->walk_controller->stepSpeed = (float)ui_walk_speed->value();
		h->walk_controller->stepHeight = (float)ui_step_height->value();

		h->setHipHeight((float)ui_walk_hheight->value());
		break;
	case evWalk: 
		if(ui_walk->value())h->walk_controller->startWalking();
		else h->walk_controller->stopWalking();
		break;

	case   evJointListSelect:  jointListSelected();   break;
	case  evIndividualJointAdjust: jointListModified();  break;




	
	case evQuit: App->files->saveCurrentState();  gs_exit();  break;
	

	case evDropBalls: App->dropBalls(); break;
	case evEditParameters: 
		_vtwin->update ();
		_vtwin->show (); break;



	case evRest: h->setTotalVelocity(GsVec(0,0,0)); break;



	}
	ui_viewer->redraw();

 }

