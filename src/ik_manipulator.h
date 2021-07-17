/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef KN_IK_MANIPULATOR_H
# define KN_IK_MANIPULATOR_H

# include <gsim/sn_lines.h>
# include <gsim/sn_shape.h>
# include <gsim/sn_group.h>
# include "ik_body.h"

//=================================== IkManipulator =====================================

class SnManipulator;
class SnTransform;
class KnScene;
class KnIk;
class IkBody;

class IkManipulator : public SnGroup
 { public :
    typedef void (*CallBack)( IkManipulator* ikm, void* udata );

   protected :
    KnIk* _ik;
    IkBody* _ikbody;
    int _ikbodyid;
    KnIkOrbitSearch _iksearch;
    KnScene* _knscene;
    KnColdet* _coldet;
    CallBack _cbfunc, _postcbfunc;
    void* _udata;
    void* _postudata;
    SnManipulator* _manip;
    SnLines* _lines;
    SnTransform* _matrix;

   protected :
    /*! Destructor only accessible through unref() */
   ~IkManipulator ();

   public :
    /*! Constructor */
    IkManipulator ();

    /*! Get and ref the given ik, attaching the manipulator to it.
        The ik pointer must be already properly initialized.
        The goalmodel, if given, is loaded in the goal position frame,
        otherwise the model attached to the end joint, if any, is used.
        See method callback() for specifying a user callback function, and
        method solve_method() for setting an automatic solve behavior.
        By default, the behavior is set to use a user callback.
        Returns true if initialization was succesfull and false otherwise. */
    bool init ( KnIk* ik, GsModel* goalmodel=0 );

    /*! Uses the given ikbody for solving the IK of the i-th ik only. 
        For example the default argument of 1 is the right wrist.
        KnIkBody must be properly initialized.
        Method solve_method() must be called afterwards. */
    bool init ( IkBody* ikbody, int i=1, GsModel* goalmodel=0 );

    /*! Access to the orbit angle search parameters; see solve_method() */
    KnIkOrbitSearch& iksearch () { return _iksearch; }
    
    /*! Set a user-defined callback to be called whenever the manipulator moves,
        completly replacing any defined solve method (the solve method becomes UserCallback)
        The user callback will then be responsible to do the job of: 
        calling ik()->solve() for current goal, updating the skeleton values,
        and drawing the new skeleton pose. */
    void callback ( CallBack cb, void* udata );
    
    /*! Set a callback to be called after each call to iksolve, ie, each time the manip moves */
    void post_callback ( CallBack cb, void* udata );
    
    /*! Returns the previously set user data pointer */
    void* udata () { return _udata; }

    /*! Set the manipulator to match the current configuration of the skeleton.
        Global matrices of the skeleton will be automatically updated. */
    void match ();

    /*! will turn on or off the drawing of the IK lines */
    void lines ( bool b );

    /*! Access to the used IkBody object (or null if not used) */
    IkBody* ikbody() const { return _ikbody; }
    int ikbodyid() const { return _ikbodyid; }

    /*! Access to the used IK object */
    KnIk* ik() { return _ik; }
    
    /*! Access to the SnManipulator object */
	SnManipulator* manip() { return _manip; }

    /*! Update axis position and call iksolve() as if the goal was moved */
    KnIk::Result update ();
    
    /*! Returns the current goal matrix in the manipulator */
    const GsMat& cmat () const;

	/*! This one can only be called after initialization!  */
	GsMat& mat ();

    /*! This method either calls the associated callback, if any, or proceed
        and solve the IK according to the specified solve method.
        As it is virtual, it can also be rewritten for custom behavior. */
    virtual KnIk::Result iksolve ();
 };

/*! Adds up to four ik manipulators to given group g (each with label "IkManip").
    Returns the number of manipulators returned. */
int add_manipulators ( SnGroup* g, IkBody* ikb, KnScene* knscene,
                          IkManipulator::CallBack postcb=0, void* udata=0, KnColdet* coldet=0 );

/*! For each IkManipulator child of g, calls its match() method. Return number of manips found. */
int match_manipulators ( SnGroup* g );

/*! For each IkManipulator child of g, calls its update() method. Return number of manips found. */
int update_manipulators ( SnGroup* g );

/*! Find the IkManipulator child of g which controls an ikbody with index id.
    Returns null if not found. */
IkManipulator* get_manipulator ( SnGroup* g, int id );

//======================================= EOF =====================================

# endif // KN_IK_MANIPULATOR_H
