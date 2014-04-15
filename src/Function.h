#ifndef FUNCTION_H
#define FUNCTION_H


#include "Executable_Block_Of_Code.h"
#include "Variable.h"

class Function
{
public:
	class Function_Arguments
	{
	public:
		Function_Arguments(const unsigned int size, Variable** variable_array);

		Variable& operator[](const unsigned int index);
		const Variable& operator[](const unsigned int index) const;
		unsigned int get_size() const;

		~Function_Arguments();
	private:
		Variable** variable_array;
		unsigned int size;
		
		void throw_if_out_of_bounds(const unsigned int index) const;
	};

	class Function_Arguments_Dummy
	{
	public:
		Function_Arguments_Dummy(const unsigned int size, std::string* name_array,
			std::string* type_array);
		Function_Arguments_Dummy(const unsigned int size, std::string* type_array);

		unsigned int get_size() const;
		const std::string& get_name_at(const unsigned int index) const;
		const std::string& get_type_at(const unsigned int index) const;
		bool operator==(const Function_Arguments& args) const;
		bool operator==(const Function_Arguments_Dummy& args_dummy) const;

		~Function_Arguments_Dummy();
	private:
		std::string *name_array;
		std::string *type_array;
		unsigned int size;

		void throw_if_out_of_bounds(const unsigned int index) const;
	};

	Function(Function_Arguments_Dummy* args_dummy, Executable_Block_Of_Code* exec);
	bool are_arguments_valid(const Function_Arguments& args_to_check) const;
	bool are_arguments_valid(const Function_Arguments_Dummy& args_dummy_to_check) const;

	~Function();
private:
	Executable_Block_Of_Code* exec;
	Function_Arguments_Dummy* args_dummy;

};

#endif