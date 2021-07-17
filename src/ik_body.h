/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 


	this class was copied and modified from kn_ik_body.h in graphsim
  =======================================================================*/

# ifndef IK_BODY_H
# define IK_BODY_H

# include <gsim/gs_shared.h>
# include <gsim/kn_coldet.h>
# include <gsim/kn_posture.h>
# include <gsim/kn_ik.h>


class SnGroup;
class SnLines;

//========================= IkBody ================================

/*! Contains all data needed for computing the Inverse Kinematics of
    the four limbs simultaneously. All classes are exchanged honoring the
    calls to ref()/unref() for efficient sharing and change of ownership */
class IkBody : public GsShared
 { private :
    KnSkeleton*  _skeleton;       // associated skeleton
    KnColdet*    _coldet;         // collision detector
    KnPosture    _posture;        // to save last posture in case IK failed
    KnIk* _ik[4];         // larm, rarm, lleg, rleg (some may be null)
   
    GsMat        _locgoal;        // last local goal matrix
    SnLines*     _lines[4];       // lines for each IK

    double _solvetime;            // if >=0, solve timer is activated
	KnIk::Result _result;		  //last result of IK
    GsMat _lfoot, _rfoot;         // global feet matrices
    GsVec _initpos;               // initial position of the detected root
	KnJoint* _root;
   public :
    /*! Initializes the class with default parameters and orbit and style search seta non-usable class, init() must be properly called afterwards */
    IkBody ();

    /*! Destructor is public but be sure to respect ref()/unref() use */
   ~IkBody ();


    /*! Initializes with the names of the end effector joints to be used in a
        single string, in the following order: lwrist, rwrist, lankle, rankle.
        The corresponding KnIk will be null in case of an initialization error.
        Returns the number of IKs created.  */
    int init ( KnSkeleton* kn, const char* endefs );

    /*! Initializes with given IKs of types: larm, rarm, lleg, rleg.
        They must be properly initialized. Null can be given to those not used.
        Returns the number of IKs created. */
    int init ( KnIk* ik[4] );


    /*! Returns ik of given index: 0:larm, 1:rarm, 2:lleg, 3:rleg. Can be null. */
    KnIk* ik ( int i ) { return _ik[i]; }

    /*! Declares the coldet to be considered, which must be connected to the used skeleton.
        Null can be set in case no collision detection is to be used */
    void coldet ( KnColdet* cd );

    /*! Returns used coldet. Can be null if no collisions are checked. */
    KnColdet* coldet () { return _coldet; }

    /*! For each valid ik, set limits of the base, mid, end joints to the maximum range */
    void remove_joint_limits ();

    /*! Stores current feet global position, legs orbit angles and root position */
    void capture_init_posture ();

    /*! Returns the global matrix of the left foot when captured as initial posture */
    GsMat& lfoot () { return _lfoot; }

    /*! Returns the global matrix of the right foot when captured as initial posture */
    GsMat& rfoot () { return _rfoot; }

    /*! Solve i-th ik, if available, according to the settings. */
	KnIk::Result solve ( int i, const GsMat& globgoal );

    /*! Call solve for each of the captured lfoot and rfoot positions */
    void fixfeet () { solve(2,_lfoot); solve(3,_rfoot); }
    
    /*! Adds up to four SnLines as children of given group g, each with 
        label "IkLines", and also save them in IkBody.
        Returns the number of SnLines inserted. */
    int add_lines ( SnGroup* g );

    /*! Returns the SnLines index i, can be null */
    SnLines* lines ( int i ) { return _lines[i]; }

    /*! Attach or detach SnLines to the IK index i */
    void lines ( int i, bool attach );

    /*! Set time measurement for each solve call on or off */
    void timer_activation ( bool on ) { if (on) _solvetime=0; else _solvetime=-1; }

    /*! Get the time (in secs) of the last call to a solve method.
        -1 is returned if the timer is not activated */
    double lsolvetime () { return _solvetime; }

   private:
    void _initskel ();
    void _unrefik ();
 };

//============================= EOF ===================================

# endif // KN_IK_BODY_H


