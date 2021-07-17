# config file

#parameters for physical humanoid
#Robert Backman 10/23/2010

parameters
{

	name =  "jumping" ; 
	joints = Spine1 Spine2 Shoulder Head LeftUpArm RightUpArm LeftArm RightArm LeftUpLeg RightUpLeg LeftLeg RightLeg LeftFoot RightFoot;

##				startVal		P, I, D,	grav,	PID,	maxT,  tMul, axis.(x,y,z),  vforce,	vfMult.(x,y,z),		offset.(x,y,z),			boxDim.(x,y,z),			angle.(x,y,z)
Spine1		=		0.1			5 0 0.07	0		1		200		500		1 0 1		1			-3 1 3			0 -0.5 0			1.4 0.8 1			0 0 0;
Spine2		=		0.1			5 0 0.07	0		1		200		500		1 0 1		1			-3 1 3			0 -0.4 0			1.4 0.8 1			-0.23 0 0;
Shoulder	=		0.1			5 0 0.07	0		1		200		500		1 0 1		1			3 1 3			0 -0.36 0			1.8 0.7 1			0.05 0 0;
Head		=		0.1			3 0 0.01	0		1		200		500		1 0 1		0			1 1 1			0 -0.7 0			0.7 0.7 0.7			0.01 0 0;
LeftUpArm	=		0.1			0.9 0 0.01	0		1		200		500		1 0 1		0			1 1 1			0 0.8 0			0.4 1.91 0.4			19.38 0 4.71;
RightUpArm	=		0.1			0.9 0 0.01	0		1		200		500		1 0 1		0			1 1 1			0 0.8 0			0.4 1.91 0.4			19.37 0 -4.72;
LeftArm		=		0.1			0.9 0 0.01	0		1		200		500		1 0 0		0			1 1 1			0 0.74 0			0.3 1.77 0.3			-59.55 0 0;
RightArm	=		0.1			0.9 0 0.01	0		1		200		500		1 0 0		0			1 1 1			0 0.74 0			0.3 1.77 0.3			-59.54 0 0;
LeftUpLeg	=		0.1			5 0 0.03	0		1		200		500		1 0 1		0			1 1 1			0 0.93 0			0.5 2.23 0.6			-8.98 0 -0.04;
RightUpLeg	=		0.1			5 0 0.03	0		1		200		500		1 0 1		0			1 1 1			0 0.93 0			0.5 2.23 0.6			-8.94 0 -0.01;
LeftLeg		=		0.1			5 0 0.04	0		1		200		500		1 0 0		1			1 1 1			0 1.12 0			0.4 2.23 0.5			19.25 0 0;
RightLeg	=		0.1			5 0 0.04	0		1		200		500		1 0 0		1			1 1 1			0 1.12 0			0.4 2.23 0.5			19.25 0 0;
LeftFoot	=		0.1			2 0 0.01	0		1		200		500		1 0 1		1			3 1 3			0 0 -0.35			0.6 0.3 1.1			-11.54 0 -0.45;
RightFoot	=		0.1			2 0 0.01	0		1		200		500		1 0 1		1			3 1 3			0 0 -0.35			0.6 0.3 1.1			-11.57 0 0.52;

		#jkP	jkD	jkPc	jkDc	velDSagittal	velDCoronal;
velocityParm	=		60	30	59	30	0	0;

		#stepHeight	hipSway	goalSpeed	stepheight	transitionSpeed	startSpeed	
walkParm	=		0.5	0.2	0.1	3.45;

		#comj	grav	pid	skel	shade	feetCollide	root
humanParm	=		true	true	true	true	true	0	false	true;

                #kp kd  damping  maxV
rootParm	=		301	-150	30	1;

}

end