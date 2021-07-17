#include "curve.h"

Curve::~Curve()
{

	subCvs.remove(0,subCvs.size());
	delete _line;

}

Curve::Curve()
{
	
	 _line = new SnLines();
	cCol.set(0.2f,0.2f,0.2f);
	cpCol.set(1.0f,0.0f,0.0f);
	curveMode = BEZIERPIECES;
	flat = 1;
	selection = 0;
	divPerPair = 3;
	closed = 0;
	selectionState = 0;
	controlPoly = 1;
	T=1;
	first = 1;
	vis = true;
}
Tang::Tang(GsVec* point)
{
	pt = point;
	in = GsVec(1,0,0);
	out = GsVec(1,0,0);
	locked = 1;
	free  = 0;
	flat  = 1;
	sharp = 2;
	tangSelected=0;
}
void Curve::addPoint(GsVec pt)
{
   p.push(pt);
   update();
}
void Curve::clear()
{
	p.remove(0,p.size());
	t.remove(0,t.size());
	_line->init();
	c.remove(0,c.size());
}
void Curve::update()
{

	c.remove(0,c.size());
	for(float t=0;t<1;t+=0.01f)
	{
		c.push(eval_bezier(t));
	}
	_line->init();
	_line->begin_polyline();
	_line->color(GsColor(0,255,0));
	
	for(int j=0;j<c.size();j++)
	{
		_line->push(c.get(j));
	}

	 _line->end_polyline ();
}

void Curve::updateTangents()
{		
	for(int i=0;i<t.size();i++)
	{
		if(!t[i]->free)
		{ createTang(i); }

		float dtin=0.0001f, dtout=0.0001f; //to make sure vec doesnt go to zero
		if(i>0) 
			dtin += (p[i].x - p[i-1].x ) / t[i]->sharp;
		if(i<p.size()-1)
			dtout += ( p[i+1].x - p[i].x) / t[i]->sharp;
		
		if(t[i]->free)
		{
			float dti = abs( t[i]->po.x - p[i].x );
			float dto = abs( p[i].x - t[i]->pi.x );
			if(  dto > dtout && i < t.size() - 1 )
			{
				float dy = abs((p[i+1].y - p[i].y) / t[i]->sharp);
				float ln = sqrtf(dy*dy+dtout*dtout);
				t[i]->out.len(ln);
			}
			else if(dti > dtin && i > 0)
			{
				float dy = abs((p[i-1].y - p[i].y) / t[i]->sharp);
				float ln = sqrtf(dy*dy+dtout*dtout);
				t[i]->out.len(ln);
			}
		}
		else{
			t[i]->in.len(dtin);
			t[i]->out.len(dtout); 
		}
		t[i]->pi.set(p[i] - t[i]->in);
		t[i]->po.set(p[i] + t[i]->out);
		
	}
}


void Curve::createTang(int i)
{
	if(i<(int)p.size()-1 && i > 0)
	{
		t[i]->out = p[i+1]-p[i-1];
		t[i]->in = t[i]->out; 
	}
	else 
	{
		t[i]->in.set(1,0,0);
		t[i]->out.set(1,0,0);
	}
}
void  Curve::initTang()
{
	t.remove(0,t.size());
	for(int i=0;i<p.size();i++)
	{
		Tang* tng = new Tang(&p[i]);
		tng->idx = i;	
		tng->flat=0;
		tng->locked = 1;
		t.push(tng);
		createTang(i);
	}
	update();
}
void Curve::makeSub()
{
	//initTang();
	subCvs.remove(0,subCvs.size());
	for(int i=1;i<p.size();i++)
	{
		Curve* newCv = new Curve();
		newCv->curveMode = BEZIER;
		newCv->controlPoly = 0;
		newCv->p.push(p[i-1]);
		newCv->p.push(p[i-1] + t[i-1]->out);
		newCv->p.push(p[i]   - t[i]->in);
		newCv->p.push(p[i]);
		subCvs.push(newCv);
	}
}

GsVec Curve::eval_lagrange ( float t)
{
int n = p.size()-1;
t = t * n; //t 0 -> n
GsVec pt;

for(int i=0; i <= n; i++)
	{
		float bi=1.0;
		for ( int j=0; j <= n; j++ )
		{
		
				if(i==j){/*skip this */}
				else
				{
					bi*=(t-j)/(i-j);
				}
		}
		pt = p[i]*(float)bi + pt;
	}

return pt;
}
long int fact(int n)
 {
  if (n<=1)
	return(1);
  else
	n=n*fact(n-1);
	return(n);
 }
float C(int n, int i)
{
return (float)(fact(n) / ( fact(i) * fact(n-i) ));
}

float B(int n, int i, float t)
{
return C(n,i)*pow(t,i)*pow(1-t,n-i);
}

GsVec Curve::eval_bezier ( float t)
{
int n = p.size();
GsVec pt;
	for(int i=0; i<n; i++)
	{
		pt= p[i]*B(n-1,i,t) + pt;
	}
return pt;
}

float N ( int i, int k, float u )
 {
	float ui=float(i);
	if ( k==1 ) 
	 return ui<=u && u<ui+1? 1.0f:0;
	else  
	return   ((u-ui)/(k-1)) * N(i,k-1,u) + ((ui+k-u)/(k-1)) * N(i+1,k-1,u);
 }


GsVec Curve::eval_bspline ( float u,int k )
{
	//for cubic start u at 3
	//knot vector [0,1,2,3,4...10] 
	//t>[0,1] --> U>[3,7] = u = 4*t+3
	int n = p.size()-1;
	GsVec pt;
	for(int i=0; i <= n; i++)
	{
		pt = p[i]*N(i,k+1,u) + pt;
	}
return pt;
}

