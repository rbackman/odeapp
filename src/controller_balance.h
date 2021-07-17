#pragma once
#include "controller.h"

class BalanceController : public Controller
{
public:
	BalanceController(ODEHuman* human);
	~BalanceController(void);
	bool evaluate();
	void setGains(float p,float i, float d,float scale);
private:
	float previousOffset_z;
	float integral_z;
	float scaleOutput;
	float Ki_z,Kd_z,Kp_z;
	float maxT;

};
