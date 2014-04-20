#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
public:
	Variable(const std::string& type);

	std::string get_type() const;
	virtual bool operator==(const Variable& var)const;
	virtual Variable& operator=(const Variable& var);

private:
	std::string type;
};

#endif
