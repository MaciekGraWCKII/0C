#ifndef SCRIPT_ENVIRONMENT_H
#define SCRIPT_ENVIRONMENT_H

#include "Function_Space.h"
#include "Variable_Space.h"
#include "Variable_Factory.h"

class Communicator;

class Script_Environment
{
public:
	Script_Environment(Communicator& comms);

	Function_Space& get_function_space();
	Variable_Space& get_variable_space();
	Variable_Factory& get_variable_factory();
	Communicator& get_communicator();

	bool is_name_used_in_variable_space(const std::string& name) const;
	bool is_name_used_in_function_space(const std::string& name) const;
	bool is_name_used_in_variable_factory(const std::string& name) const;

private:
	Function_Space function_space;
	Variable_Space variable_space;
	Variable_Factory variable_factory;
	Communicator& comms;

};

#endif
