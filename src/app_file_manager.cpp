


#include "app_file_manager.h"

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


//# define GS_USE_TRACE1  // keyword tracing
# include <gsim/gs_trace.h>

# define CHKVAR(s) if(_vars->search(s)<0) { fltk::alert("Missing parameter [%s]!",s); exit(0); }

AppFileManager::AppFileManager ()
 {

	 vars = new GsVars;
	 files = new GsVars;
	 loadVars ( files, "../data/files.txt" );
	 directory = files->get("dataDir")->gets();
	 fileType  = files->get("fmt")->gets();

 }


void AppFileManager::loadVars( GsVars* v, const char* cfgfile )
 {
   GsInput in;
   if ( !cfgfile ) cfgfile = fltk::file_chooser ( "Enter Configuration File", "*.txt", 0 );
   if ( !cfgfile ) return;
   if ( !in.open(cfgfile) ) { fltk::alert("Could not open cfg file!"); return; }
	
   gsout<<"Loaded "<<cfgfile<<gsnl;
   
   in.commentchar ( '#' );
   in.lowercase ( false );

   while ( true )
    { if ( in.get()==GsInput::End ) break;
    
      GS_TRACE1 ( "Token: "<<in.ltoken() );

      if ( in.ltoken()=="parameters" )
       { GsVars parms;
         in >> parms;
         v->merge ( parms );
       }
      else if ( in.ltoken()=="end" )
       { break;
       }
    }

  GS_TRACE1("End Parsing");
 }

#define TAB <<"\t"<<
#define DUBTAB <<"\t\t"<<
#define TRITAB <<"\t\t\t"<<
#define SPACE <<" "<<
#define VC App->human->velocity_controller 
#define WC App->human->walk_controller
#define START_PARM(x) f<<x<<(strlen(x)<=7?"\t\t":"\t" )<<"=\t\t"<<
#define END_PARM <<";\n"
#define ROUND2(x) gs_round(x,0.01f)
#define TO_DEG(x) ROUND2( GS_TODEG(x) )

void AppFileManager::saveParameterNamed(const char* file)
	{
		GsString fileName;
		fileName << directory << file << fileType;
		gsout<<"Saving "<<fileName<<gsnl;

		GsOutput f;
		f.open(fileName);
		AppMainWin* m = App->mainwin;
		ODEHuman* human = App->human;
	

	f<<"# config file\n\n";
	f<<"#parameters for physical humanoid\n";
	f<<"#Robert Backman 10/23/2010\n\n";
	f<<"parameters\n{\n\n";

	f<<"	name =  \"" << file <<"\" ; \n"; 
	f<<"	joints =";
	for(int i=0;i<human->numJoints();i++)
		f<<" "<<human->joint(i)->name();
	f<<";\n\n";
	f<<"##\t			startVal\t\tP, I, D,\tgrav,\tPID,\tmaxT,  tMul, axis.(x,y,z),  vforce,\tvfMult.(x,y,z),\t\toffset.(x,y,z),\t\t\tboxDim.(x,y,z),\t\t\tangle.(x,y,z)\n"; 
	for(int i=0;i<human->numJoints();i++)
	{
		ODEJoint* j = human->joint(i);
		const char* name = j->name();
		if(j)
		{
			PID* p = human->reference_controller->pid(j->name());
			float kp = 0.95f,ki = 0.0f,kd = 0.01f,maxT = 200.0f,tMult = 500.0f;

			if(p)
			{
				kp = p->Kp;
				ki = p->Ki;
				kd = p->Kd;
				maxT = p->maxT;
				tMult = p->torqueMultiplier ;
			}
		
				START_PARM(name)
					0.1	<< "\t\t"	TAB //this first value is to make sure that everything else is encoded as a float
					kp				SPACE 
					ki				SPACE 
					kd				TAB 
					(int)j->grav	DUBTAB 
					(int)j->usePID	DUBTAB 
					maxT				DUBTAB 
					tMult				DUBTAB
					j->axis.x		SPACE 
					j->axis.y		SPACE 
					j->axis.z		DUBTAB 
					(int)j->vforce	TRITAB 
					ROUND2(j->vfMult.x)		SPACE 
					ROUND2(j->vfMult.y)		SPACE 
					ROUND2(j->vfMult.z)	TRITAB
					ROUND2(j->boxOffset.x)			SPACE
					ROUND2(j->boxOffset.y)			SPACE
					ROUND2(j->boxOffset.z)			TRITAB
					ROUND2(j->boxDim.x)			SPACE
					ROUND2(j->boxDim.y)			SPACE
					ROUND2(j->boxDim.z)			TRITAB
					TO_DEG(j->ax)*j->axis.x		SPACE
					TO_DEG(j->ay)*j->axis.y			SPACE
					TO_DEG(j->az)*j->axis.z			
				END_PARM;
			
		}
	}

	f<<"\n\t\t#jkP\tjkD\tjkPc\tjkDc\tvelDSagittal\tvelDCoronal;\n";

	START_PARM("velocityParm") 
		(float)VC->jkP				TAB  
		(float)VC->jkD				TAB  
		(float)VC->jkPc			TAB  
		(float)VC->jkDc			TAB  
		(float)VC->velDSagittal	TAB 
		(float)VC->velDCoronal
	END_PARM;

f<<"\n\t\t#stepHeight\thipSway\tgoalSpeed\tstepheight\ttransitionSpeed\tstartSpeed	\n";

		START_PARM("walkParm")
			(float)WC->stepHeight TAB 
			(float)0.5 /*WC->hipSway*/ TAB 
			(float)WC->getGoalSpeed() TAB 
			(float)human->getHeightOffset()
		END_PARM;				//=\t1;"; //<< WC-> TAB  VC->jkD TAB  VC->jkPc TAB  VC->jkDc TAB  VC->velDSagittal TAB VC->velDCoronal<<";\n"; 

f<<"\n\t\t#comj\tgrav\tpid\tskel\tshade\tfeetCollide\troot\n";
	START_PARM("humanParm") 
		human->useComJ				TAB 
		human->useGravC			TAB 
		human->usePID				TAB 
		App->skelVis				TAB 
		App->shaded				TAB 
		0 TAB
		human->collideFeetOnly		TAB 
		human->useRoot
	END_PARM; //"; //<< WC-> TAB  VC->jkD TAB  VC->jkPc TAB  VC->jkDc TAB  VC->velDSagittal TAB VC->velDCoronal<<";\n"; 


f<<"\n                #kp kd  damping  maxV\n";
START_PARM("rootParm")
(int)human->root_controller->Kp				TAB 
(int)human->root_controller->Kd				TAB 
human->root_controller->stanceHipDamping	TAB 
human->root_controller->stanceHipMaxVelocity
END_PARM; 



f<<"\n}\n\nend";
f.close();
}
void AppFileManager::loadParameterFiles()
{
		
		//mainwin->ui_current_parm->value();
		App->mainwin->ui_config_browser->remove_all();

		for(int i=0;i < files->get("configurations")->size();i++)
		{
			GsString fileName = files->get("configurations")->gets(i);
			GsString fileFullName;
			
			fileFullName<< directory << fileName << fileType;

			gsout<<"full name"<<fileFullName<<gsnl;
			
			
			App->mainwin->ui_config_browser->add(fileName);
		}

	
}



void AppFileManager::loadParameterNamed(const char* file)
{
	GsString fileName;
	fileName << directory << file << fileType;

	loadVars ( vars, fileName );

	AppMainWin* m = App->mainwin;

	int numJ = vars->get("joints")->size();
	m->ui_jointList->remove_all();
	for(int i=0;i<numJ;i++)
	{
		const char* jname = vars->get("joints")->gets(i);
		m->ui_jointList->add(jname);
		ODEJoint* j = App->human->joint(jname);

		if(j)
		{
			//gsout<<jname<<" loaded\n";
			PID* p = App->human->reference_controller->pid(jname);
			GsVar* data = vars->get(jname);
			if(p)
			{
				
				p->Kp = data->getf(CONFIG_PID_P); 
				p->Ki = data->getf(CONFIG_PID_I); 
				p->Kd = data->getf(CONFIG_PID_D);
				p->torqueMultiplier = data->getf(CONFIG_PID_T_MULT);
				p->maxT = data->getf(CONFIG_PID_MAXT);
			}
			j->usePID = (bool)data->geti(CONFIG_PID);
			j->grav = (bool)data->geti(CONFIG_GRAV);
			j->vforce = (bool)data->geti(CONFIG_VF);
			j->vfMult.x = data->getf(CONFIG_VFM_X);
			j->vfMult.y = data->getf(CONFIG_VFM_Y);
			j->vfMult.z = data->getf(CONFIG_VFM_Z);
			j->setAxis(data->getf(CONFIG_X), data->getf(CONFIG_Y),data->getf(CONFIG_Z));
					
		}
		
	}
	VC->jkP			= vars->get("velocityParm")->getf(0);
	VC->jkD			= vars->get("velocityParm")->getf(1);
	VC->jkPc			= vars->get("velocityParm")->getf(2);
	VC->jkDc			= vars->get("velocityParm")->getf(3);
	VC->velDSagittal	= vars->get("velocityParm")->getf(4);
	VC->velDCoronal	= vars->get("velocityParm")->getf(5);
	
//  #comj grav pid skel shade useBubble feetCollide root
	App->human->useComJ			= (bool)vars->get("humanParm")->getb(0);
	App->human->useGravC		= (bool)vars->get("humanParm")->getb(1);
	App->human->usePID			= (bool)vars->get("humanParm")->getb(2);
	App->skelVis				= (bool)vars->get("humanParm")->getb(3);
	App->shaded					= (bool)vars->get("humanParm")->getb(4);
//	App->human->reference_controller->useBubbleT = (bool)vars->get("humanParm")->getb(5) ;
	App->human->collideFeetOnly = (bool)vars->get("humanParm")->getb(6) ;
	App->human->useRoot			= (bool)vars->get("humanParm")->getb(7) ;

	App->human->root_controller->Kp = vars->get("rootParm")->getf(0) ;
	App->human->root_controller->Kd = vars->get("rootParm")->getf(1) ;
	App->human->root_controller->stanceHipDamping = vars->get("rootParm")->getf(2) ;
	App->human->root_controller->stanceHipMaxVelocity = vars->get("rootParm")->getf(3) ;



	WC->stepHeight = vars->get("walkParm")->getf(0);
	//WC->hipSway = vars->get("walkParm")->getf(1);
	WC->setSpeed(vars->get("walkParm")->getf(2));
	App->human->setHeightOffset(vars->get("walkParm")->getf(3));
		
	App->mainwin->setUIFromScene();

}
void AppFileManager::saveCurrentState()
{
	saveParameterNamed( "current" );
}
void AppFileManager::loadCurrentState()
{
	loadParameterNamed( "current" );
}
void AppFileManager::loadDefault()
{
	loadParameterNamed( "default" );
}

