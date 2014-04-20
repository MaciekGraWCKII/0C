#include "Variable_Space.h"
#include "Script_Environment.h"

Variable_Space::Variable_Space(Script_Environment& env) : environment(env)
{
	this->subspace_vector.push_back(this->dummy);
}

void Variable_Space::new_subspace()
{
	this->subspace_vector.push_back(this->dummy);
}

void Variable_Space::pop_subspace()
{
	if(this->subspace_vector.size() <= 1)
	{
		//TODO we can not delete global Subspace!
	}
	else
	{
		this->subspace_vector.pop_back();
	}
}

void Variable_Space::add_variable(const std::string& name, Variable* variable)
{
	if(this->is_available(name))
	{
		this->subspace_vector.back().add_variable(name, variable);
	}
	else
	{
		//TODO name already used!
	}
}

void Variable_Space::add_global_variable(const std::string& name, Variable* variable)
{
	if(this->is_available(name))
	{
		if(this->subspace_vector.front().is_name_here(name))
		{
			//TODO name already used!
		}
		else
		{
			this->subspace_vector.front().add_variable(name, variable);
		}
	}
	else
	{
		//TODO name already used!
	}
}

void Variable_Space::delete_global_variable(const std::string& name)
{
	this->subspace_vector.front().delete_variable(name);
}

bool Variable_Space::is_name_used_in_current_subspace(const std::string& name) const
{
	return this->subspace_vector.back().is_name_here(name);
}

bool Variable_Space::is_name_used_here(const std::string& name) const
{
	for(std::vector<Subspace>::const_iterator it = this->subspace_vector.begin(); 
		it != this->subspace_vector.end(); ++it)
	{
		if(it->is_name_here(name))
		{
			return true;
		}
	}

	return false;
}

Variable& Variable_Space::get_variable(const std::string& name)
{
	for(std::vector<Subspace>::reverse_iterator it = this->subspace_vector.rbegin(); 
		it != this->subspace_vector.rend(); ++it)
	{
		try
		{
			return it->get_variable(name);
		}
		catch (bool)
		{
			continue;
		}
	}
}

Variable_Space::Subspace::~Subspace()
{
	for(std::map<std::string, Variable*>::iterator it = this->map.begin();
		it != this->map.end(); ++it)
	{
		delete it->second;
	}
}

void Variable_Space::Subspace::add_variable(
	const std::string& name, Variable* variable)
{
	this->map.insert(std::make_pair(name, variable));
}

bool Variable_Space::Subspace::is_name_here(const std::string& name) const
{
	return this->map.find(name) != this->map.end();
}

void Variable_Space::Subspace::delete_variable(const std::string& name)
{
	if(this->map.erase(name)/* != 0 */)
	{
		
	}
	else
	{
		//TODO throw?
	}
}

Variable& Variable_Space::Subspace::get_variable(const std::string& name) throw (bool)
{
	std::map<std::string, Variable*>::iterator it = this->map.find(name);

	if(it != this->map.end())
	{
		return *it->second;
	}
	else
	{
		throw false;
	}
}

bool Variable_Space::is_available(const std::string& name) const
{
	if(environment.is_name_used_in_function_space(name))
	{
		return false;
	}
	else
	{
		return !this->is_name_used_in_current_subspace(name);
	}
}