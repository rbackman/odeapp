#pragma once
#include "controller.h"

//this is a template that makes a good starting place for a new controller.. copy this

class TemplateController : public Controller
{
public:
	TemplateController(ODEHuman* human);
	~TemplateController(void);
	bool evaluate();
private:
//stuff
	
};
