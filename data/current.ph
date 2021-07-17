# config file

#parameters for physical humanoid
#Robert Backman 10/23/2010

parameters
{

	name =  "current" ; 
	joints = Spine1 Spine2 Shoulder Head LeftUpArm RightUpArm LeftArm RightArm LeftUpLeg RightUpLeg LeftLeg RightLeg LeftFoot RightFoot;

##				startVal		P, I, D,	grav,	PID,	maxT,  tMul, axis.(x,y,z),  vforce,	vfMult.(x,y,z),		offset.(x,y,z),			boxDim.(x,y,z),			angle.(x,y,z)
Spine1		=		0.1			0 0 0.013	0		1		200		500		1 0 1		1			1 1 1			0 -0.5 0			1.4 0.8 1			0 0 0;
Spine2		=		0.1			4.1 0 0.12	0		1		200		500		1 0 1		1			1 1 1			0 -0.4 0			1.4 0.8 1			0.11 0 0;
Shoulder	=		0.1			4.1 0 0.12	0		1		200		500		1 0 1		1			1 1 1			0 -0.36 0			1.8 0.7 1			0.04 0 0;
Head		=		0.1			0.9 0 0.01	0		1		200		500		1 0 1		0			1 1 1			0 -0.7 0			0.7 0.7 0.7			0.11 0 0;
LeftUpArm	=		0.1			0.05 0 0.003	0		1		200		500		1 0 1		0			1 1 1			0 0.8 0			0.4 1.91 0.4			14.75 0 1.16;
RightUpArm	=		0.1			0.05 0 0.003	0		1		200		500		1 0 1		0			1 1 1			0 0.8 0			0.4 1.91 0.4			14.7 0 -1.09;
LeftArm		=		0.1			0.05 0 0.003	0		1		200		500		1 0 0		0			1 1 1			0 0.74 0			0.3 1.77 0.3			-51.14 0 0;
RightArm	=		0.1			0.05 0 0.003	0		1		200		500		1 0 0		0			1 1 1			0 0.74 0			0.3 1.77 0.3			-51.21 0 0;
LeftUpLeg	=		0.1			5 0 0	0		1		200		500		1 0 1		0			1 1 1			0 0.93 0			0.5 2.23 0.6			-9.91 0 0;
RightUpLeg	=		0.1			5 0 0	0		1		200		500		1 0 1		0			1 1 1			0 0.93 0			0.5 2.23 0.6			-9.83 0 -0.01;
LeftLeg		=		0.1			5 0 0.04	0		1		200		500		1 0 0		1			1 1 1			0 1.12 0			0.4 2.23 0.5			19.19 0 0;
RightLeg	=		0.1			5 0 0.04	0		1		200		500		1 0 0		1			1 1 1			0 1.12 0			0.4 2.23 0.5			19.19 0 0;
LeftFoot	=		0.1			1.95 0 0.039	0		1		200		500		1 0 1		1			3 1 3			0 0 -0.35			0.6 0.3 1.1			-11.11 0 -0.49;
RightFoot	=		0.1			1.95 0 0.039	0		1		200		500		1 0 1		1			3 1 3			0 0 -0.35			0.6 0.3 1.1			-11.18 0 0.54;

		#jkP	jkD	jkPc	jkDc	velDSagittal	velDCoronal;
velocityParm	=		60	30	59	30	0	0;

		#stepHeight	hipSway	goalSpeed	stepheight	transitionSpeed	startSpeed	
walkParm	=		0.6	0.5	0	3;

		#comj	grav	pid	skel	shade	feetCollide	root
humanParm	=		true	true	true	true	false	0	true	false;

                #kp kd  damping  maxV
rootParm	=		601	-150	30	0;

}

end