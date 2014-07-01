#include "Function.h"

#include "Variable_Space.h"
#include "Script_Environment.h"

Function::Function_Arguments::Function_Arguments(
	const unsigned int size, Variable** variable_array) : size(size),
	variable_array(variable_array)
{

}

Variable& Function::Function_Arguments::operator[](const unsigned int index)
{
	this->throw_if_out_of_bounds(index);

	return *(this->variable_array[index]);
}

const Variable& Function::Function_Arguments::operator[](const unsigned int index) const
{
	this->throw_if_out_of_bounds(index);

	return *(this->variable_array[index]);
}

unsigned int Function::Function_Arguments::get_size() const
{
	return this->size;
}

Function::Function_Arguments::~Function_Arguments()
{
	for(unsigned int i = 0; i < this->size; ++i)
	{
		//TODO odkomentowac!!!
		//delete this->variable_array[i];
	}

	delete[] variable_array;
}

void Function::Function_Arguments::throw_if_out_of_bounds(const unsigned int index) const
{
	if(index >= this->size)
	{
		//TODO: throw 'out of bounds' exception
	}
}

Function::Function_Arguments_Dummy::Function_Arguments_Dummy(
	const unsigned int size, std::string* name_array, std::string* type_array) 
	: size(size), name_array(name_array), type_array(type_array)
{

}

Function::Function_Arguments_Dummy::Function_Arguments_Dummy(const unsigned int size,
	std::string* type_array) : size(size), name_array(NULL), type_array(type_array)
{

}

unsigned int Function::Function_Arguments_Dummy::get_size() const
{
	return size;
}

const std::string& Function::Function_Arguments_Dummy::get_name_at(
	const unsigned int index) const
{
	this->throw_if_out_of_bounds(index);

	return this->name_array[index];
}

const std::string& Function::Function_Arguments_Dummy::get_type_at(
	const unsigned int index) const
{
	this->throw_if_out_of_bounds(index);

	return this->type_array[index];
}

bool Function::Function_Arguments_Dummy::operator==(const Function_Arguments& args) const
{
	if(this->size != args.get_size())
	{
		return false;
	}
	else
	{
		for(unsigned int i = 0; i < this->size; ++i)
		{
			if(this->type_array[i] != args[i].get_type())
			{
				return false;
			}
		}
	}

	return true;
}

bool Function::Function_Arguments_Dummy::operator==(const Function_Arguments_Dummy& args_dummy) const
{
	if(this->size != args_dummy.get_size())
	{
		return false;
	}
	else
	{
		for(unsigned int i = 0; i < this->size; ++i)
		{
			if(this->type_array[i] != args_dummy.get_type_at(i))
			{
				return false;
			}
		}
	}

	return true;
}

Function::Function_Arguments_Dummy::~Function_Arguments_Dummy()
{
	delete[] this->name_array;
	delete[] this->type_array;
}

void Function::Function_Arguments_Dummy::throw_if_out_of_bounds(const unsigned int index) const
{
	if(index > this->size)
	{
		//TODO: throw 'index out of bounds' exception
	}
}

Function::Function(
	const std::string& return_value, Function::Function_Arguments_Dummy* args_dummy, 
	Executable_Block_Of_Code* exec) : return_value(return_value), 
	args_dummy(args_dummy), exec(exec)
{
	if(this->args_dummy == NULL || this->exec == NULL)
	{
		//TODO: incorrect parameters!
	}
}

bool Function::are_arguments_valid(const Function_Arguments& args_to_check) const
{
	return this->args_dummy->operator==(args_to_check);
}

bool Function::are_arguments_valid(const Function_Arguments_Dummy& args_to_check) const
{
	return this->args_dummy->operator==(args_to_check);
}

const std::string& Function::get_return_type() const
{
	return this->return_value;
}

bool Function::operator==(const Function& other) const
{
	return args_dummy == other.args_dummy;
}

Variable* Function::execute(const Function_Arguments& args, 
							Script_Environment& environment) const
{
	if(this->are_arguments_valid(args))
	{
		Variable* temp;

		this->prep_subspace_and_args(args, environment);
		temp = this->exec->execute(environment);
		this->delete_subspace(args, environment);

		return temp;
	}
	else
	{
		//TODO incorrect arguments!
	}
}

Function::~Function()
{
	delete this->exec;
	delete this->args_dummy;
}

void Function::prep_subspace_and_args(const Function_Arguments& args, 
	Script_Environment& env) const
{
	Variable_Space& var_space = env.get_variable_space();
	var_space.new_subspace();

	for(int i = 0; i < args.get_size(); ++i)
	{
		var_space.add_variable(this->args_dummy->get_name_at(i), args[i].replicate());
	}
}

void Function::delete_subspace(const Function_Arguments& args, Script_Environment& env) const
{
	for(int i = 0; i < args.get_size(); ++i)
	{
		((Variable &) args[i]) = env.get_variable_space().get_variable(this->args_dummy->get_name_at(i)); //wtf1!
	}
	env.get_variable_space().pop_subspace();
}