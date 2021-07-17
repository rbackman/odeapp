// Ensure the header file is included only once in multi-file projects
// Pragma once is a non-standard but widely supported preprocessor directive
# pragma once

#include "common.h"


// A vector

#define BPIECES 1

enum curve_types
{
	LEGRANGE,
	BEZIER,
	BSPLINEQUAD,
	BSPLINECUBIC,
	BEZIERPIECES,
};
class Curve;


class Tang :public GsShared//tangent 
{
	public :
	int idx;
	GsVec* pt;		//curve pt for reference
	GsVec in;	    //in direction
	GsVec out;      //out direction
	GsVec	pi;     //in point
	GsVec	po;     //out point
	bool locked;    //in dir = out dir
	bool free;		//free = no tangent control
	bool flat;		//tang = 0
	int sharp;		// limit to prevent overlap
	int	tangSelected; //0 1:INtang 2:outTang
	Tang(GsVec* point);
};

class Curve 
 { public :
 ~Curve();
    /*! Curve constructor n*/
 Curve();

   void updateTangents();
   void update();
   void createTang(int i);
   void initTang();
   void makeSub();
   void addPoint(GsVec pt);
void clear();
   GsArray<GsVec> p; //control points
   GsArray<Tang*> t; //tangents
   GsArray<Curve*> subCvs; //for piecwise
   bool vis;  /*!determines if curve will be drawn*/
   GsVec cCol; /*!color for curve*/ 
   GsVec cpCol;/*!color for control poly*/ 

	int first;  /*!for initialization*/

	int divPerPair;		 //measure of curve resolution
	int curveMode;		 //current method for curve generation
	int closed;
	bool flat;

	//for selection
	int selectionState; //0:not selected 1:pt selected 2: tangent selected
	unsigned selection;  //the current selected control vertex
	
	int controlPoly;
	float curveTime;
	float T; //parametric curve length

	
GsVec eval_bezier   ( float t);
GsVec eval_lagrange ( float t);
GsVec eval_bspline ( float u,int k);


 SnLines* _line;
 private:
	
   GsArray<GsVec> c; //curve points
 
 };

