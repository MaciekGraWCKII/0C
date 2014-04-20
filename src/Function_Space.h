#ifndef FUNCTION_SPACE_H
#define FUNCTION_SPACE_H

#include <string>
#include <map>
#include <list>

#include "Function.h"
#include "Variable.h"

//class Script_Environment;

class Function_Space
{
public:
	Function_Space(Script_Environment& env);

	Variable* call_function(const std::string& name,
		const Function::Function_Arguments& args) const; //will not check if the function exists
	bool is_function_here(const std::string& name,
		const Function::Function_Arguments& args) const;
	bool is_function_here(const std::string& name,
		const Function::Function_Arguments_Dummy& args_dummy) const;
	bool is_name_here(const std::string& name) const;
	void add_function(const std::string& name, Function* fun);
	void delete_function(const std::string& name,
		const Function::Function_Arguments_Dummy& args_dummy);

	~Function_Space();
private:
	std::map<std::string, std::list<Function*>> fun_map;
	Script_Environment& environment;

	bool is_name_available(const std::string& name) const;
};

#endif