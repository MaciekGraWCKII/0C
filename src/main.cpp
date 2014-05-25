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

	string entry;

	while(nexit)
	{
		getline(cin, entry);
		if(entry == "SAFEWORD")
		{
			break;
		}
		engine.parse_test_regex(entry);
	}

	cout << endl << "system(\"PAUSE\");" << endl;
	system("PAUSE");
	return 0;
}