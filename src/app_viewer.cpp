
# include <gsim/fl.h>
# include <gsim/fl_vars_win.h>


#include <gsim/sn_group.h>
#include <gsim/sn_polyed.h>


# include "app_viewer.h"
# include "app_main_win.h"

#include "ode_human.h"

# define CHKVAR(s) if(_vars->search(s)<0) { fltk::alert("Missing parameter [%s]!",s); exit(0); }


AppViewer::AppViewer ( int x, int y, int w, int h, const char *l ) : FlViewer ( x, y, w, h, l )
 {
   // we build here an example scene graph for polygon edition
		 _root = new SnGroup;


		_ikGroup = new SnGroup;
		_lines = new SnGroup;
		_kns = new KnScene;
		_knsref = new KnScene;
		_root->add(_knsref);
		_root->add(_kns);
		_root->add(_ikGroup);
		_root->add(_lines);


	view_all();

  // FlViewer::cmd ( FlViewer::CmdPlanar );
   FlViewer::root ( _root );
 }

AppViewer::~AppViewer ()
 {

 }

void AppViewer::init ( AppMainWin* win )
 {
   _mainwin = win;
   
 }

void AppViewer::draw ()
 {
	
   _knsref->update();
   _kns->update();
   FlViewer::draw ();
 }


int AppViewer::handle_scene_event ( const GsEvent &e )
 {
   // window events can be custom-processed here:
   if ( e.button1 )
    {
    }

   // now let the viewer process the remaining events:
   return FlViewer::handle_scene_event ( e );
 }

