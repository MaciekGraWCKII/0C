#ifndef VARIABLE_FACTORY_H
#define VARIABLE_FACTORY_H

#include <string>
#include <map>

#include "Variable.h"

class Variable_Factory
{
public:
	class Variable_Factory_Helper
	{
	protected:
		std::string type;

	public:
		Variable_Factory_Helper(const std::string& type);

		Variable* new_variable();
	};

	Variable* new_variable(const std::string& type);

private:
	std::map<std::string, Variable_Factory_Helper*> vfh_map;
	
	Variable* new_simple_variable(const Variable_Factory_Helper& vfh);

};

#endif