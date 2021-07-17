#include "controller_template.h"


TemplateController::TemplateController(ODEHuman* human):Controller(human)
{

}

TemplateController::~TemplateController(void)
{

}

bool TemplateController::evaluate()
{
bool valid = Controller::evaluate();
	

return valid;
}


