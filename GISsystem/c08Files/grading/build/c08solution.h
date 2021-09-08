#include <stdint.h>
#include <stdio.h>
#include "StringHashTable.h"

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


struct _FIDObj {
	int id;
	uint32_t fid_offset;
};
typedef struct _FIDObj FIDObj;


/**Function for creating an FID Object. 
 *The function returns a pointer that points to the created object.
 *It sets the id and fid_offset for the newly created object.
 */
FIDObj* create_obj(int Id, uint32_t location);


/**Reads the input argument untill the last character in the name of the file.
 * Implements a while loop to achieve the desired function.
 * Takes two parameters: one is the input file and the other is the last character that 
 * it should read till.  
 */  
uint32_t read_argument(FILE* input, char last_char);


/** Function that has been provided for hashing.
 * The function aids in converting an integer from a string. 
 * Mystery Function
 */
uint32_t elfhash(const char* str);

/** The function is defined to complete the array indexes of the table. 
 * It takes several parameters such as file, the pointer to the array, StringHashTable and array_size.
 * The function also uses realloc for doubling the capacity of array, incase the array become full 
 * or should the limit of the array exceed. It also calls addEntry from the StringHashTable.h to keep adding entry
 * at specific locations in the table. 
 */
uint32_t complete_array(FILE* file, FIDObj** array, StringHashTable* table, uint32_t array_size);


/** The function compares the ids of two FID objects to sort the array. 
 * This function takes parameters which are pointers pointing to the 
 * ids of two different FID Objects and then returning the one which is 
 * lesser of the two. 
 */
int compare_Ids(const void *left, const void *right);

void read_data(FILE* in, FILE* out, FILE* data, FIDObj* array, StringHashTable* table, uint32_t numEle);


/**The function calculates the number of indices in an array.
 * It implements a while loop to calculate the length of an array
 * It takes the parameter which is a pointer to the array 
 * whose length has to be calculated.
 */
int calculate_len(uint32_t* ipt_arr);

/**The function compares the offsets for different fid so that they 
 * can be sorted accordingly in the main function.
 * The function takes pointer parameters that point to two offsets of different fids.
 * It returns an int value based on the comparison.
 */
int compare_fid_offsets(const void* left, const void* right);


/**The function determines the direction based on the coordinates given
 * It calculates the degrees minutes and seconds and also the direction
 * After determining the direction it defines a pointer to the direction string
 * It finally prints out the degrees minutes seconds and direction for the result.
 * These details are printed out in the details section for the output.
 */
char* defining_direct(char* coordinates, int lat_long);
