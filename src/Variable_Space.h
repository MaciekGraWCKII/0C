#ifndef VARIABLE_SPACE_H
#define VARIABLE_SPACE_H

#include <string>
#include <vector>
#include <map>

#include "Variable.h"

class Script_Environment;

class Variable_Space
{
public:
	Variable_Space(Script_Environment& env);
	
	void new_subspace();
	void pop_subspace();
	void add_variable(const std::string& name, Variable* variable);
	void add_global_variable(const std::string& name, Variable* variable);
	void delete_global_variable(const std::string& name);

	bool is_name_used_in_current_subspace(const std::string& name) const;
	bool is_name_used_here(const std::string& name) const; //check if this particular identifier is used as a Variable name; if yes it can not be used as a Function name
	Variable& get_variable(const std::string& name);

private:
	class Subspace
	{
	public:
		void add_variable(const std::string& name, Variable* variable);//does not check if the variable name is free
		bool is_name_here(const std::string& name) const;
		void delete_variable(const std::string& name);
		Variable& get_variable(const std::string& name) throw (bool);//throws if the variable under given name does not exist

		~Subspace();
	private:
		std::map<std::string, Variable*> map;
	};

	Script_Environment& environment;
	std::vector<Subspace> subspace_vector;
	Subspace dummy;

	bool is_available(const std::string& name) const; //check if this particular identifier is NOT used in current subspace OR as a Function name
	
};

#endif