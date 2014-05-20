#include "Variable_Factory.h"

Variable_Factory::Variable_Creator::Variable_Creator(
	const std::string& type) : type(type)
{

}

Variable_Factory::Variable_Creator::~Variable_Creator()
{

}

Variable_Factory::Simple_Variable_Creator::Simple_Variable_Creator(
	const std::string& type, 
	const std::list<std::pair<std::string, std::string>>& type_name_list) : 
	Variable_Creator(type), type_name_list(type_name_list)
{

}

Variable* Variable_Factory::Simple_Variable_Creator::new_variable(Variable_Factory& var_factory, 
	const std::string& additional_information)
{
	return var_factory.new_simple_variable(this->type, *this);
}

Variable_Factory::Variable_Factory()
{
	std::list<std::pair<std::string, std::string>> list;

	Simple_Variable_Creator *svc = new Simple_Variable_Creator(TYPE_INTEGER, list);
	this->creator_map.insert(std::make_pair(TYPE_INTEGER, svc));

	svc = new Simple_Variable_Creator(TYPE_STRING, list);
	this->creator_map.insert(std::make_pair(TYPE_STRING, svc));
}

bool Variable_Factory::is_name_used_here(const std::string& name) const
{
	std::string temp = name.substr(0, name.find_first_of('<'));

	std::map<std::string, Variable_Creator*>::const_iterator it = 
		this->creator_map.find(temp);

	if(it != this->creator_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Variable* Variable_Factory::new_variable(const std::string& type, const std::string& additional_information)
{
	std::map<std::string, Variable_Creator*>::iterator it = 
		this->creator_map.find(type);

	if(it != this->creator_map.end())
	{
		return it->second->new_variable(*this, additional_information);
	}
	else
	{
		//TODO
		//no such type!
	}

	return NULL;
}

Variable* Variable_Factory::new_simple_variable(const std::string& type, 
	Simple_Variable_Creator& simple_creator)
{
	//Check for default types
	if(type == TYPE_INTEGER)
	{
		return new Integer();
	}
	
	if(type == TYPE_STRING)
	{
		return new String();
	}

	return NULL;
}

Variable_Factory::~Variable_Factory()
{
	std::map<std::string, Variable_Creator*>::iterator it = creator_map.begin();
	std::map<std::string, Variable_Creator*>::iterator it_end = creator_map.end();

	for(it; it != it_end; ++it)
	{
		delete it->second;
	}
}