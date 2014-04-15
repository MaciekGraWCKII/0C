#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
public:
	Variable(std::string type);

	std::string get_type() const;
	virtual bool operator==(const Variable &var)const;

private:
	std::string type;
};

#endif
