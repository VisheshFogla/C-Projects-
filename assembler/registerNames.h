#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//creates an object Register 
//that has fields as value, Registers and regName.
struct _Register {
	uint8_t value;
	char* Registers;
	char* regName;
};
typedef struct _Register Register;

//// a method that would retrieve the register object based on the 
// regName field.
const Register* getRegister(const char* const inputReg);
