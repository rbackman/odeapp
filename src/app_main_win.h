# pragma once

# include <gsim/gs_vars.h>
# include <gsim/fl_vars_win.h>

# include "app_fluid.h"
# include "app_main.h"

class AppMainWin : public AppFluid
 { private :
    FlVarsWin* _vtwin;
    GsString _buf;
	
	
   public :
    AppMainWin ();
   ~AppMainWin ();
    void show ();
	//Sets a message about which stance state .. Both Feet, Left Foot etc
	void stance_message ( const char* s ) { ui_stepState->label(s); ui_stepState->redraw(); }
	
	//a general place to display error messages
	void message ( const char* s ) { ui_message->label(s); ui_message->redraw();}
    void message ( const char* s, int i ) { _buf.setf(s,i); ui_message->label(_buf);  ui_message->redraw();}
    void message ( const char* s, float f ) { _buf.setf(s,f); ui_message->label(_buf);  ui_message->redraw();}
	void message ( const char* s, const char* f ) { _buf.setf(s,f); ui_message->label(_buf);  ui_message->redraw();}
	
	//UI for manually controlling euler angles
	void setUIFromSkel(); //update sliders on UI .. mostly from IK changing skeleton
	void setSkelFromUI(); //set euler angles of skeleton from the UI
	
	void refreshScene(); //probably a bad way to control if the scene is rendered shaded or wireframe
	
	void jointListModified(); //set the joint values based on changes of the UI
	void jointListSelected(); //fill the UI with info on the current selected joint


	void setUIFromScene(); //Copy all scene paramaters to the UI. used for loading a new character	
	void setSceneFromUI(); //set the scene based on the current UI paramaters
const char* selectedConfig();
   public :
    virtual void event ( AppEvent e );
 };

