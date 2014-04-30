#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <string>

#include "Script_Environment.h"
#include "Communicator.h"

class Function;

class Script_Engine
{
public:
	Script_Engine();
	Script_Engine(Communicator* communicator);

	void parse(std::string& line_of_code) const;

	bool add_function(const std::string& name, Function* fun);

	~Script_Engine();
private:
	class Default_Communicator : public Communicator
	{
	public:
		void write(const std::string& line) const;
	};

	Script_Environment environment;
	Communicator* comms;

};

#endif
