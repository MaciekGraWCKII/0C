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

	void parse(std::string& line_of_code);

	void parse_test_regex_with_expression(std::string& line_of_code);
	void parse_test_regex(std::string& line_of_code);
	void parse_preprocessed(std::string& line_of_code);
		//assumes code is prepared for execution after one operation: split
		//Data example: "int a = 5 * 8 ;"
		//will be split into "int", "a", "=", "5", "*", "8", ";" and executed 
	
	bool add_function(const std::string& name, Function* fun);

	~Script_Engine();
private:
	class Default_Communicator : public Communicator
	{
	public:
		void write(const std::string& line) const;

		~Default_Communicator();
	};

	Script_Environment environment;
	Communicator* comms;

};

#endif
