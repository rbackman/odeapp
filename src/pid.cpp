#include "pid.h"
#include <gsim/gs_quat.h>
#include <gsim/gs_euler.h>
PID::PID(ODEJoint* jt)
{
	
	joint=jt;

	previousError=0;
	integral=0;
	previousError2=0;
	integral2=0;
	
	//some default values which should be set by the file manager
	Kp = 0.92f;
	Kd = 0.01f;
	Ki = 0.0f;
	torqueMultiplier = 500.0f;
	maxT = 200.0f;
	
}



PID::~PID(void)
{

}


bool PID::evaluate()
{
	bool valid = true;
	float dt = TIME_STEP;

GsVec torque = GsVec(0,0,0);
dJointID jid = joint->jid;

switch(joint->jointType)
	{
	case BALL:
		{
			
			float setpoint = joint->ax;
			float position = dJointGetAMotorAngle(joint->m_jid,0);  
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;

			float setpoint2 =	joint->ay;
			float position2 = dJointGetAMotorAngle(joint->m_jid,1);  
			float error2 = setpoint2 - position2;
			integral2 = integral2 + (error2*dt);
			float derivative2 = (error2 - previousError2)/dt;
			float output2 = (Kp*error2) + (Ki*integral2) + (Kd*derivative2);
			previousError2 = error2;

			float setpoint3 = joint->az;
			float position3 = dJointGetAMotorAngle(joint->m_jid,2);  
			float error3 = setpoint3 - position2;
			integral3 = integral3 + (error3*dt);
			float derivative3 = (error3 - previousError3)/dt;
			float output3 = (Kp*error3) + (Ki*integral3) + (Kd*derivative3);
			previousError3 = error3;
			torque.x = min(maxT,output)*torqueMultiplier;
			torque.y = min(maxT,output2)*torqueMultiplier;
			torque.z = min(maxT,output3)*torqueMultiplier;
		}
		break;
	case HINGE_X:
		{
			float setpoint = joint->ax;
			// gsout<<joint->name()<<" :"<<GS_TODEG( setpoint)<<gsnl;
			float position = dJointGetHingeAngle(jid);
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;
			torque.x = min(maxT,output)*torqueMultiplier;
			
		}break;
	case HINGE_Y:
		{
			float setpoint = joint->ay;
			float position = dJointGetHingeAngle(jid);
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;
		
			torque.y = min(maxT,output)*torqueMultiplier;
			
		}break;
	case HINGE_Z:
		{
			float setpoint = joint->az;
			float position = dJointGetHingeAngle(jid);
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;
		
			torque.z = min(maxT,output)*torqueMultiplier;
		}break;
	case UNI_XY:
		{
			float setpoint = joint->ax;
			float position = dJointGetUniversalAngle1(jid);  
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;

			float setpoint2 = joint->ay;
			float position2 = dJointGetUniversalAngle2(jid);  
			float error2 = setpoint2 - position2;
			integral2 = integral2 + (error2*dt);
			float derivative2 = (error2 - previousError2)/dt;
			float output2 = (Kp*error2) + (Ki*integral2) + (Kd*derivative2);
			previousError2 = error2;

			torque.x = min(maxT,output)*torqueMultiplier;
			torque.y = min(maxT,output2)*torqueMultiplier;
			
		}break;
	case UNI_XZ:
		{
			float setpoint = joint->ax;
			float position = dJointGetUniversalAngle1(jid);  
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;

			float setpoint2 = joint->az;
			float position2 = dJointGetUniversalAngle2(jid);  
			float error2 = setpoint2 - position2;
			integral2 = integral2 + (error2*dt);
			float derivative2 = (error2 - previousError2)/dt;
			float output2 = (Kp*error2) + (Ki*integral2) + (Kd*derivative2);
			previousError2 = error2;

			torque.x = min(maxT,output)*torqueMultiplier;
			torque.z = min(maxT,output2)*torqueMultiplier;
		}break;
	case UNI_YZ:
		{
			float setpoint = joint->ay;
			float position = dJointGetUniversalAngle1(jid);  
			float error = setpoint - position;
			integral = integral + (error*dt);
			float derivative = (error - previousError)/dt;
			float output = (Kp*error) + (Ki*integral) + (Kd*derivative);
			previousError = error;

			float setpoint2 = joint->az;
			float position2 = dJointGetUniversalAngle2(jid);  
			float error2 = setpoint2 - position2;
			integral2 = integral2 + (error2*dt);
			float derivative2 = (error2 - previousError2)/dt;
			float output2 = (Kp*error2) + (Ki*integral2) + (Kd*derivative2);
			previousError2 = error2;
		
			torque.y = min(maxT,output)*torqueMultiplier;
			torque.z = min(maxT,output2)*torqueMultiplier;
		}break;
	}
	joint->addTorque(torque);

	return valid;
}


