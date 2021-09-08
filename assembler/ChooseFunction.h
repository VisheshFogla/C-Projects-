#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

//creates an object function 
//that has fields as number and functionName.
struct _Function {
	char* number;
	char* functionName;
};
typedef struct _Function Function;

// a method that would retrieve the function object based on the 
// functionName(Mneumonic) field.
const Function* getFunction(const char* const inputInstruct);

// a method that converts the immediate bits from decimal to binary numbers.
//it returns an array of chars that contain the immediate in the form of binary numbers.
char* getImmediate(int imm);
