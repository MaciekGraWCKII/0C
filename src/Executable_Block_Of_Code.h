#ifndef EXECUTABLE_BLOCK_OF_CODE_H
#define EXECUTABLE_BLOCK_OF_CODE_H

class Script_Environment;
class Variable;

class Executable_Block_Of_Code
{
public:
	virtual Variable* execute(Script_Environment& environment) = 0;
	
};

#endif