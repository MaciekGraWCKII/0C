#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception
{
public:
	Exception(std::string description);

	std::string get_description();

	//virtual ~Exception();
private:
	std::string description;
};

#endif
