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

bool Integer::operator==(const Variable& var) const
{
	if(var.get_type() == this->get_type())
	{
		return this->operator==((Integer&)var);
	}
	else
	{
		//TODO
		//czy moze jednak wyjatek?
		return false;
	}
}

bool Integer::operator==(const Integer& integer) const
{
	return this->value == integer.value;
}

Variable& Integer::operator=(const Variable& var)
{
	if(var.get_type() == this->get_type())
	{
		return this->operator=((Integer&)var);
	}
	else
	{
		//TODO
		//czy moze jednak wyjatek?
		return *this;
	}
}

Integer& Integer::operator=(const Integer& integer)
{
	this->value = integer.value;
	this->Variable::operator=(integer);
	return *this;
}

Variable* Integer::replicate() const
{
	return new Integer(this->value);
}

int Integer::get_int()
{
	return value;
}

String::String(const std::string& str) : Variable(TYPE_STRING), str(str)
{

}

bool String::operator==(const Variable& var) const
{
	if(var.get_type() == this->get_type())
	{
		return this->operator==((String&)var);
	}
	else
	{
		//TODO
		//czy moze jednak wyjatek?
		return false;
	}
}

bool String::operator==(const String& str) const
{
	return this->str == str.str;
}

Variable& String::operator=(const Variable& var)
{
	if(var.get_type() == this->get_type())
	{
		return this->operator=((String&) var);
	}
	else
	{
		//TODO
		//czy moze jednak wyjatek?
		return *this;
	}
}

String& String::operator=(const String& str)
{
	this->str = str.str;
	return *this;
}

Variable* String::replicate() const
{
	return new String(this->str);
}

std::string& String::get_string()
{
	return this->str;
}