#include "Script_Environment.h"

Script_Environment::Script_Environment(Communicator& comms) : comms(comms), function_space(*this), variable_space(*this)
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

Variable_Factory& Script_Environment::get_variable_factory()
{
	return this->variable_factory;
}

Communicator& Script_Environment::get_communicator()
{
	return this->comms;
}

bool Script_Environment::is_name_used_in_function_space(const std::string& name) const
{
	return this->function_space.is_name_here(name);
}

bool Script_Environment::is_name_used_in_variable_space(const std::string& name) const
{
	return this->variable_space.is_name_used_here(name);
}

bool Script_Environment::is_name_used_in_variable_factory(const std::string& name) const
{
	return this->variable_factory.is_name_used_here(name);
}
