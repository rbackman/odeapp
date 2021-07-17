/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/sn_model.h>
# include <gsim/sn_transform.h>
# include <gsim/sn_manipulator.h>
# include <gsim/kn_scene.h>
# include <gsim/kn_coldet.h>
# include <gsim/kn_skeleton.h>
# include <gsim/kn_skin.h>

# include "ik_manipulator.h"
//=================================== IkManipulator =====================================

IkManipulator::IkManipulator ()
 {
   _ik = 0;
   _ikbody = 0;
   _ikbodyid = 0;
   _knscene = 0;
   _coldet = 0;
   _cbfunc = 0;
   _postcbfunc = 0;
   _udata = 0;
   _postudata = 0;
   separator ( true );
 }

IkManipulator::~IkManipulator ()
 {
   if ( _ik ) _ik->unref();

   if ( _ikbody ) _ikbody->unref();
   if ( _knscene ) _knscene->unref();
   if ( _coldet ) _coldet->unref();
 }

static void _ikcb ( SnManipulator* /*manip*/, const GsEvent& /*ev*/, void* udata )
 {
   ((IkManipulator*)udata)->update();
 }

bool IkManipulator::init ( KnIk* ik, GsModel* goalmodel )
 {
   if ( !ik ) return false;
   if ( !ik->base() ) return false;
   
   // unref old classes:
   if ( _ik ) _ik->unref();
   if ( _ikbody ) { _ikbody->unref(); _ikbody=0; }
   
   // ref new classes:
   _ik = ik;
   _ik->ref();
   _ik->lines(0);
   _iksearch.set_defaults ( _ik->type() );

   
   // create nodes to display:
   GsModel* model = goalmodel ? goalmodel : _ik->end()->visgeo();
   _manip = new SnManipulator;
   _lines = new SnLines;
   _matrix = new SnTransform;
   SnModel* snmodel = new SnModel(model); // ok if model is null
   if ( _ik->end()->skeleton()->skin() ) snmodel->visible(false); //render_mode(gsRenderModeLines);
   SnGroup::remove_all();
   SnGroup::add ( _manip );
   SnGroup::add ( _lines );
   SnGroup::add ( _matrix );
   SnGroup::add ( snmodel ); 

   // Create axis with the same sizes as of the model to manipulate:
   GsBox box;
   if ( model ) model->get_bounding_box ( box );
    else { box.a=GsVec::minusone; box.b=GsVec::one; }
   SnLines* l = new SnLines;
   l->push_axis ( GsPnt::null, 0/*len not used*/, 3, "xyz", true/*unit marks*/, &box );

   // Set the manip to control the axis in the current wrist place:
   _ik->end()->skeleton()->update_global_matrices();
   match ();
   _manip->child ( l );
   _manip->callback ( _ikcb, this );

   return true;
 }

bool IkManipulator::init ( IkBody* ikbody, int i, GsModel* goalmodel )
 {
   if ( !ikbody ) return false;

   if ( !ikbody->ik(i) ) return false;
   if ( !init(ikbody->ik(i),goalmodel) ) return false;
   
   _ikbody = ikbody;
   _ikbody->ref();
   _ikbodyid = i;
   return true;
 }

 
void IkManipulator::post_callback ( CallBack cb, void* udata )
 {
   _postcbfunc = cb;
   _postudata = udata;
 }

void IkManipulator::match ()
 {
   GsMat goal;
   _ik->base()->skeleton()->update_global_matrices();
   goal = _ik->end()->gmat();
    
   _manip->init();
   _manip->initial_mat ( goal );
   _matrix->set ( goal );
 }
 
void IkManipulator::lines ( bool b )
 {
   if ( b )
    { _ik->lines ( _lines );
      update ();
    }
   else
    { _ik->lines ( 0 );
      _lines->init();
    }
 }

KnIk::Result IkManipulator::update ()
 {
   if ( !_manip ) return KnIk::Ok;
   _matrix->set ( _manip->mat() ); // update the displayed model position
   return iksolve ();   
 }

const GsMat& IkManipulator::cmat() const
 {
   if ( !_manip ) return GsMat::id;
   return _manip->mat();
 }

GsMat& IkManipulator::mat()
 {
   return _manip->mat();
 }

//-------------- virtuals -----------------------

KnIk::Result IkManipulator::iksolve ()
 {
   KnIk::Result res = KnIk::Ok;
	res = _ikbody->solve ( _ikbodyid, _manip->mat() );
    if ( _knscene ) _knscene->update();
     
   if (_postcbfunc) (_postcbfunc) ( this, _postudata );

   return res;
 }

int add_manipulators ( SnGroup* g, IkBody* ikb, KnScene* knscene, 
                          IkManipulator::CallBack postcb, void* udata, KnColdet* coldet )
 {
   IkManipulator* ikm;
   int i, count=0;

   for ( i=0; i<4; i++ )
    {
      ikm = new IkManipulator;
      ikm->ref();

	  GsModel* m = new GsModel;
	  GsBox box;
	  float size = 0.3f;
	  box.set(GsVec(-size,-size*0.7f-0.2f,-2*size),GsVec(size,size*0.2f-0.2f,2*size));
	  m->make_box(box);

	  if ( ikm->init(ikb,i,m ))
       { ikm->label ( "IkManip" );
         g->add ( ikm );
         if ( postcb ) ikm->post_callback(postcb,udata);
         count++;
       }

      ikm->unref();
    }

   return count;
 }

int match_manipulators ( SnGroup* g )
 {
   IkManipulator* ikm;
   int i, count=0;

   for ( i=0; i<g->size(); i++ )
    {
      if ( gs_comparecs(g->get(i)->label(),"IkManip")==0 )
       {
         ikm = (IkManipulator*)g->get(i);
         ikm->match();
         count++;
       }
    }

   return count;
 }

int update_manipulators ( SnGroup* g )
 {
   IkManipulator* ikm;
   int i, count=0;

   for ( i=0; i<g->size(); i++ )
    {
      if ( gs_comparecs(g->get(i)->label(),"IkManip")==0 )
       {
         ikm = (IkManipulator*)g->get(i);
         ikm->update();
         count++;
       }
    }

   return count;
 }

IkManipulator* get_manipulator ( SnGroup* g, int id )
 {
   IkManipulator* ikm;
   int i;

   for ( i=0; i<g->size(); i++ )
    {
      if ( gs_comparecs(g->get(i)->label(),"IkManip")==0 )
       { ikm = (IkManipulator*)g->get(i);
         if ( ikm->ikbodyid()==id ) return ikm;
       }
    }

   return 0;
 }

//======================================= EOF =====================================

