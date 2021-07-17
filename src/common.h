#pragma once
#include <math.h>
#include <gsim/gs_vec.h>
#include <gsim/gs_vars.h>
#include <gsim/gs_array.h>
#include <gsim/gs_euler.h>
#include <gsim/gs_polygon.h>
#include <gsim/gs_model.h>
#include <gsim/gs_quat.h>

#include <gsim/kn_posture.h>
#include <gsim/kn_skeleton.h>
#include <gsim/kn_scene.h>
#include <gsim/sn_manipulator.h>
#include <gsim/sn_transform.h>
#include <gsim/sn_model.h>
#include <gsim/sn_lines.h>
#include <gsim/fl.h>
#include <gsim/fl_output_win.h>
#include <gsim/sn_manipulator.h>

#define TOES false 

#define SET_JOINT(j,x,y,z) App->skref->joint(j)->euler()->value(x,y,z);
#define UI_JNT_VAL(x) GS_TORAD(ui_jnt[x]->value())
#define JOINT_VAL(x,y) GS_TODEG(App->skref->joint(x)->euler()->value(y))

#define GRAVITY -9.8f*5
#define START_HEIGHT 4.2f
#define TIME_STEP 0.01f
#define min(a,b)            (((a) < (b)) ? (a) : (b))

static inline float bump ( float t ) // f([0,1])->[0,1]
 {
  return 1.0f - pow ( 2.0f*t - 1, 4.0f ); //exponent has to be pair: 2 (low extremity speed), 4, 6, 8 (quicker), ...
 }
void swapP(void * pOne, void * pTwo);
float boundToRange(float in,float min,float max);
GsVec interp(GsVec start, GsVec end , float t);
float interp(float start,float end, float t);
float interp_cubic ( float t, float tmin, float tmax );
GsVec GetClosetPoint(GsVec A, GsVec B, GsVec P, bool segmentClamp);


static inline GsVec vecMult(GsVec a,GsVec b){return GsVec(a.x*b.x,a.y*b.y,a.z*b.z);}