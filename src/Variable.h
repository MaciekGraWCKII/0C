#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

//Default type names defined here
#define TYPE_INTEGER "INTEGER"
#define TYPE_STRING "STRING"
#define TYPE_DOUBLE "DOUBLE"
#define TYPE_BOOL "BOOL"
#define TYPE_NULL "NULL"

class Variable
{
public:
	Variable(const std::string& type);

	std::string get_type() const;
	virtual bool operator==(const Variable& var) const;
	virtual Variable& operator=(const Variable& var);

	virtual ~Variable();
private:
	std::string type;
};

class Integer : public Variable
{
public:
	Integer(const int nteger = 0);

	int get_int();
private:
	int value;
};

class String : public Variable
{
public:
	String(const std::string& str = "");

	std::string& get_string();
private:
	std::string str;
};

#endif
