#ifndef EXECUTABLE_BLOCK_OF_CODE_H
#define EXECUTABLE_BLOCK_OF_CODE_H

#include "Script_Environment.h"

class Executable_Block_Of_Code
{
public:
	Variable* execute(Script_Environment& environment);
	
};

#endif