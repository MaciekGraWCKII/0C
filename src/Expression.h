#ifndef EXPRESSION_H
#define EXRPESSION_H

#include <string>

class Variable;
class Script_Environment;

class Expression_Parser
{
public:
	Variable* parse(const std::string& line, Script_Environment& env);

private:
	Variable* parse_identifier(const std::string& line, unsigned int first_char, unsigned int* last_parsed, Script_Environment& env);
};

#define PLUS_PRIORITY 1
#define MINUS_PRIORITY 1
#define OBELUS_PRIORITY 2
#define TIMES_PRIORITY 2

class Operator
{
private:
	unsigned int priority;
	static unsigned int max_priority;

protected:
	void no_match_error(const std::string& who, const std::string& ltype, const std::string& rtype, Variable* l, Variable* r)const;

public:
	Operator(unsigned int priority);
	unsigned int get_priority();
	static unsigned int get_max_priority();

	virtual Variable* execute(Variable* l, Variable* r) = 0;

	//virtual ~Operator();
};

class Operator_Plus : public Operator
{
public:
	Operator_Plus();

	Variable* execute(Variable* l, Variable* r);
};

class Operator_Minus : public Operator
{
public:
	Operator_Minus();

	Variable* execute(Variable* l, Variable* r);
};

class Operator_Times : public Operator
{
public:
	Operator_Times();

	Variable* execute(Variable* l, Variable* r);
};

class Operator_Obelus : public Operator
{
public:
	Operator_Obelus();

	Variable* execute(Variable* l, Variable* r);
};

#endif
