#pragma once
#include "common.h"


class Model
{
	public:
		Model(GsVec Pt);
		
		void setPosition(GsVec p);
		void setRotation(GsQuat q);
		void rotate(float radx,float rady,float radz);
		void setVisible(bool t);
		void setColor(float r, float g, float b);
		
	protected:
		SnGroup* grp;
		GsMat m;
		GsVec pos;
		GsQuat rot;
		SnModel* node;
		GsModel* model;
		SnTransform* tfm;
	public:
		GsModel* getModel(){return model;}
		SnTransform* getTfm(){return tfm;}
		SnGroup* getGrp(){return grp;}

};

class Arrow: public Model
{
	public:
		Arrow(GsVec Pt, GsVec dir);
		void setDirection(GsVec d);
	private:
		GsVec direction;
};

class Ball: public Model
{
	public:
		Ball(GsVec Pt, float rad);
		void setRadius(float r);
	private:
		float rad;
};
class Box: public Model
{
	public:
		Box(GsVec Pt, GsVec sz);

};

