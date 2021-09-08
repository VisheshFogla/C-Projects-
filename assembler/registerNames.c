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
#include "registerNames.h"

//setting the look up table for registers.
#define TOTAL 26

//look up table for registers with a total of 9 instructions.
//the table gives the resgister name with the binary number associated with it
//as well as the number of the register.
static Register registerList[TOTAL] = {
	{0, "00000", "$zero"},
	{1, "00001", "$at"},
	{4, "00100", "$a0"},
	{5, "00101", "$a1"},
	{8, "01000", "$t0"},
	{9, "01001", "$t1"},
	{10, "01010", "$t2"},
	{11, "01011", "$t3"},
	{12, "01100", "$t4"},
	{13, "01101", "$t5"},
	{14, "01110", "$t6"},
	{15, "01111", "$t7"},
	{16, "10000", "$s0"},
	{17, "10001", "$s1"},
	{18, "10010", "$s2"},
	{19, "10011", "$s3"},
	{20, "10100", "$s4"},
	{21, "10101", "$s5"},
	{22, "10110", "$s6"},
	{23, "10111", "$s7"},
	{24, "11000", "$t8"},
	{25, "11001", "$t9"},
	{26, "11010", "$k0"},
	{27, "11011", "$k1"},
	{2, "00010", "$v0"},
	{3, "00011", "$v1"}
};

//the method to retrieve register objects based on the regNamefield.
const Register* getRegister(const char* const inputReg) {
	
	//a loop that iterates over the look up table for regisster 
	for (int i = 0; i < TOTAL; i++) {
		
		//finds the matching register object that has the same 
		// regName field as the input field.
		if (strcmp(inputReg, registerList[i].regName) == 0) {
			return &registerList[i];
		}
	}
	return NULL;
}
