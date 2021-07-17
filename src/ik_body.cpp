/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include "ik_body.h"
# include <gsim/sn_lines.h>
# include <gsim/sn_group.h>
# include <gsim/kn_skeleton.h>

//========================= IkBody ================================

IkBody::IkBody ()
 {
   _skeleton = 0;
   _coldet = 0;
   _solvetime = -1;

   int i;
   for ( i=0; i<4; i++ )
    { 
	  _ik[i] = 0;
      _lines[i] = 0;
    }
 }

IkBody::~IkBody ()
 {
   _unrefik ();
   if ( _skeleton ) _skeleton->unref();
   if ( _coldet ) _coldet->unref();
 }   

void IkBody::_unrefik ()
 {
	for (int i=0; i<4; i++ ) if ( _ik[i] ) {_ik[i]->unref(); _ik[i]=0;} 
 }


int IkBody::init ( KnSkeleton* sk, const char* endefs )
 {
   KnJoint* jts[4];
   _unrefik ();
   _skeleton = sk;
   _root = _skeleton->root();
   if ( _skeleton )
    { 
	  _skeleton->ref();
      _posture.init ( _skeleton );
    }

   int i;
   GsInput inp;
   inp.init ( endefs );
   for ( i=0; i<4; i++ )
    { jts[i] = 0;
      inp.get();
      if ( inp.ltype()==GsInput::String ) 
       { jts[i] = sk->joint ( inp.ltoken() );
       }
    }


   int count=0;
   bool ok;
   _unrefik ();

   for ( i=0; i<4; i++ )
    { // ensure endef is valid:
      if ( jts[i]==0 ) continue;

      // create new one:
      _ik[i] = new KnIk;
      _ik[i]->ref();

      // initialize it:
      ok = _ik[i]->init ( (KnIk::Type)i, jts[i] );
      if ( ok )
       { count++; }
      else
       { _ik[i]->unref(); _ik[i]=0; }
    }






   return count;
 }


void IkBody::coldet ( KnColdet* cd )
 {
   if ( _coldet ) { _coldet->unref(); _coldet=0; }
   _coldet = cd;
   if ( _coldet ) _coldet->ref();
 }

void IkBody::remove_joint_limits ()
 {
	  if ( _ik[0] ) _ik[0]->remove_joint_limits();
      if ( _ik[1] ) _ik[1]->remove_joint_limits();
      if ( _ik[2] ) _ik[2]->remove_joint_limits();
      if ( _ik[3] ) _ik[3]->remove_joint_limits();
  
 }

void IkBody::capture_init_posture ()
 {
   if ( !_skeleton ) return;
   _skeleton->update_global_matrices();
   
   // get feet global positions:
   if ( _ik[2] ) _lfoot = _ik[2]->end()->gmat();
   if ( _ik[3] ) _rfoot = _ik[3]->end()->gmat();

   // get root position:
   _initpos = _root->pos()->value();

 }

KnIk::Result IkBody::solve ( int i, const GsMat& globgoal )
 {
   // 1. Be sure IK exists:
   if ( !_ik[i] ) return KnIk::Ok;

   // 2. Set goal matrix to local:
   _posture.get(); // save entire body posture, looking ahead to full body ik
   _skeleton->update_global_matrices();
   _locgoal = globgoal;

   _ik[i]->set_local(_locgoal);

   // 3. Solve linkage i:
   if ( _solvetime>=0 ) _solvetime=gs_time();

   _result = _ik[i]->solve ( _locgoal,0); //goal and orbit angle
  

   if ( _solvetime>=0 ) _solvetime=gs_time()-_solvetime;

  // if ( _result==KnIk::Ok )
		_ik[i]->apply_last_result();
 //  else
//		_posture.apply();

   return _result;
 }

int IkBody::add_lines ( SnGroup* g )
 {
   int i, count=0;
   SnLines* lines;

   for ( i=0; i<4; i++ )
    {
      if ( ik(i) )
       {
         lines = new SnLines;
         lines->label ( "IkLines" );
         _lines[i] = lines;
         g->add ( lines );
         count++;
       }
    }

   return count;
 }

void IkBody::lines ( int i, bool attach )
 {
	if ( ik(i) ) ik(i)->lines ( attach? lines(i):0 ); 

   if ( _lines[i] && !attach ) _lines[i]->init();
 }

//============================== EOF =====================================
