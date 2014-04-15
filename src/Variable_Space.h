#ifndef VARIABLE_SPACE_H
#define VARIABLE_SPACE_H

#include <string>

#include "Variable.h"

class Variable_Space
{
public:
	
	void new_subspace();
	void pop_subspace();
	void add_variable(const std::string& name, Variable* variable);
	void add_global_variable(const std::string& name, Variable* variable);
	void delete_variable(const std::string& name);

	Variable& get_variable(const std::string& name) const;

private:
	
};

#endif