#include "Expression.h"

#include <vector>
#include <list>
#include <assert.h>

#include "Variable.h"
#include "Exception.h"
#include "Script_Environment.h"

bool is_this_end_of_int(char character)
{
	return(character == ' ' || 
		character == '+' || character == '-' || character == '*' || 
		character == '/');
}

Variable* parse_number(const std::string& line, const unsigned int first_char, unsigned int* last_parsed)
{
	char character;
	bool dot_encountered = false;

	for(unsigned int i = first_char + 1; i < line.size(); ++i)
	{
		character = line[i];
		if(character == '.')
		{
			if(dot_encountered)
			{
				//TODO throw : second dot in double !
			}
			else
			{
				dot_encountered = true;
			}
		}
		if(is_this_end_of_int(character))
		{
			*last_parsed = i - 1;
			if(dot_encountered)
			{
				//parse as a double
				return new Double(std::stod(line.substr(first_char, i - first_char)));
			}
			else
			{
				return new Integer(std::stoi(line.substr(first_char, i - first_char)));
			}
		}
		else if(!isdigit(character))
		{
			//TODO
			//throw sumfin' - we have encountered a character which is not a digit nor a '.'
		}
	}

	*last_parsed = line.size();
	return new Integer(std::stoi(line.substr(first_char)));
}

//first_character of a string, not the '\"'
Variable* parse_string(const std::string& line, unsigned int first_char, unsigned int* last_parsed)
{
	///TODO moze nie wychwycic niepozadanego zakonczenia stringa

	char character;
	unsigned int number_of_slashes = 0;
	std::string line_copy;

	for(unsigned int i = first_char; i < line.size(); ++i)
	{
		character = line[i];

		if(character == '\\')
		{
			++i;
			if(line[i] == '\"')
			{
				line_copy += line[i];
			}
		}
		else if(character == '\"')
		{
			*last_parsed = i;
			return new String(line_copy);
		}
		else
		{
			line_copy += character;
		}
	}

	*last_parsed = line.size();
	return NULL;
}

bool is_this_end_of_identifier(char c)
{
	return (c == ' ' || c == '+' || c == '-' || c == '*' || c == '/');
}

Variable* Expression_Parser::parse_identifier(const std::string& line, unsigned int first_char, unsigned int* last_parsed, Script_Environment& env)
{
	for(unsigned int i = 0; i < line.size(); ++i)
	{
		if(line[i] == '(')
		{
			//Function call
			//cut the identifier of the function to be called
			std::string identifier = line.substr(first_char, i - first_char);
			
			//check if the function identifier exists in the system
			if(!env.get_function_space().is_name_here(identifier))
			{
				throw Exception("Expression::parse: There is no function with such an identifier: \"" + identifier + "\"");
			}

			//look for a closing ')' (but do not forget about '(', because each one requires its own closing ')')
			unsigned int closing_counter = 1;
			unsigned int first_char_of_args = i + 1;

			while(closing_counter != 0)
			{
				++i;
				if(line[i] == ')')
				{
					--closing_counter;
				}
				else if(line[i] == '(')
				{
					++closing_counter;
				}
			}
			//cut args
			std::string args = line.substr(first_char_of_args, i - first_char_of_args);
			*last_parsed = i;

			//parse args
			std::list<Variable*> var_list;
			unsigned int number_of_args = 0;
			unsigned int first_to_cut = 0;

			for(unsigned int i = 0; i < args.size(); ++i)
			{
				if(args[i] == ',')
				{
					var_list.push_back(this->parse(args.substr(first_to_cut, first_to_cut - i - 1), env));
					first_to_cut = i + 1;
					++number_of_args;
				}
			}
			var_list.push_back(this->parse(args.substr(first_to_cut), env));

			//in case of only one arg present or none whatsoever
			if(NULL != var_list.back())
			{
				++number_of_args;
			}
			//array of arguments:
			Variable** var_array = new Variable*[number_of_args];
			std::list<Variable*>::iterator it = var_list.begin();
			//copy arguments to the array
			for(unsigned int i = 0; i < number_of_args; ++i, ++it)
			{
				var_array[i] = *it;
			}
			Function::Function_Arguments f_args(number_of_args, var_array);
			if(env.get_function_space().is_function_here(identifier, f_args))
			{
				//Function can accept those arguments
				return env.get_function_space().call_function(identifier, f_args);
			}
			else
			{
				//Function exists but will not accept those arguments
				std::string arg_str;
				for(unsigned int i = 0; i < number_of_args; ++i)
				{
					arg_str += var_array[i]->get_type() + "\n";
				}
				throw Exception("Expression::parse: function identified as \"" + identifier + "\" can not accept those arguments: \n" + arg_str);
			}
		}
		else if(is_this_end_of_identifier(line[i]))
		{
			//Variable identifier
			*last_parsed = i;
			return &env.get_variable_space().get_variable(line.substr(first_char, i - first_char));
		}
	}

	*last_parsed = line.size();
	return NULL;
}

Variable* parse_parentheses(const std::string& line, unsigned int first_char, unsigned int* last_parsed, Expression_Parser& e_parser, Script_Environment& env)
{
	unsigned int number_of_open_parentheses = 1;

	for(unsigned int i = 0; i < line.size(); ++i)
	{
		if(line[i] == ')')
		{
			--number_of_open_parentheses;
			if(number_of_open_parentheses == 0)
			{
				*last_parsed = i;
				return e_parser.parse(line.substr(first_char + 1, i - 1), env);
			}
		}
	}

	//TODO
	//throw: not all parentheses have been closed
}

Variable* Expression_Parser::parse(const std::string& line, Script_Environment& env)
{
	std::vector<bool> is_var_temp_vector;
	std::vector<Variable*> variable_vector;
	std::vector<Operator*> operator_vector;

	bool operator_allowed = false;
	char character;
	bool space_only = true;
	for(unsigned int i = 0; i < line.size(); ++i)
	{
		character = line[i];

		if(character == ' ')
		{

		}
		else
		{
			space_only = false;
			if(isdigit(character))
			{
				variable_vector.push_back(parse_number(line, i, &i));
				is_var_temp_vector.push_back(true);
				operator_allowed = true;
			}
			else if(character == '\"')
			{
				variable_vector.push_back(parse_string(line, i + 1, &i));
				is_var_temp_vector.push_back(true);
				operator_allowed = true;
			}
			else if(character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z')
			{
				//identifier
				variable_vector.push_back(parse_identifier(line, i, &i, env));
				is_var_temp_vector.push_back(false);
				operator_allowed = true;
			}
			else if(character == '(')
			{
				variable_vector.push_back(parse_parentheses(line, i, &i, *this, env));
				is_var_temp_vector.push_back(true);
				operator_allowed = true;
			}
			else
			{
				if(!operator_allowed)
				{
					//TODO throw

				}

				if(!is_var_temp_vector.back())
				{
					//copy lvar
					Variable* var = variable_vector.back();
					variable_vector.pop_back();
					variable_vector.push_back(var->replicate());
					//not necessary? 
					is_var_temp_vector.pop_back();
					is_var_temp_vector.push_back(true);
				}

				switch(character)
				{
				case '+':
					//add plus_operator
					operator_vector.push_back(new Operator_Plus());
					operator_allowed = false;
					break;

				case '-':
					//add minus_operator
					operator_vector.push_back(new Operator_Minus());
					operator_allowed = false;
					break;

				case '*':
					//add times
					operator_vector.push_back(new Operator_Times());
					operator_allowed = false;
					break;

				case '/':
					//add obelus
					operator_vector.push_back(new Operator_Obelus());
					operator_allowed = false;
					break;
				}
			}
		}
	}

	if(space_only)
	{
		return NULL;
	}

	unsigned int current_operator_priority = Operator::get_max_priority();
	Variable* temp;
	while(!operator_vector.empty())
	{
		unsigned int i = 0;
		while(i < operator_vector.size())
		{
			if(operator_vector[i]->get_priority() == current_operator_priority)
			{
				temp = operator_vector[i]->execute(variable_vector[i], variable_vector[i + 1]);
				variable_vector.erase(variable_vector.begin() + i, variable_vector.begin() + i + 2);
				variable_vector.insert(variable_vector.begin() + i, temp);
				operator_vector.erase(operator_vector.begin() + i);
				i = 0;
			}
			else ++i;
		}
		--current_operator_priority;
	}

	assert(variable_vector.size() == 1);

	return variable_vector[0];
}

unsigned int Operator::max_priority;

Operator::Operator(unsigned int priority) : priority(priority)
{
	if(Operator::max_priority < this->priority)
	{
		Operator::max_priority = this->priority;
	}
}

unsigned int Operator::get_priority()
{
	return this->priority;
}

unsigned int Operator::get_max_priority()
{
	return Operator::max_priority;
}

void Operator::no_match_error(const std::string& who, const std::string& ltype, const std::string& rtype, Variable* l, Variable* r)const
{
	delete l;
	delete r;
	throw Exception(who + " can not find a suitable match for the types: ltype = \"" + ltype + "\" rtype = \"" + rtype);
}

Operator_Plus::Operator_Plus() : Operator(PLUS_PRIORITY)
{

}

Variable* Operator_Plus::execute(Variable* l, Variable* r)
{
	std::string ltype = l->get_type();
	std::string rtype = r->get_type();

	if(ltype == TYPE_BOOL)
	{
		if(rtype == TYPE_BOOL)
		{
			
		}
		else if(rtype == TYPE_INTEGER)
		{
		
		}
		else if(rtype == TYPE_DOUBLE)
		{

		}

		this->no_match_error("Operator_Plus", ltype, rtype, l, r);
	}

	if(ltype == TYPE_STRING)
	{
		if(rtype == TYPE_STRING)
		{
			String* ts = new String(((String*)l)->get_string() + ((String*)r)->get_string());
			delete l;
			delete r;
			return ts;
		}

		this->no_match_error("Operator_Plus", ltype, rtype, l, r);
	}

	if(ltype == TYPE_INTEGER)
	{
		if(rtype == TYPE_INTEGER)
		{
			Integer* ti = new Integer(dynamic_cast<Integer*>(l)->get_int() + dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return ti;
		}

		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Integer*>(l)->get_int() + dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Plus", ltype, rtype, l, r);
	}

	if(ltype == TYPE_DOUBLE)
	{
		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() + dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		if(rtype == TYPE_INTEGER)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() + dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Plus", ltype, rtype, l, r);
	}
	
}

Operator_Minus::Operator_Minus() : Operator(MINUS_PRIORITY)
{

}

Variable* Operator_Minus::execute(Variable* l, Variable* r)
{
	std::string ltype = l->get_type();
	std::string rtype = r->get_type();

	if(ltype == TYPE_INTEGER)
	{
		if(rtype == TYPE_INTEGER)
		{
			Integer* ti = new Integer(dynamic_cast<Integer*>(l)->get_int() - dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return ti;
		}

		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Integer*>(l)->get_int() - dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Minus", ltype, rtype, l, r);
	}

	if(ltype == TYPE_DOUBLE)
	{
		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() - dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		if(rtype == TYPE_INTEGER)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() - dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Minus", ltype, rtype, l, r);
	}

}

Operator_Times::Operator_Times() : Operator(TIMES_PRIORITY)
{

}

Variable* Operator_Times::execute(Variable* l, Variable* r)
{
	std::string ltype = l->get_type();
	std::string rtype = r->get_type();

	if(ltype == TYPE_INTEGER)
	{
		if(rtype == TYPE_INTEGER)
		{
			Integer* ti = new Integer(dynamic_cast<Integer*>(l)->get_int() * dynamic_cast<Integer*>(r)->get_int());
			delete r;
			delete l;
			return ti;
		}

		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Integer*>(l)->get_int() * dynamic_cast<Double*>(r)->get_double());
			delete r;
			delete l;
			return td;
		}

		this->no_match_error("Operator_Times", ltype, rtype, l, r);
	}

	if(ltype == TYPE_DOUBLE)
	{
		if(rtype == TYPE_DOUBLE)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() * dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		if(rtype == TYPE_INTEGER)
		{
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() * dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Times", ltype, rtype, l, r);
	}
}

Operator_Obelus::Operator_Obelus() : Operator(OBELUS_PRIORITY)
{

}

Variable* Operator_Obelus::execute(Variable* l, Variable* r)
{
	std::string ltype = l->get_type();
	std::string rtype = r->get_type();

	if(ltype == TYPE_INTEGER)
	{
		if(rtype == TYPE_INTEGER)
		{
			if(dynamic_cast<Integer*>(r)->get_int() == 0)
			{
				throw Exception("Operator_Obelus: can not divide by zero. l(\"" + ltype + "\")=" 
					+ std::to_string((long long)dynamic_cast<Integer*>(l)->get_int()) + " r(\"" + rtype + "\"=" + 
					std::to_string((long long)dynamic_cast<Integer*>(r)->get_int()));
			}
			Integer* ti = new Integer(dynamic_cast<Integer*>(l)->get_int() / dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return ti;
		}

		if(rtype == TYPE_DOUBLE)
		{
			if(dynamic_cast<Double*>(r)->get_double() == 0)
			{
				throw Exception("Operator_Obelus: can not divide by zero. l(\"" + ltype + "\")=" 
					+ std::to_string((long long)dynamic_cast<Integer*>(l)->get_int()) + " r(\"" + rtype + "\"=" + 
					std::to_string((long double)dynamic_cast<Double*>(r)->get_double()));
			}
			Double* td = new Double(dynamic_cast<Integer*>(l)->get_int() / dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Obelus", ltype, rtype, l, r);
	}

	if(ltype == TYPE_DOUBLE)
	{
		if(rtype == TYPE_DOUBLE)
		{
			if(dynamic_cast<Double*>(r)->get_double() == 0)
			{
				throw Exception("Operator_Obelus: can not divide by zero. l(\"" + ltype + "\")=" 
					+ std::to_string((long double)dynamic_cast<Double*>(l)->get_double()) + " r(\"" + rtype + "\"=" + 
					std::to_string((long double)dynamic_cast<Double*>(r)->get_double()));
			}
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() / dynamic_cast<Double*>(r)->get_double());
			delete l;
			delete r;
			return td;
		}

		if(rtype == TYPE_INTEGER)
		{
			if(dynamic_cast<Integer*>(r)->get_int() == 0)
			{
				throw Exception("Operator_Obelus: can not divide by zero. l(\"" + ltype + "\")=" 
					+ std::to_string((long double)dynamic_cast<Double*>(l)->get_double()) + " r(\"" + rtype + "\"=" + 
					std::to_string((long long)dynamic_cast<Integer*>(r)->get_int()));
			}
			Double* td = new Double(dynamic_cast<Double*>(l)->get_double() / dynamic_cast<Integer*>(r)->get_int());
			delete l;
			delete r;
			return td;
		}

		this->no_match_error("Operator_Obelus", ltype, rtype, l, r);
	}

}