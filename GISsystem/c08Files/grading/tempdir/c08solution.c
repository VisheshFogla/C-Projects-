#include "c08solution.h"
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/types.h>
#include <math.h>
#include "StringHashTable.h"
#define PI 3.14159265358979323846
#define RADIUS 6371.0088

//  On my honor:
////  -I have not discussed the C language code in my program with
//    anyone other than my instructor or the teaching assistants 
//    assigned to this course.
////  -I have not used C language code obtained from another student, 
//    the Internet, or any other unauthorized source, either modified
//    or unmodified.  
////  -If any C language code or documentation used in my program 
//    was obtained from an authorized source, such as a text book or
//    course notes, that has been clearly noted with a proper citation
//    in the comments of my program.
////  -I have not designed this program in such a way as to defeat or
//    interfere with the normal operation of the Curator System.
////    VISHESH FOGLA
//    visheshfogla

int main(int argc, char **argv) {

	// Reads the command line argument and inputs the filename

	FILE *Input = fopen(argv[1], "r");

	//Checks if the argument is NULL and returns an error message.
	if (Input == NULL){
		printf("Could not find file.");
	}

	//Reading the name of the input argument by using a while loop
	while ((char)fgetc(Input) == ';'){
		read_argument(Input, '\n');
	}

	//File pointer to the Output file that would store the result required.

	//Since we need to write to this file we use 'w'
	FILE *Output = fopen(argv[2], "w");

	//Creating an char array of size 100 to store the information in the file.
	//We will then parse the data and process it.
	char* textfile = malloc(50 * sizeof(char));

	fscanf(Input,"%*s%s",textfile);

	//Defining a pointer for the file to be read.
	FILE *Info = fopen(textfile, "r");

	//Defining a variable for size of the table
	uint32_t table_sz;
	fscanf(Input, "%*s%"PRIu32"\n", &table_sz);

	//Defining an array for feature index
	FIDObj *fid_arr = malloc(256 * sizeof(FIDObj));

	//Also defining the length of array to store information from the file
	//Do this while implementing the FID index.
	uint32_t arr_sz = 256;


	//Pointer for the StringHashTable that we are creating 
	//The parameters include the size of the table and elfhash
	StringHashTable* table = StringHashTable_create(table_sz, elfhash);

	//completes the number of coloumns that are separated by pipe '|'
	//It takes parameter Info, Table, size of array and 
	uint32_t parts = complete_array(Info, &fid_arr, table, arr_sz);

	//Using qsort to sort the entries based on Feature ID index.
	qsort(fid_arr, parts, sizeof(FIDObj), compare_Ids);

	read_data(Input, Output, Info, fid_arr, table, parts);

	

	free(textfile);

	//clears the table entirely	
	StringHashTable_clear(table);

	//Next, freeing the table and closing the pointers to the files.
	free(table);

	free(fid_arr);

	fclose(Info);
	fclose(Input);
	fclose(Output);

	return 0;
}

/**Function for creating an FID Object. The function returns a pointer that points to the created object.
*It sets the id and fid_offset for the newly created object.
*/
FIDObj* create_obj(int Id, uint32_t location){

	//Creating a new pointer that points to a FID object
        FIDObj* new = (FIDObj*)malloc(sizeof(FIDObj));
 
	//setting the id for the new object being created
        new->id = Id;

	//setting the fid_offset for the new FID object being created
        new->fid_offset = location;

        return new;
}

/**Reads the input argument untill the last character in the name of the file.
 * Implements a while loop to achieve the desired function.
 * Takes two parameters: one is the input file and the other is the last character that 
 * it should read till. 
*/  
uint32_t read_argument(FILE* input, char last_char){

	uint32_t counter_read = 1;

	char single = (char)fgetc(input);

	while ((single != last_char) && (single != EOF))
	{
		single = (char)fgetc(input);
		counter_read++;
	}

	return counter_read;
}


/** Function that has been provided for hashing.
 * The function aids in converting an integer from a string. 
 * Mystery Function
 */
uint32_t elfhash(const char* str) {
	assert(str != NULL ); // self-destuct if called with NULL
	uint32_t hashvalue = 0, // value to be returned
		 high; // high nybble of current hashvalue
	while ( *str ) { // continue until *str is '\0'
		hashvalue = (hashvalue << 4) + *str++; // shift high nybble out,
						       // add in next char,
						       // skip to the next char
		if ( high = (hashvalue & 0xF0000000) ) { // if high nybble != 0000
		hashvalue = hashvalue ^ (high >> 24); // fold it back in
		}
		hashvalue = hashvalue & 0x7FFFFFFF; // zero high nybble
	}
	return hashvalue;
}

/** The function is defined to complete the array indexes of the table. 
 * It takes several parameters such as file, the pointer to the array, StringHashTable and array_size.
 * The function also uses realloc for doubling the capacity of array, incase the array become full 
 * or should the limit of the array exceed. It also calls addEntry from the StringHashTable.h to keep adding entry
 * at specific locations in the table. 
 */
uint32_t complete_array(FILE* file, FIDObj** array, StringHashTable* table, uint32_t array_size){
	
	//Defining an offset which would take input from the file that is being read
	uint32_t offset = read_argument(file, '\n');

	//setting pointer to the row attribute to NULL.
	char* line_buf = NULL;

	//setting the size of attributes to 0.
	size_t buf_size = 0;

	
	//initializing a variable for length of row.
	ssize_t line_len;
	uint32_t numEle = 0;

	//using the getline function for extracting attributes from input file.
	line_len = getline(&line_buf, &buf_size, file);

	
	while (line_len >= 0){

		//feature id, state and name
		int id;
		char* name = (char*)malloc(124*sizeof(char));
       		char* state = (char*)malloc(3*sizeof(char));

		//taking out the fid, f_name and f_state from the row
		sscanf(line_buf, "%i|%[^|]|%*[^|]|%[^|]|", &id, name, state);

		
		strcat(name, state);

		//creating a FID Object to store id and offset
		FIDObj* fid = create_obj(id, offset);
		if (numEle == array_size){
			*array = (FIDObj*)realloc(*array, array_size * sizeof(FIDObj) * 2);
			array_size = array_size * 2;
		}

		
		(*array)[numEle] = *fid;

		//adding an entry to the table
		StringHashTable_addEntry(table, name, offset);
		
		//freeing the pointers to f_name and f_state
		free(name);
		free(state);

		//increment the counter 
		numEle++;
		offset = offset + line_len;

		line_len = getline(&line_buf, &buf_size, file);

		//lastly, freeing the f_id to let the loop run again and reset values.
		free(fid);
	}

	//freeing the row_attr so everything is cleared up.
	free(line_buf);
	return numEle;
}

/** The function compares the ids of two FID objects to sort the array. 
 * This function takes parameters which are pointers pointing to the 
 * ids of two different FID Objects and then returning the one which is 
 * lesser of the two. 
 */
int compare_Ids(const void *left, const void *right){

	//Creating a local pointer 
	const FIDObj* pLeft = (FIDObj*) left;
	const FIDObj* pRight = (FIDObj*) right;

	// Comparing the ids to be arranged in the ascending order.
	return (((pLeft)->id) - ((pRight)->id));
}

void read_data(FILE* in, FILE* out, FILE* data, FIDObj* array, StringHashTable* table, uint32_t numEle){
	char* row_attr = NULL;
	size_t attr_sz = 0;
	ssize_t row_length = getline(&row_attr, &attr_sz, in);
	int cmdNum = 1;
	while (row_length > 0){
		if (row_attr[0] == ';'){
			fprintf(out, row_attr);
		}
		else{
			fprintf(out, "Cmd\t%i:\t%s\n", cmdNum, row_attr);
			cmdNum++;
			char* cmd = (char*)malloc(17*sizeof(char));
			sscanf(row_attr, "%s", cmd);
			if (strcmp(cmd, "exists") == 0){
				char* name = (char*)malloc(124*sizeof(char));
				char* nameCpy = (char*)malloc(124*sizeof(char));
               			char* state = (char*)malloc(3*sizeof(char));
				sscanf(row_attr, "%*s\t%[^\t]%s", name, state);
				strcpy(nameCpy, name);
				strcat(name, state);
				uint32_t* offsets = StringHashTable_getLocationsOf(table, name);
				int numKeys = calculate_len(offsets);
				fprintf(out, "%i occurrences: %s %s\n", numKeys, nameCpy, state);
				free(nameCpy);
				free(offsets);
				free(name);
				free(state);
			}
			else if (strcmp(cmd, "details_of") == 0){
				char* name = (char*)malloc(124*sizeof(char));
                		char* state = (char*)malloc(3*sizeof(char));
				sscanf(row_attr, "%*s\t%[^\t]%s", name, state);
				strcat(name, state);
				uint32_t* offsets = StringHashTable_getLocationsOf(table, name);
				if (offsets != NULL){
					int numOffsets = calculate_len(offsets);	
					qsort(offsets, numOffsets, sizeof(uint32_t), compare_fid_offsets);
					for (int i = 0; i < numOffsets; i++){
						uint32_t offset = offsets[i];
						int id;
						char* name = (char*)malloc(121*sizeof(char));
						char* class = (char*)malloc(51*sizeof(char));
						char* state = (char*)malloc(3*sizeof(char));
						char* county = (char*)malloc(101*sizeof(char));
						char* latDMS = (char*)malloc(8*sizeof(char));
						char* longDMS = (char*)malloc(9*sizeof(char));
						float latDEC;
						float longDEC;
						fseek(data, offset, SEEK_SET);
						char* row_attr2 = NULL;
						size_t attr_sz2 = 0;
						getline(&row_attr2, &attr_sz2, data);
						sscanf(row_attr2, "%i|%[^|]|%[^|]|%[^|]|%*[^|]|%[^|]|%*[^|]|%[^|]|%[^|]|%f|%f|", &id, name, class, state, county, latDMS, longDMS, &latDEC, &longDEC);
						fprintf(out, "FID:       %i\n", id);
						fprintf(out, "Name:      %s\n", name);
						fprintf(out, "Type:      %s\n", class);
						fprintf(out, "State:     %s\n", state);
						fprintf(out, "County:    %s\n", county);
						latDMS = defining_direct(latDMS, 0);
						longDMS = defining_direct(longDMS, 1);
						fprintf(out, "Longitude: %s   (%f)\n", longDMS, longDEC);
						fprintf(out, "Latitude:   %s   (%f)\n\n", latDMS, latDEC);
						free(row_attr2);
						free(latDMS);
						free(longDMS);
						free(county);
						free(class);
						free(name);
						free(state);
					}
				}
				else{
					fprintf(out, "No records found for: %s\n", name);
				}
				free(name);
				free(state);
				free(offsets);
			}
			else if (strcmp(cmd, "distance_between") == 0){
				int id1;
				int id2;
				sscanf(row_attr, "%*s\t%i\t%i", &id1, &id2);
				FIDObj* obj1 = create_obj(id1, 0);
				FIDObj* obj2 = create_obj(id2, 0);
				FIDObj* result1 = bsearch(obj1, array, numEle, sizeof(FIDObj), compare_Ids);
				FIDObj* result2 = bsearch(obj2, array, numEle, sizeof(FIDObj), compare_Ids);
				if (result1 != NULL && result2 != NULL){
					uint32_t offset1 = result1->fid_offset;
					uint32_t offset2 = result2->fid_offset;
					char* name1 = (char*)malloc(121*sizeof(char));
                                	char* state1 = (char*)malloc(3*sizeof(char));
                                	char* latDMS1 = (char*)malloc(8*sizeof(char));
                                	char* longDMS1 = (char*)malloc(9*sizeof(char));
                                	float latDEC1;
                                	float longDEC1;
					char* name2 = (char*)malloc(121*sizeof(char));
                                	char* state2 = (char*)malloc(3*sizeof(char));
                                	char* latDMS2 = (char*)malloc(8*sizeof(char));
                                	char* longDMS2 = (char*)malloc(9*sizeof(char));
                                	float latDEC2;
                                	float longDEC2;
					fseek(data, offset1, SEEK_SET);
					char* row_attr2 = NULL;
                                	size_t attr_sz2 = 0;
                                	getline(&row_attr2, &attr_sz2, data);
					sscanf(row_attr2, "%*i|%[^|]|%*[^|]|%[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%[^|]|%f|%f|", name1, state1, latDMS1, longDMS1, &latDEC1, &longDEC1);
					fseek(data, offset2, SEEK_SET);
					getline(&row_attr2, &attr_sz2, data);
					sscanf(row_attr2, "%*i|%[^|]|%*[^|]|%[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%[^|]|%f|%f|", name2, state2, latDMS2, longDMS2, &latDEC2, &longDEC2);
					latDMS1 = defining_direct(latDMS1, 0);
					longDMS1 = defining_direct(longDMS1, 1);
					latDMS2 = defining_direct(latDMS2, 0);
                                	longDMS2 = defining_direct(longDMS2, 1);
					latDEC1 = latDEC1 * PI / 180;
					longDEC1 = longDEC1 * PI / 180;
					latDEC2 = latDEC2 * PI / 180;
					longDEC2 = longDEC2 * PI / 180;
					float cent_ang = acos(sin(latDEC1)*sin(latDEC2) + cos(latDEC1)*cos(latDEC2)*cos(fabs(longDEC1 - longDEC2)));
					float length = round(RADIUS * cent_ang * 10) / 10;
					fprintf(out, "First:     ( %s,  %s )  %s, %s\n", longDMS1, latDMS1, name1, state1);
					fprintf(out, "Second:    ( %s,  %s )  %s, %s\n", longDMS2, latDMS2, name2, state2);
					fprintf(out, "Distance:  %.1fkm\n", length);
					free(row_attr2);
					free(name1);
					free(state1);
					free(latDMS1);
					free(longDMS1);
					free(name2);
					free(state2);
					free(latDMS2);
					free(longDMS2);
					free(obj1);
					free(obj2);
				}
				else{
					fprintf(out, "No record was found for one or both of: %i, %i\n", id1, id2);
				}
			}
			free(cmd);
			fprintf(out, "----------------------------------------------------------------------\n");
		}
		row_length = getline(&row_attr, &attr_sz, in);
	}
	free(row_attr);
}

/**The function calculates the number of indices in an array.
 * It implements a while loop to calculate the length of an array
 * It takes the parameter which is a pointer to the array 
 * whose length has to be calculated.
 */
int calculate_len(uint32_t* ipt_arr){

	//Defining an integer variable count to keep track of the index
	int count = 0;

	//while loop to determine the number of elements in the array.
	while (ipt_arr[count] != 0){
		count++;
	}

	//returning the length of the given array.
	return count;
}

/**The function compares the offsets for different fid so that they 
 * can be sorted accordingly in the main function.
 * The function takes pointer parameters that point to two offsets of different fids.
 * It returns an int value based on the comparison.
 */
int compare_fid_offsets(const void* left, const void* right){
	return (*((uint32_t*)left) - *((uint32_t*)right));
}

/**The function determines the direction based on the coordinates given
 * It calculates the degrees minutes and seconds and also the direction
 * After determining the direction it defines a pointer to the direction string
 * It finally prints out the degrees minutes seconds and direction for the result.
 * These details are printed out in the details section for the output.
 */
char* defining_direct(char* coordinates, int lat_long){

	if (coordinates[0] == '0'){
		memmove(coordinates, coordinates+1, strlen(coordinates));
	}

	char direction;

	int value;

	int deg; 
	int mins;
	int sec;


	sscanf(coordinates, "%i%c", &value, &direction);

	deg = floor(value / 10000);
        mins = floor((value % 10000) / 100);
        sec = (value % 100);


	char* way;
	if (direction == 'W'){
		way = "West";
	}
	else if (direction == 'E'){
		way = "East";
	}
	else if (direction == 'S'){
		way = "South";
	}
	else if (direction == 'N'){
		way = "North";
	}
	char* store = (char*)malloc(19*sizeof(char));
	if (lat_long == 0){
		sprintf(store, "%id %im %is %s", deg, mins, sec, way);
	}
	else{
		sprintf(store, "%03id %im %is %s", deg, mins, sec, way);
	}
	free(coordinates);
	return store;
}


