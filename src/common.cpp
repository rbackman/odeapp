#include "common.h"


float boundToRange(float in,float min,float max)
{
if(in<min)return min;
if(in>max)return max;
return in;
}

void swapP(void * pOne, void * pTwo)
{
  void * pTemp;
  pTemp = pOne;
  pOne = pTwo;
  pTwo = pTemp;
}


GsVec interp(GsVec start, GsVec end , float t)
{
return start + (end - start)*t;;
}
float interp(float start,float end, float t)
{
return start + (end-start)*t;
}
float interp_cubic ( float t, float tmin, float tmax )
 {
   t = (t-tmin)/(tmax-tmin);    // normalize t to [0,1]
   t=-(2.0f*(t*t*t)) + (3.0f*(t*t));  // cubic spline
   return t*(tmax-tmin) + tmin; // scale back
 }

GsVec GetClosetPoint(GsVec A, GsVec B, GsVec P, bool segmentClamp)
{
	GsVec AP = P - A;
	GsVec AB = B - A;
	float ab2 = AB.x*AB.x + AB.y*AB.y;
	float ap_ab = AP.x*AB.x + AP.y*AB.y;
	float t = ap_ab / ab2;
	if (segmentClamp)
	{
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;
	}
	GsVec Closest = A + AB * t;
	return Closest;
}
