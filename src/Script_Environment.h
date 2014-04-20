#ifndef SCRIPT_ENVIRONMENT_H
#define SCRIPT_ENVIRONMENT_H

#include "Function_Space.h"
#include "Variable_Space.h"

class Script_Environment
{
public:
	Function_Space& get_function_space() const;
	Variable_Space& get_variable_space() const;

	bool is_name_used_in_variable_space(const std::string& name) const;
	bool is_name_used_in_function_space(const std::string& name) const;
};

#endif
