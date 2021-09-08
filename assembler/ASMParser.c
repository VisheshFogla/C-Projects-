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
#include "ASMParser.h"

/***  Add include directives for here as needed.  ***/



#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "Label.h"
#include "Opcode.h"
#include "ChooseFunction.h"
#include "registerNames.h"
#include "ParseResult.h"

char* DataSegment(const char* const readFile);
char* convertToBin32(int32_t num);
char* convertToBin16(int16_t num);
char* convertToBin8(char num);
char* convertToBin5(int8_t num);

int main(int argc, char** argv){
	
	if (argc == 3) {
	
		FILE* src = fopen(argv[1], "r");
		if ( src == NULL ) {
			printf("Unable to find file %s... exiting.\n", argv[1]);
			exit(1);
		}
	
		FILE* results  = fopen(argv[2], "w");
		if (results == NULL) {
				printf("Output file could not opened.");
				exit(1);
		}
	
		char* dataString = DataSegment(argv[1]);
	
		char* line_instr = (char*)calloc(257, sizeof(char));
		size_t line_size = 256;
		int line_len;
		line_len = getline(&line_instr, &line_size, src);
		int line_num = 1;	
		char size[257];
		char length[100];
	
		while (line_len > 0){
			
				printf(line_instr);	
			
				sprintf(size, "%d", line_len);	
		
				char* comment = strchr(line_instr, '#');
				if (comment != NULL){
					int idx = (int)(comment - line_instr);
					if (idx > 0){
						char* temp = (char*)calloc(idx + 1, sizeof(char));
						strncpy(temp, line_instr, idx);
						free(line_instr);
						line_instr = temp;
						line_len = strlen(line_instr);
					}
					else{
						line_len = 0;
					}
				}
				if (line_len > 0){
					char* temp = (char*)calloc(line_len + 1, sizeof(char));
					sscanf(line_instr, " %[^\n]", temp);
					free(line_instr);
					line_instr = temp;
					line_len = strlen(line_instr);
				}
				if (line_len > 1){
				
					bool header = (strchr(line_instr, '.') != NULL);
					bool label = (strchr(line_instr, ':') != NULL);
					if (!(header || label)){
					
							ParseResult* store = parseASM(line_instr);
					
							if (strcmp(line_instr, "add") == 0 || strcmp(line_instr, "mul") == 0
				|| strcmp(line_instr, "sub") == 0 || strcmp(line_instr, "nor") == 0 || strcmp(line_instr, "and") == 0 || strcmp(line_instr, "slt") == 0 || strcmp(line_instr, "addu") == 0 || strcmp(line_instr, "srav") == 0)	
							{
								fprintf(results, "%s%s%s%s00000%s\n", store->Opcode, store->RS, store->RT, store->RD, store->Funct);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "addi") == 0 || strcmp(line_instr, "andi") == 0 || strcmp(line_instr, "slti") == 0 || strcmp(line_instr, "addiu") == 0)
							{
								fprintf(results, "%s%s%s%s\n", store->Opcode, store->RS, store->RT, store->IMM);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "lui") == 0)
							{
								fprintf(results, "%s00000%s%s\n", store->Opcode, store->RT, store->IMM);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "move") == 0)
							{
								fprintf(results, "%s00000%s%s00000%s\n", store->Opcode, store->RS, store->RD, store->Funct);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "la") == 0)
							{
								fprintf(results, "%s00000%s%s\n", store->Opcode, store->RD, store->label);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "li") == 0)
							{
								fprintf(results, "%s00000%s%s\n", store->Opcode, store->RD, store->IMM);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "sra") == 0 || strcmp(line_instr, "sll") == 0)
							{
								fprintf(results, "%s00000%s%s%s%s\n", store->Opcode, store->RT, store->RD, store->label, store->Funct);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "mult") == 0)
							{
								fprintf(results, "%s%s%s0000000000%s\n", store->Opcode, store->RS, store->RT, store->Funct);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "beq") == 0 || strcmp(line_instr, "bne") == 0)
							{			
								char* binary = convertToBin16(store->rd - line_num);
											
								fprintf(results, "%s%s%s%s\n", store->Opcode, store->RS, store->RT, binary);
								clearResult(store);
								free(store);
								free(binary);
							}
							else if(strcmp(line_instr, "blez") == 0 || strcmp(line_instr, "bgtz") == 0)
							{
								char* binary = convertToBin16(store->rd - line_num);
											
								fprintf(results, "%s%s00000%s\n", store->Opcode, store->RS, binary);
								clearResult(store);
								free(store);
								free(binary);
							}
							else if(strcmp(line_instr, "j") == 0)
							{
								fprintf(results, "%s0000000000%s\n", store->Opcode, store->label);
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "blt") == 0)
							{
								char* binary = convertToBin16(store->rd - line_num - 1);
							
								fprintf(results, "000000%s%s0000100000%s\n", store->RS, store->RT, store->Funct);
								fprintf(results, "0001010000100000%s\n", binary);
								clearResult(store);
								free(store);
								free(binary);
							}
							else if(strcmp(line_instr, "syscall") == 0)
							{
								fprintf(results, "00000000000000000000000000001100\n");
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "nop") == 0)
							{
								fprintf(results, "00000000000000000000000000000000\n");
								clearResult(store);
								free(store);
							}
							else if(strcmp(line_instr, "lw") == 0 || strcmp(line_instr, "sw") == 0)
							{
								if(store->label != NULL)
								{
									fprintf(results, "%s00000%s%s\n", store->Opcode, store->RT, store->label);
									clearResult(store);
									free(store);
								}
								else
								{
									fprintf(results, "%s%s%s%s\n", store->Opcode, store->RS, store->RT, store->IMM);
									clearResult(store);
									free(store);
								}
							}
						
							line_num = line_num + 1;
			
							printf("%d\t", line_num);
						
						}
					
					
					
				}
			
			
				free(line_instr);
				line_instr = (char*)calloc(257, sizeof(char));
				line_len = getline(&line_instr, &line_size, src);
			}
		
			fprintf(results, "\n%s", dataString);
			free(line_instr);
			fclose(src);
			fclose(results);
			ClearTable();
			free(dataString);
		}
		else if (argc == 4)
		{
		char* dataString = DataSegment(argv[1]);
        FILE *results = fopen(argv[2], "w");
		int totalLabel = GetFill();
		for (int i = 0; i < totalLabel; i++){
			Label* currLabel = GetElement(i);
			fprintf(results, "0x0000%04"PRIX16"  %s\n", currLabel->Offset, currLabel->Name);
		}
		free(dataString);
		fclose(results);
		ClearTable();
	}
	
	return 0;
}
	
	


ParseResult* parseASM(const char* const pASM) {

	//a temp pointer to avoid the const parameter
	char* temp = pASM;
	
	//allocating dynamic memory to the ParseResult object
	//the object is named as parser.
	ParseResult* parser = (ParseResult*)calloc(1000, sizeof(ParseResult));


	//allocating dynamic memory to the ASMInstruction field of the parser object
	parser->ASMInstruction = (char*)calloc(1000, sizeof(char*));
	
	//setting the ASMInstruction field to the string pointed by 
	// the parameter pASM
	strcpy(parser->ASMInstruction, pASM);
	
	//defining a pointer for separating the given input string.
	char* space;

	//using strtok to separate the strings to form a string array.
	space = strtok(temp, " ,\t()");
	
	//allocating dynamic memory to the Mneumonic field of the parser object
	parser->Mnemonic = (char*)calloc(1000, sizeof(char*));

	//setting the Mneumonic field to the string pointed by 
	// the first element when separated by space pointer.
	strcpy(parser->Mnemonic, space);
	
	//checking if the instruction is R-type
	if (strcmp(parser->Mnemonic, "add") == 0 || strcmp(parser->Mnemonic, "mul") == 0
			|| strcmp(parser->Mnemonic, "sub") == 0 || strcmp(parser->Mnemonic, "nor") == 0 || strcmp(parser->Mnemonic, "and") == 0 || strcmp(parser->Mnemonic, "slt") == 0 || strcmp(parser->Mnemonic, "addu") == 0){

		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rdName field of the parser object
		parser->rdName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rdName field of the parser object by using strcpy
		strcpy(parser->rdName, space);
		
		//getting the right register for the correct register field.
		parser->rd = getRegister(parser->rdName)->value;
		
		//allocating dynamic memory to the RD field of the parser object
		parser->RD = (char*)calloc(1000, sizeof(char*));
		
		//setting the RD field of the parser object by using strcpy
		strcpy(parser->RD, getRegister(parser->rdName)->Registers);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rsName field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rsName field of the parser object by using strcpy
		strcpy(parser->rsName, space);
		
		//getting the right register for the correct register field.
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);
		
		//getting the right register for the correct register field.
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		parser->Imm = 0;
		parser->IMM = NULL;
	}
	else if (strcmp(parser->Mnemonic, "srav") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rdName field of the parser object
		parser->rdName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rdName field of the parser object by using strcpy
		strcpy(parser->rdName, space);
		
		//getting the right register for the correct register field.
		parser->rd = getRegister(parser->rdName)->value;
		
		//allocating dynamic memory to the RD field of the parser object
		parser->RD = (char*)calloc(1000, sizeof(char*));
		
		//setting the RD field of the parser object by using strcpy
		strcpy(parser->RD, getRegister(parser->rdName)->Registers);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);
		
		//getting the right register for the correct register field.
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rsName field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rsName field of the parser object by using strcpy
		strcpy(parser->rsName, space);
		
		//getting the right register for the correct register field.
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		
		parser->Imm = 0;
		parser->IMM = NULL;
		
	}
	
	//checking if the instruction is I-type(addi)
	else if (strcmp(parser->Mnemonic, "addi") == 0 || strcmp(parser->Mnemonic, "andi") == 0 || strcmp(parser->Mnemonic, "slti") == 0 || strcmp(parser->Mnemonic, "addiu") == 0)
	{
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);
		
		
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rsName field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rsName field of the parser object by using strcpy
		strcpy(parser->rsName, space);
		
		
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);

		parser->Funct = NULL;
		parser->rdName = NULL;
		parser->RD = NULL;
		parser->rd = 255;
		space = strtok(NULL, " ,\t()");
		parser->Imm = atoi(space);
		
		//allocating dynamic memory to the IMM field of the parser object
		parser->IMM = (char*)calloc(16, sizeof(char*));
		
		char* input = getImmediate(atoi(space));
		
		strcpy(parser->IMM, input);
		free(input);
	}	
	//checking if the instruction is I-type(lui)
	else if (strcmp(parser->Mnemonic, "lui") == 0) {
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(10, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(10, sizeof(char*));
		
		space = strtok(NULL, " ,\t()");
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);

		parser->rsName = NULL;
		parser->rdName = NULL;
		parser->RD = NULL;
		parser->Funct = NULL;
		parser->rs = 0;
		parser->rd = 255;
		parser->rt = getRegister(parser->rtName)->value;
		space = strtok(NULL, " ,\t()");
		parser->Imm = atoi(space);
		
		//allocating dynamic memory to the IMM field of the parser object
		parser->IMM = (char*)calloc(16, sizeof(char*));
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(10, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(10, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, "00000");
		
		
		char* input = getImmediate(atoi(space));
		
		strcpy(parser->IMM, input);
		free(input);

	}
	else if (strcmp(parser->Mnemonic, "mult") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(10, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rsName field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rsName field of the parser object by using strcpy
		strcpy(parser->rsName, space);
		
		
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);
		
		
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		parser->rdName = NULL;
		parser->RD = NULL;
		parser->rd = 255;
		parser->Imm = 0;
		parser->IMM = NULL;
		
	}
	else if (strcmp(parser->Mnemonic, "la") == 0 || strcmp(parser->Mnemonic, "li") == 0){
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(10, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rdName field of the parser object
		parser->rdName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rdName field of the parser object by using strcpy
		strcpy(parser->rdName, space);
		
		//getting the right register for the correct register field.
		parser->rd = getRegister(parser->rdName)->value;
		
		//allocating dynamic memory to the RD field of the parser object
		parser->RD = (char*)calloc(1000, sizeof(char*));
		
		//setting the RD field of the parser object by using strcpy
		strcpy(parser->RD, getRegister(parser->rdName)->Registers);
		
		space = strtok(NULL, " ,\t()");

		parser->rsName = NULL;
		parser->rtName = NULL;
		parser->Funct = NULL;
		parser->RS = NULL;
		parser->RT = NULL;
		parser->rs = 0;
		parser->rt = 0;
		
		if(strcmp(parser->Mnemonic, "la") == 0)
		{
			parser->label = convertToBin16(GetLabel(space));
		}
		else if(strcmp(parser->Mnemonic, "li") == 0)
		{
			parser->Imm = atoi(space);
		
			//allocating dynamic memory to the IMM field of the parser object
			parser->IMM = (char*)calloc(16, sizeof(char*));
		
			char* input = getImmediate(atoi(space));
		
			strcpy(parser->IMM, input);
			free(input);
		}
	}
	else if (strcmp(parser->Mnemonic, "move") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		
		//allocating dynamic memory to the rdName field of the parser object
		parser->rdName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rdName field of the parser object by using strcpy
		strcpy(parser->rdName, space);
		
		//getting the right register for the correct register field.
		parser->rd = getRegister(parser->rdName)->value;
		
		//allocating dynamic memory to the RD field of the parser object
		parser->RD = (char*)calloc(1000, sizeof(char*));
		
		//setting the RD field of the parser object by using strcpy
		strcpy(parser->RD, getRegister(parser->rdName)->Registers);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rsName field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rsName field of the parser object by using strcpy
		strcpy(parser->rsName, space);
		
		
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		
		//setting the RS field of the parser object by using strcpy
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		parser->rtName = NULL;
		parser->RT = NULL;
		parser->rt = 255;
	}
	else if (strcmp(parser->Mnemonic, "sra") == 0 || strcmp(parser->Mnemonic, "sll") == 0){
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rdName field of the parser object
		parser->rdName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rdName field of the parser object by using strcpy
		strcpy(parser->rdName, space);
		
		//getting the right register for the correct register field.
		parser->rd = getRegister(parser->rdName)->value;
		
		//allocating dynamic memory to the RD field of the parser object
		parser->RD = (char*)calloc(1000, sizeof(char*));
		
		//setting the RD field of the parser object by using strcpy
		strcpy(parser->RD, getRegister(parser->rdName)->Registers);
		
		
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		
		//setting the rtName field of the parser object by using strcpy
		strcpy(parser->rtName, space);
		
		//getting the right register for the correct register field.
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		
		//setting the RT field of the parser object by using strcpy
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		parser->rsName = NULL;
		parser->RS = NULL;
		parser->rs = 0;
		
		space = strtok(NULL, " ,\t()");
		
		
		
		parser->label = convertToBin5(atoi(space));
		
	}
	
	//checking if the instruction is I-type(beq)
	else if (strcmp(parser->Mnemonic, "beq") == 0 || strcmp(parser->Mnemonic, "bne") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->rsName, space);
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->rtName, space);
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);

		parser->Funct = NULL;
		parser->rdName = NULL;
		parser->RD = NULL;
		space = strtok(NULL, " ,\t()");
		
		printf("%s\n", convertToBin16(GetLabel(space)/4));
		
		parser->rd = GetLabel(space)/4;
	}
	else if (strcmp(parser->Mnemonic, "blez") == 0 || strcmp(parser->Mnemonic, "bgtz") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->rsName, space);
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		space = strtok(NULL, " ,\t()");
		
		parser->Funct = NULL;
		parser->rdName = NULL;
		parser->RD = NULL;
		parser->rtName = NULL;
		parser->rt = NULL;
		parser->rd = GetLabel(space)/4;
	}
	else if (strcmp(parser->Mnemonic, "blt") == 0)
	{
		//allocating dynamic memory to the funct field of the parser object
		parser->Funct = (char*)calloc(1000, sizeof(char*));
		
		//setting the funct field of the parser object by using strcpy
		strcpy(parser->Funct, getFunction(parser->Mnemonic)->number);
		
		space = strtok(NULL, " ,\t()");
		
		parser->rsName = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->rsName, space);
		parser->rs = getRegister(parser->rsName)->value;
		
		//allocating dynamic memory to the RS field of the parser object
		parser->RS = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->RS, getRegister(parser->rsName)->Registers);
		space = strtok(NULL, " ,\t()");
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->rtName, space);
		parser->rt = getRegister(parser->rtName)->value;
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(1000, sizeof(char*));
		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		space = strtok(NULL, " ,\t()");
	
		parser->rdName = NULL;
		parser->RD = NULL;
		
		parser->rd = GetLabel(space)/4;
		
		
	}
	else if (strcmp(parser->Mnemonic, "j") == 0)
	{
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(1000, sizeof(char*));
		
		//setting the Opcode field of the parser object by using strcpy
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);
		space = strtok(NULL, " ,\t()");
		
		parser->label = convertToBin16(GetLabel(space)/4);
	}
	
	//checking if the instruction is lw or sw
	else if (strcmp(parser->Mnemonic, "lw") == 0 || strcmp(parser->Mnemonic, "sw") == 0) {
		
		//allocating dynamic memory to the Opcode field of the parser object
		parser->Opcode = (char*)calloc(10, sizeof(char*));
		
		strcpy(parser->Opcode, getOpcode(parser->Mnemonic)->number);	
		
		space = strtok(NULL, " ,\t()");
		
		
		//allocating dynamic memory to the rtName field of the parser object
		parser->rtName = (char*)calloc(10, sizeof(char*));
		
		strcpy(parser->rtName, space);
		
		//allocating dynamic memory to the RT field of the parser object
		parser->RT = (char*)calloc(10, sizeof(char*));

		strcpy(parser->RT, getRegister(parser->rtName)->Registers);
		
		space = strtok(NULL, " ,\t()");
		
		if(GetLabel(space) != -1)
		{
			parser->label = convertToBin16(GetLabel(space));
		}
		else
		{
			//allocating dynamic memory to the IMM field of the parser object
			parser->IMM = (char*)calloc(16, sizeof(char*));
			parser->Imm = atoi(space);
			char* input = getImmediate(atoi(space));
		
			strcpy(parser->IMM, input);
			free(input);
			
			space = strtok(NULL, " ,\t()");
			
			//allocating dynamic memory to the rsName field of the parser object
			parser->rsName = (char*)calloc(10, sizeof(char*));
			strcpy(parser->rsName, space);
		
			//allocating dynamic memory to the RS field of the parser object
			parser->RS = (char*)calloc(10, sizeof(char*));
		
			strcpy(parser->RS, getRegister(parser->rsName)->Registers);
			
			parser->rs = getRegister(parser->rsName)->value;
			
			parser->label = NULL;
			
		}	
		
		
		parser->rd = 255;
		
		parser->rt = getRegister(parser->rtName)->value;
		parser->Funct = NULL;
		parser->RD = NULL;
		parser->rdName = NULL;

	}
	
	
	return parser;
}


char* convertToBin16(int16_t num){
	char* result = (char*)calloc(17, sizeof(char));
	for (int i = 15; i >= 0; i--){
		if (num & 1) {
                	result[i] = '1';
                }
                else {
                        result[i] = '0';
                }
                num = num >> 1;
	}
	return result;
}



char* convertToBin32(int32_t num){
        char* result = (char*)calloc(33, sizeof(char));
        for (int i = 31; i >= 0; i--){
                if (num & 1) {
                        result[i] = '1';
                }
                else {
                        result[i] = '0';
                }
                num = num >> 1;
        }
        return result;
}

char* convertToBin5(int8_t num){
        char* result = (char*)calloc(6, sizeof(char));
        for (int i = 4; i >= 0; i--){
                if (num & 1) {
                        result[i] = '1';
                }
                else {
                        result[i] = '0';
                }
                num = num >> 1;
        }
        return result;
}

char* convertToBin8(char num){
        char* result = (char*)calloc(9, sizeof(char));
        for (int i = 7; i >= 0; i--){
                if (num & 1) {
                        result[i] = '1';
                }
                else {
                        result[i] = '0';
                }
                num = num >> 1;
        }
        return result;
}


char* DataSegment(const char* const readFile){
	FILE *In = fopen(readFile, "r");
        if (In == NULL){
        	printf("ERROR: Could not find input file.");
                return NULL;
        }
	char* dataString = (char*)calloc(1, sizeof(char));
	bool isData = false;
	bool isText = false;
	int16_t lineOffset = 0;
	char* line_instr = (char*)calloc(257, sizeof(char));
    size_t line_size = 256;
    int line_len;
    line_len = getline(&line_instr, &line_size, In);
    
    char size[257];
	while (line_len > 0){
		
				printf(line_instr);	
			
				sprintf(size, "%d", line_len);	
		
				char* comment = strchr(line_instr, '#');
                if (comment != NULL){
                	int idx = (int)(comment - line_instr);
					if (idx > 0){
							char* temp = (char*)calloc(idx + 1, sizeof(char));
                        	strncpy(temp, line_instr, idx);
                        	free(line_instr);
                        	line_instr = temp;
                        	line_len = strlen(line_instr);
					}
					else{
						line_len = 0;
					}
                }
				if (line_len > 0){
					char* temp = (char*)calloc(line_len + 1, sizeof(char));
                	sscanf(line_instr, " %[^\n]", temp);
                	free(line_instr);	
                	line_instr = temp;
                	line_len = strlen(line_instr);
				}
				if (line_len > 1){
			
					bool header = (strchr(line_instr, '.') != NULL);
					bool label = (strchr(line_instr, ':') != NULL);
					if (header && !label){
						char* section = (char*)calloc(5, sizeof(char));
						sscanf(line_instr, " .%s", section);
						if (strncmp("data", section, 4) == 0){
							isData = true;
							isText = false;
						}
						else if (strncmp("text", section, 4) == 0){
							int numPadding = 4 - (lineOffset % 4);
							if (numPadding != 4){
								for (int i = 0; i < numPadding; i++){
									dataString = (char*)strncat(dataString, "00000000", 8);
									lineOffset++;
								}
								dataString = (char*)strncat(dataString, "\n", 1);
							}
							isText = true;
							isData = false;
							lineOffset = 0;
						}
						free(section);
					}
					else if (isData){
						if (label){
							char* name = (char*)calloc(33, sizeof(char));
							char* type = (char*)calloc(7, sizeof(char));
							char* data = (char*)calloc(257, sizeof(char));
							sscanf(line_instr, " %[^:]: .%s %[^\n]", name, type, data);
							
							if (strlen(type) == 4 && strncmp("word", type, 4) == 0){
								int numPadding = 4 - (lineOffset % 4);
								if (numPadding != 4){
									for (int i = 0; i < numPadding; i++){
										dataString = (char*)strncat(dataString, "00000000", 8);
										lineOffset++;
									}
									dataString = (char*)strncat(dataString, "\n", 1);
								}
								
								bool range = (strchr(data, ':') != NULL);
								bool array = (strchr(data, ',') != NULL);
								if (range){
									int count;
									int32_t init;
									sscanf(data, "%"PRIi32":%i", &init, &count);
									InsertLabel(name, lineOffset + 0x2000);
									char* initStr = convertToBin32(init);
									for (int i = 0; i < count; i++){
										size_t len = strlen(dataString);
										dataString = (char*)realloc(dataString, len + 34);
										dataString = (char*)strncat(dataString, initStr, 32);
										dataString = (char*)strncat(dataString, "\n", 1);
										lineOffset = lineOffset + 4;
									}
									free(initStr);
								}
								else if (array){
									InsertLabel(name, lineOffset + 0x2000);
									while (array){
									
										int32_t init;
										char* temp = (char*)calloc(257, sizeof(char));
										sscanf(data, "%"PRIi32", %[^\n]", &init, temp);
										free(data);
										data = temp;
										array = (strchr(data, ',') != NULL);
										lineOffset = lineOffset + 4;
										size_t len = strlen(dataString);
										char* initStr = convertToBin32(init);
										dataString = (char*)realloc(dataString, len + 34);
										dataString = (char*)strncat(dataString, initStr, 32);
										dataString = (char*)strncat(dataString, "\n", 1);
										free(initStr);
									}	
									
									int32_t init;
									sscanf(data, "%"PRIi32"", &init);
									char* initStr = convertToBin32(init);
									size_t len = strlen(dataString);
									dataString = (char*)realloc(dataString, len + 34);
									dataString = (char*)strncat(dataString, initStr, 32);
									dataString = (char*)strncat(dataString, "\n", 1);
									lineOffset = lineOffset + 4;
									free(initStr);
								}
								else{
									
									int32_t init;
									sscanf(data, "%"PRIi32"", &init);
									char* initStr = convertToBin32(init);
									InsertLabel(name, lineOffset + 0x2000);
									size_t len = strlen(dataString);
									dataString = (char*)realloc(dataString, len + 34);
									dataString = (char*)strncat(dataString, initStr, 32);
									dataString = (char*)strncat(dataString, "\n", 1);
									lineOffset = lineOffset + 4;
									free(initStr);
								}
							}	
							else if (strlen(type) == 6 && strncmp("asciiz", type, 6) == 0){
								
								char* string = (char*)calloc(257, sizeof(char));
								sscanf(data, "\"%[^\"]\"", string);
								InsertLabel(name, lineOffset + 0x2000);
								size_t len = strlen(string);
								for (int i = 0; i < len; i++){
									if ((lineOffset % 4) == 0){
										dataString = (char*)realloc(dataString, strlen(dataString) + 34);
									}
									char letter = string[i];
									char* binary = convertToBin8(letter);
									dataString = (char*)strncat(dataString, binary, 8);
									free(binary);
									lineOffset++;
									if ((lineOffset % 4) == 0){
										dataString = (char*)strncat(dataString, "\n", 1);
									}
								}
								if ((lineOffset % 4) == 0){
									dataString = (char*)realloc(dataString, strlen(dataString) + 34);
								}
								dataString = (char*)strncat(dataString, "00000000", 8);
								lineOffset++;
                                if ((lineOffset % 4) == 0){
									dataString = (char*)strncat(dataString, "\n", 1);
                                }
                                
								free(string);	
							}
							free(type);
							free(data);
						}
					}
					else if (isText){
						if (label){
							char* name = (char*)calloc(33, sizeof(char));
							sscanf(line_instr, " %[^:]", name);
							InsertLabel(name, lineOffset * 4);
							
						}
						else{
							char* name = (char*)calloc(8, sizeof(char));
							sscanf(line_instr, "%s", name);
							if (strlen(name) == 3 && strncmp(name, "blt", 3) == 0){
								lineOffset++;
							}
							lineOffset++;
							free(name);
						}
					}
				}
				free(line_instr);
				line_instr = (char*)calloc(257, sizeof(char));
				line_len = getline(&line_instr, &line_size, In);
			}
			free(line_instr);
			fclose(In);
			return dataString;
}
