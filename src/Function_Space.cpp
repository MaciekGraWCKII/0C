#include "Function_Space.h"
#include "Script_Environment.h"

Function_Space::Function_Space(Script_Environment& env) : environment(env)
{

}

Variable* Function_Space::call_function(const std::string& name, 
	const Function::Function_Arguments& args) const
{
	const std::list<Function*>& list = this->fun_map.find(name)->second;
	std::list<Function*>::const_iterator it = list.begin();
	std::list<Function*>::const_iterator it_end = list.end();
	
	while(it != it_end)
	{
		if((*it)->are_arguments_valid(args))
		{
			return (*it)->execute(args);
		}

		++it;
	}
	
	return NULL;
}

bool Function_Space::is_function_here(const std::string& name, 
	const Function::Function_Arguments& args) const
{
	std::map<std::string, std::list<Function*>>::const_iterator map_it = this->fun_map.find(name);
	
	if(map_it == this->fun_map.end())
	{
		return false;
	}
	else
	{	
		const std::list<Function*>& list = map_it->second;
		std::list<Function*>::const_iterator it;
		std::list<Function*>::const_iterator it_end = list.end();
	
		for(it = list.begin(); it != it_end; ++it)
		{
			if((*it)->are_arguments_valid(args))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool Function_Space::is_function_here(const std::string& name,
	const Function::Function_Arguments_Dummy& args_dummy) const
{
	std::map<std::string, std::list<Function*>>::const_iterator map_it = this->fun_map.find(name);
	
	if(map_it == this->fun_map.end())
	{
		return false;
	}
	else
	{	
		const std::list<Function*>& list = map_it->second;
		std::list<Function*>::const_iterator it;
		std::list<Function*>::const_iterator it_end = list.end();
	
		for(it = list.begin(); it != it_end; ++it)
		{
			if((*it)->are_arguments_valid(args_dummy))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool Function_Space::is_name_here(const std::string& name) const
{
	std::map<std::string, std::list<Function*>>::const_iterator it = this->fun_map.find(name);

	if(it != this->fun_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Function_Space::add_function(const std::string& name, Function* fun)
{
	if(this->is_name_available(name))
	{
		std::map<std::string, 
			std::list<Function*>>::const_iterator it = this->fun_map.find(name);

		std::list<Function*> list;

		if(it != this->fun_map.end())
		{
			list = it->second;
			std::list<Function*>::const_iterator list_it;
			std::list<Function*>::const_iterator it_end = list.end();
			
			for(list_it = list.begin(); list_it != it_end; ++list_it)
			{
				if((**list_it) == *fun)
				{
					//TODO error: function collision!
					//Must throw here, lest the colliding function will be added anyway
				}
			}

			list.push_back(fun);
		}
		else
		{
			list.push_back(fun);

			this->fun_map.insert(std::make_pair(
				name, list));
		}
	}
	else
	{
		//TODO name is already used!
	}
}

void Function_Space::delete_function(const std::string& name, 
	const Function::Function_Arguments_Dummy& args_dummy)
{
	std::map<std::string, std::list<Function*>>::iterator it = this->fun_map.find(name);

	if(it != this->fun_map.end())
	{
		std::list<Function*>::iterator list_it = it->second.begin();
		std::list<Function*>::iterator it_end = it->second.end();

		for(list_it; list_it != it_end; ++list_it)
		{
			if((**list_it).are_arguments_valid(args_dummy))
			{
				it->second.erase(list_it);
				return;
			}
		}

		//TODO there is no such function (by args_dummy)
	}
	else
	{
		//TODO there is no such function (by name)
	}
}

Function_Space::~Function_Space()
{
	std::map<std::string, std::list<Function*>>::iterator map_it = this->fun_map.begin();
	std::list<Function*>::iterator list_it;

	for(map_it; map_it != this->fun_map.end(); ++map_it)
	{
		for(list_it = map_it->second.begin(); list_it != map_it->second.end(); ++list_it)
		{
			delete *list_it;
		}
	}
}

bool Function_Space::is_name_available(const std::string& name) const
{
	return environment.is_name_used_in_variable_space(name);
}