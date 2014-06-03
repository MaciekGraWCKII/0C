#include <iostream>

#include "Script_Engine.h"
#include "Executable_Block_Of_Code.h"

using namespace std;

void halt()
{
	cout << "Halt. Hammerzeit!" << endl;
}

class Exec : public Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment)
	{
		halt();
		return NULL;
	}
};

bool nexit = true;

void wydz()
{
	nexit = false;
}

class Exec_wydz : public Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment)
	{
		wydz();
		return NULL;
	}
};

void echo(std::string str)
{
	cout << str << endl;
}

class Exec_echo : public Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment)
	{
		String& var = (String&)environment.get_variable_space().get_variable("ECHO_VARIABLE_0");
		echo(var.get_string());
		return NULL;
	}
};

bool can_i_has_log = true;
class Announcer : public Communicator
{
public:
	void write(const std::string& line)const
	{
		if(can_i_has_log)
		{
			std::cout << line;
		}
	}
};

class Change_logging : public Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment)
	{
		can_i_has_log = ((Integer&)environment.get_variable_space().get_variable("CHANGE_LOGGING_VARIABLE_0")).get_int() != 0;
		return NULL;
	}
};

#define MODE_FIRST_VERSION 1
#define MODE_REGEX_WITHOUT_EXPRESSION 2
#define MODE_REGEX_WITH_EXPRESSION 3
unsigned int mode = MODE_REGEX_WITH_EXPRESSION;

class Change_Parsing_Function : public Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment)
	{
		std::string t = dynamic_cast<String*>(&environment.get_variable_space().get_variable("CHANGE_PARSING_FUNCTION_ARG"))->get_string();

		if(t == "FIRST_VERSION")
		{
			mode = MODE_FIRST_VERSION;
		}
		else if(t == "REGEX_WITHOUT_EXPRESSION")
		{
			mode = MODE_REGEX_WITHOUT_EXPRESSION;
		}else if(t == "REGEX_WITH_EXPRESSION")
		{
			mode = MODE_REGEX_WITH_EXPRESSION;
		}
		else
		{
			environment.get_communicator().write("I do not now this mode: \"" + t + "\"\n");
		}

		return NULL;
	}
};

int main()
{
	Script_Engine engine(new Announcer());
	cout << engine.add_function("halt", new Function(TYPE_NULL, 
		new Function::Function_Arguments_Dummy(0, NULL, NULL),
		new Exec())) << endl;

	cout << engine.add_function("exit", new Function(TYPE_NULL,
		new Function::Function_Arguments_Dummy(0, NULL, NULL),
		new Exec_wydz())) << endl;

	std::string* tab = new std::string[1];
	tab[0] = "ECHO_VARIABLE_0";
	std::string* tab1 = new std::string[1];
	tab1[0] = "STRING";
	cout << engine.add_function("echo", new Function(TYPE_NULL,
		new Function::Function_Arguments_Dummy(1, tab, tab1),
		new Exec_echo())) << endl;

	std::string* tab_log = new std::string[1];
	tab_log[0] = "CHANGE_LOGGING_VARIABLE_0";
	std::string* tab_log_types = new std::string[1];
	tab_log_types[0] = TYPE_INTEGER;
	cout << engine.add_function("change_logging", new Function(TYPE_NULL,
		new Function::Function_Arguments_Dummy(1, tab_log, tab_log_types),
		new Change_logging())) << endl;

	std::string* ch_p_f = new std::string[1];
	ch_p_f[0] = "CHANGE_PARSING_FUNCTION_ARG";
	std::string* ch_p_f_types = new std::string[1];
	ch_p_f_types[0] = TYPE_STRING;
	cout << engine.add_function("change_parsing_function", new Function(TYPE_NULL,
		new Function::Function_Arguments_Dummy(1, ch_p_f, ch_p_f_types),
		new Change_Parsing_Function())) << endl;

	string entry;

	while(nexit)
	{
		getline(cin, entry);
		if(entry == "SAFEWORD")
		{
			break;
		}
		switch(mode)
		{
		case MODE_FIRST_VERSION:
			engine.parse_preprocessed(entry);
			break;
		
		case MODE_REGEX_WITHOUT_EXPRESSION:
			engine.parse_test_regex(entry);
			break;

		case MODE_REGEX_WITH_EXPRESSION:
			engine.parse_test_regex_with_expression(entry);
			break;

		default:
			cout << "DUPESKO!" << endl;
			break;
		}
	}

	cout << endl << "system(\"PAUSE\");" << endl;
	system("PAUSE");
	return 0;
}
