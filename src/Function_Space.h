#ifndef FUNCTION_SPACE_H
#define FUNCTION_SPACE_H

#include <string>
#include <map>
#include <list>

#include "Function.h"
#include "Variable.h"

class Function_Space
{
public:

	Variable call_function(const std::string& name, const Function::Function_Arguments& args) const;
	bool is_function_there(const std::string& name, const Function::Function_Arguments& args) const;
	bool is_function_there(const std::string& name, const Function::Function_Arguments_Dummy& args) const;
	bool is_name_taken(const std::string& name) const;
	void add_function(std::string& name, Function* fun);
	void delete_function(std::string& name, const Function::Function_Arguments& args);

private:
	std::map<std::string, std::list<Function*>> fun_map;

};

#endif