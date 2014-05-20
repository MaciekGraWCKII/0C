#ifndef VARIABLE_FACTORY_H
#define VARIABLE_FACTORY_H

#include <string>
#include <map>
#include <list>

#include "Variable.h"

class Variable_Factory
{
public:
	class Variable_Creator
	{
	protected:
		std::string type;

	public:
		Variable_Creator(const std::string& type);

		virtual Variable* new_variable(Variable_Factory& var_factory,
			const std::string& additional_information = "") = 0;
			//additional_information are closed in brackets <> just after the type (without space)
			//so if we encounter "Vector<int>"
			//"Vector" will be the type
			//and "<int>" will be the additional_information

		virtual ~Variable_Creator();
	};

	class Simple_Variable_Creator : public Variable_Creator
	{
	public:
		Simple_Variable_Creator(const std::string& type, 
			const std::list<std::pair<std::string, std::string>>& type_name_list);

		Variable* new_variable(Variable_Factory& var_factory, 
			const std::string& additional_information = "");
			//For this particular Creator additional_information should be empty
	private:
		std::list<std::pair<std::string, std::string>> type_name_list;
	};
	
	Variable_Factory();

	bool is_name_used_here(const std::string& name) const;
	Variable* new_variable(const std::string& type, const std::string& additional_information);

	~Variable_Factory();

private:
	std::map<std::string, Variable_Creator*> creator_map;
	
	Variable* new_simple_variable(const std::string& type, 
		Simple_Variable_Creator& simple_creator);

};

#endif
