#include "Script_Engine.h"

#include <assert.h>
#include <iostream>

#include <vector>
#include <list>
#include <cctype>
#include <regex>
#include "Expression.h"

Script_Engine::Script_Engine() : comms(new Default_Communicator()), environment(*comms)
{

}

Script_Engine::Script_Engine(Communicator* communicator) : comms(communicator), environment(*communicator)
{

}

void Script_Engine::parse(std::string& line_of_code)
{
	
}

void Script_Engine::parse_test_regex_with_expression(std::string& line_of_code)
{
	std::regex reg("([a-zA-Z]\\w*)\\((.*)\\);");
	std::smatch match;

	if(std::regex_match(line_of_code, match, reg))
	{
		for(unsigned int i = 0; i < match.size(); ++i)
		{
			this->comms->write(std::to_string(i) + ": \"" + match[i].str() + "\"\n");
		}
		if(!this->environment.get_function_space().is_name_here(match[1]))
		{
			this->comms->write("There is no function with the name \"" + match[1].str() + "\n");
			return;
		}
		Expression_Parser expression_parser;
		std::list<Variable*> var_list; 
		unsigned int number_of_args = 0;
		unsigned int first_to_cut = 0;
		std::string args = match[2];
		for(unsigned int i = 0; i < args.size(); ++i)
		{
			if(args[i] == ',')
			{
				var_list.push_back(expression_parser.parse(args.substr(first_to_cut, first_to_cut - i - 1)));
				first_to_cut = i + 1;
				++number_of_args;
			}
		}
		var_list.push_back(expression_parser.parse(args.substr(first_to_cut)));
		if(NULL != var_list.back())
		{
			++number_of_args;
		}
		Variable** var_array = new Variable*[number_of_args];
		std::list<Variable*>::iterator it = var_list.begin();
		for(unsigned int i = 0; i < number_of_args; ++i, ++it)
		{
			var_array[i] = *it;
		}
		Function::Function_Arguments f_args(number_of_args, var_array);
		if(this->environment.get_function_space().is_function_here(match[1], f_args))
		{
			this->environment.get_function_space().call_function(match[1], f_args);
		}
		else
		{
			this->comms->write("There is no \"" + match[1].str() + "\" with these arguments: ");
			for(unsigned int i = 0; i < number_of_args; ++i)
			{
				this->comms->write("\"" + f_args[i].get_type() + "\"");
			}
			this->comms->write("\n");
		}
	}
}

void Script_Engine::parse_test_regex(std::string& line_of_code)
{
	std::regex reg("([a-zA-Z]\\w*)\\((.*)\\);");
	std::smatch match;

	if(std::regex_match(line_of_code, match, reg))
	{
		this->comms->write("Matched: \"" + line_of_code + "\"\n");
		for(unsigned int i = 0; i < match.size(); ++i)
		{
			this->comms->write(std::to_string(i) + ": \"" + match[i].str() + "\"\n");
		}

		if(!this->environment.get_function_space().is_name_here(match[1]))
		{
			this->comms->write("There is no function that goes by the name of \"" + match[1].str() + "\n");
			return;
		}

		if(match[2] == "")
		{
			//no args
			Function::Function_Arguments func_args(0, NULL);
			if(this->environment.get_function_space().is_function_here(
				match[1], func_args))
			{
				this->comms->write("Calling.\n");
				this->environment.get_function_space().call_function(match[1], func_args);
				this->comms->write("#done.\n");
			}
			else
			{
				this->comms->write("No matching function found for these arguments.\n");
			}

			return;
		}

		std::string args = match[2];
		std::list<std::string> arg_list;
		unsigned int first_letter_to_cut = 0;

		for(unsigned int i = 0; i < args.size(); ++i)
		{
			if(args[i] == ',')
			{
				arg_list.push_back(args.substr(first_letter_to_cut, i - 1));
				first_letter_to_cut = i + 1;
			}
		}
		arg_list.push_back(args.substr(first_letter_to_cut));
		
		unsigned int number_of_variables = 0;
		std::list<Variable*> var_list;
		for(std::list<std::string>::iterator it = arg_list.begin(); it != arg_list.end(); ++it)
		{
			this->comms->write("arg " + std::to_string(number_of_variables) + ": \"" + *it + "\"");
			++number_of_variables;
			
			//Remove irrelevant spaces
			bool before = true;
			bool met_quotation_mark = false;
			unsigned int index_of_a_first_character = 0;
			unsigned int number_of_chars = 0;
			for(unsigned int i = 0; i < (*it).size(); ++i)
			{
				if(before)
				{
					if((*it)[i] == ' ')
					{
						++index_of_a_first_character;
					}
					else
					{
						before = false;
						if((*it)[i] == '\"')
						{
							met_quotation_mark = true;
						}
						++number_of_chars;
					}
				}
				else
				{
					if((*it)[i] == '\"')
					{
						break;
					}
					if((*it)[i] == ' ')
					{
						if(!met_quotation_mark)
						{
							break;
						}
						++number_of_chars;
					}
					else
					{
						++number_of_chars;
					}
				}
			}
			*it = (*it).substr(index_of_a_first_character, number_of_chars);

			if((*it)[0] == '"')
			{
				//String
				this->comms->write(" is of a String type.\n");
				var_list.push_back(new String((*it).substr(1)));
			}
			else
			{
				//Integer
				this->comms->write(" is of an Integer type.\n");
				var_list.push_back(new Integer(std::stoi(*it)));
			}
			
		}

		Variable** var_array = new Variable*[number_of_variables];
		unsigned int index = 0;
		for(std::list<Variable*>::iterator it = var_list.begin(); it != var_list.end(); ++it, ++index)
		{
			var_array[index] = *it;
		}

		Function::Function_Arguments func_args(number_of_variables, var_array);
		if(this->environment.get_function_space().is_function_here(match[1], func_args))
		{
			this->comms->write("Calling.\n");
			this->environment.get_function_space().call_function(match[1], func_args);
			this->comms->write("#done.\n");
		}
		else
		{
			this->comms->write("No matching function found for these arguments.\n");
			return;
		}
	}
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

Script_Engine::Default_Communicator::~Default_Communicator()
{

}