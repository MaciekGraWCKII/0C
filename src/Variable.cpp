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
	this->type = var.get_type();
	return *this;
}

Variable::~Variable()
{

}

Integer::Integer(int nteger) : Variable(TYPE_INTEGER), value(nteger)
{

}

int Integer::get_int()
{
	return value;
}

String::String(const std::string& str) : Variable(TYPE_STRING), str(str)
{

}

std::string& String::get_string()
{
	return this->str;
}