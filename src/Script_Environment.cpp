#include "Script_Environment.h"

Script_Environment::Script_Environment() : function_space(*this), variable_space(*this)
{

}

Function_Space& Script_Environment::get_function_space()
{
	return this->function_space;
}

Variable_Space& Script_Environment::get_variable_space()
{
	return this->variable_space;
}

bool Script_Environment::is_name_used_in_function_space(const std::string& name) const
{
	return this->function_space.is_name_here(name);
}

bool Script_Environment::is_name_used_in_variable_space(const std::string& name) const
{
	return this->variable_space.is_name_used_here(name);
}

