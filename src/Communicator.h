#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <string>

class Communicator
{
public:
	virtual void write(const std::string& line) const = 0;

	virtual ~Communicator(){}
};

#endif