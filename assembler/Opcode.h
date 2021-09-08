#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

//creates an object opcode 
//that has fields as number and instruction.
struct _Opcode {
	char* number;
	char* ASMinstruct;
};
typedef struct _Opcode Opcode;

// a method that would retrieve the opcode object based on the 
// instruction field.
const Opcode* getOpcode(const char* const instruction);

