#include "Script_Engine.h"

#include <assert.h>
#include <iostream>

#include <vector>
#include <list>
#include <cctype>

Script_Engine::Script_Engine()
{
	this->comms = new Default_Communicator();
}

Script_Engine::Script_Engine(Communicator* communicator) : comms(communicator)
{

}

void Script_Engine::parse(std::string& line_of_code)
{
	
}

void Script_Engine::parse_preprocessed(std::string& line_of_code)
{
	//Extremely dummy
	std::vector<std::string> words;

	size_t current_index = 0;
	while(1)
	{
		if(current_index == line_of_code.size())
		{
			break;
		}
		if(line_of_code[current_index] == ' ')
		{
			words.push_back(line_of_code.substr(0, current_index));
			line_of_code.erase(0, current_index + 1);
			current_index = 0;
		}
		else
		{
			++current_index;
		}
	}
	//the last word
	words.push_back(line_of_code);

	if(this->environment.get_function_space().is_name_here(words[0]))
	{
		if(words[1] != "(")
		{
			assert(0);
		}
		//TODO
		//Function call
		//get the args

		if(words.size() == 4)
		{
			if(words[2] == ")")
			{
				//run a function with no arguments
				this->environment.get_function_space().call_function(
					words[0], Function::Function_Arguments(0, NULL));
			}
			else
			{
				assert(0);
			}
		}

		unsigned int arg_count = (words.size() - 4) / 2 + 1;
		Variable** variable_array = new Variable*[arg_count];
		unsigned int index = 0;

		for(size_t i = 2; i < words.size() - 1; i += 2, ++index)
		{
			if(isdigit(words[i].at(0)))
			{
				 variable_array[index] = new Integer(std::stoi(words[i]));
			}
			else
			{
				variable_array[index] = new String(words[i]);
			}
		}

		this->environment.get_function_space().call_function(
			words[0], Function::Function_Arguments(arg_count, variable_array));
	}
}

bool Script_Engine::add_function(const std::string& name, Function* fun)
{
	//TODO try and catch this shit
	this->environment.get_function_space().add_function(name, fun);
	return true;
}

Script_Engine::~Script_Engine()
{
	delete this->comms;
}

void Script_Engine::Default_Communicator::write(const std::string& line) const
{
	std::cout << line;
}
