#include "Function.h"

Function::Function_Arguments::Function_Arguments(
	const unsigned int size, Variable** variable_array) : size(size),
	variable_array(variable_array)
{

}

Variable& Function::Function_Arguments::operator[](const unsigned int index)
{
	this->throw_if_out_of_bounds(index);

	return *this->variable_array[index];
}

const Variable& Function::Function_Arguments::operator[](const unsigned int index) const
{
	this->throw_if_out_of_bounds(index);

	return *this->variable_array[index];
}

unsigned int Function::Function_Arguments::get_size() const
{
	return size;
}

Function::Function_Arguments::~Function_Arguments()
{
	for(unsigned int i = 0; i < this->size; ++i)
	{
		delete this->variable_array[i];
	}
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