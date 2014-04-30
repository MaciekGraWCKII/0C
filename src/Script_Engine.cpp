#include "Script_Engine.h"

#include <iostream>

Script_Engine::Script_Engine()
{
	this->comms = new Default_Communicator();
}

Script_Engine::Script_Engine(Communicator* communicator) : comms(communicator)
{

}

void Script_Engine::parse(std::string& line_of_code) const
{

}

bool Script_Engine::add_function(const std::string& name, Function* fun)
{
	//TODO try and catch this shit
	this->environment.get_function_space().add_function(name, fun);
}

Script_Engine::~Script_Engine()
{
	delete this->comms;
}

void Script_Engine::Default_Communicator::write(const std::string& line) const
{
	std::cout << line;
}
