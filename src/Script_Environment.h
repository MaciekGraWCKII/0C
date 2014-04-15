#ifndef SCRIPT_ENVIRONMENT_H
#define SCRIPT_ENVIRONMENT_H

#include "Function_Space.h"
#include "Variable_Space.h"

class Script_Environment
{
public:
	Function_Space& get_function_space();
	Variable_Space& get_variable_space();

};

#endif
