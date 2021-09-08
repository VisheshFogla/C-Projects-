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

#include "Opcode.h"

//setting the look up table for OPCODE.
#define TOTAL 26

//look up table for opcode with a total of 8 instructions.
static Opcode Mneumonic[TOTAL] = {
	
	{"000000", "add"},
	{"000000", "sub"},
	{"001000", "addi"},
	{"000000", "addu"},
	{"000000", "move"},
	{"001001", "addiu"},
	{"000000", "and"},
	{"001100", "andi"},
	{"000000", "nor"},
	{"000000", "slt"},
	{"001010", "slti"},
	{"001111", "lui"},
	{"100011", "lw"},
	{"000100", "beq"},	
	{"000101", "bne"},
	{"000110", "blez"},
	{"000111", "bgtz"},
	{"000010", "j"},
	{"101011", "sw"},
	{"011100", "mul"},
	{"000000", "mult"},
	{"000000", "sra"},
	{"000000", "srav"},
	{"000000", "sll"},
	{"001001", "li"},
	{"001000", "la"}	 		

};

//the method to retrieve opcodes based on the instruction field.
const Opcode* getOpcode(const char* const instruction) {
	
	//a loop that iterates over the look up table for opcode 
	
	for (int i = 0; i < TOTAL; i++) {
		
		//finds the matching opcode object that has the same 
		// insrtruction field as the input field.
		if (strcmp(instruction, Mneumonic[i].ASMinstruct) == 0) {
			return &Mneumonic[i];
		}
	}
	
	//otherwise return null.
	return NULL;
}
