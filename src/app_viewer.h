# pragma once

# include <gsim/fl_viewer.h>
#include <gsim/kn_scene.h>


class AppMainWin;
class SnGroup;
class AppViewer : public FlViewer
 { public :
    SnGroup*  _root;
	SnGroup*  _ikGroup;
	KnScene* _kns;
KnScene* _knsref;
	SnGroup* _lines;
    AppMainWin* _mainwin;
	
   public:
    AppViewer ( int x, int y, int w, int h, const char *l=0 );
   ~AppViewer ();
    void init ( AppMainWin* win );


    virtual void draw ();

    // several event handlers can be re-writen, below is on example; see the
    // base class header file for all possible event methods to be re-written
    virtual int handle_scene_event ( const GsEvent &e );
};

