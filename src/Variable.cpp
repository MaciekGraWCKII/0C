#include "Variable.h"

Variable::Variable(const std::string& type) : type(type)
{

}

std::string Variable::get_type() const
{
	return this->type;
}

bool Variable::operator==(const Variable& var) const
{
	return this->type == var.type;
}

Variable& Variable::operator=(const Variable& var)
{
	this->type = var.type;
}