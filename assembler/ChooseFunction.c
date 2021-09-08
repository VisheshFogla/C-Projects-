//On myhonor:
////-I have not discussed the C language code in my programwith
//anyone other than my instructor or the teachingassistants
//assigned to thiscourse.
////-I have not used C language code obtained from anotherstudent,
//the Internet, or any other unauthorized source, eithermodified
//orunmodified.
////-If any C language code or documentation used in myprogram
//was obtained from an authorized source, such as a text bookor
//course notes, that has been clearly noted with a propercitation
//in the comments of myprogram.
////-I have not designed this program in such a way as to defeator
//interfere with the normal operation of the gradingcode.
//// Vishesh Fogla
//visheshfogla
#include "ChooseFunction.h"

//setting the look up table for function.
#define TOTAL 13

//look up table for function with a total of 3 instructions.
//the table gives the function name with the binary number associated with it.
static Function functionList[TOTAL] = {
	{"100000", "add"},
	{"100001", "addu"},
	{"100001", "move"},
	{"100010", "sub"},
	{"000010", "mul"},
	{"011000", "mult"},
	{"100100", "and"},
	{"101010", "slt"},
	{"000011", "sra"},
	{"000111", "srav"},
	{"000000", "sll"},
	{"101010", "blt"},
	{"100111", "nor"}
	
};

//the method to retrieve function objects based on the functionName field.
const Function* getFunction(const char* const inputInstruct) {
	
	//a loop that iterates over the look up table for functions 
	for (int i = 0; i < TOTAL; i++) {
		
		//finds the matching function object that has the same 
		// functionName field as the input field.
		if (strcmp(inputInstruct, functionList[i].functionName) == 0) {
			return &functionList[i];
		}
	}
	return NULL;
}

//a method that converts immediate bits from decimal to binary.
char* getImmediate(int imm) {
	
	// a number to check whether the bit is 0 or 1
	int r;
	
	//dynaically allocating an array to store 
	//the chars for immediate bits that are 16 bits.
	char* store = calloc(16, sizeof(char*));
	
	//loop to iterate over the bits to determine 1 or 0
	for (int i = 15; i >= 0; i--) {
		r = imm >> i;
		
		// if 1 then
		if (r & 1) {
			store[15 - i] = '1';
		}
		// if 0 then
		else {
			store[15 - i] = '0';
		}
	}
	return store;
}
