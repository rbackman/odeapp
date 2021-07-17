#include "balance_controller.h"
#include "ode_human.h"

BalanceController::BalanceController(ODEHuman* human):Controller(human)
{
		h = human;
		scaleOutput = 0.008f;
		Kp_z=0;
		Kd_z=0;
		Ki_z=0;
		integral_z=0;
		previousOffset_z=0;
}

BalanceController::~BalanceController(void)
{
}
void BalanceController::setGains(float p,float i, float d,float scale)
{
	Kp_z = p;
	Ki_z = i;

Kd_z = d;
scaleOutput = scale;
}
bool BalanceController::evaluate()
{
bool valid = true;

return valid;
}


