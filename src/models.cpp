#include "models.h"



Model::Model(GsVec Pt)
{
	pos = Pt;
	tfm = new SnTransform;
	grp = new SnGroup;
	grp->separator(true);
	grp->add(tfm);
}


void Model::setPosition(GsVec p)
{
	pos = p;
	compose ( rot, pos, m );
	tfm->get().set(m);	
}
void Model::setVisible(bool t)
{
	grp->visible(t);
}

void Model::setRotation(GsQuat q)
{
	rot = q;
	compose ( rot, pos, m );
	tfm->get().set(m);	
}
void Model::setColor(float r, float g, float b)
{
	GsMaterial m;
	m.diffuse.set(r,g,b);
	model->set_one_material(m);
}
void Model::rotate(float radx,float rady,float radz)
{
	GsMat m;
	GsQuat rot;
	gs_rot(gsXYZ,rot,radx,rady,radz);
	compose ( rot, pos, m );
	tfm->get().set(m);	
}





Ball::Ball(GsVec Pt, float r) : Model(Pt)
{
	rad = r;
	model = new GsModel;
	model->make_sphere (GsVec(0,0,0),r,12,1);
	setColor(0,0,1);
	node = new SnModel(model);
	grp->add(node);
	setVisible(false);

}
void Ball::setRadius(float r)
{
	rad = r;
	model->make_sphere(GsVec(0,0,0),r,12,1);
}

Arrow::Arrow(GsVec Pt, GsVec dir):Model(Pt)
{
direction = dir;
model = new GsModel;
model->load_obj("../models/arrow.obj");
node = new SnModel(model);
grp->add(node);

}

void Arrow::setDirection(GsVec d)
{
	direction = d;
	
}

Box::Box(GsVec Pt, GsVec sze):Model(Pt)
{

	model = new GsModel;
	GsVec a = GsVec(-sze.x/2.0f,-sze.y/2.0f,-sze.z/2.0f);
	GsVec b = GsVec(sze.x/2.0f,sze.y/2.0f,sze.z/2.0f);
	model->make_box(GsBox(a,b));
	GsMaterial newMat;
	newMat.diffuse.set(0,0,255);
	model->set_one_material(newMat);
	node = new SnModel(model);
	grp->add(node);
	setPosition(Pt);
}



	
		