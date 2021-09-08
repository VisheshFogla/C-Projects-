// Payload.c

//  On my honor:
//
//  - I have not discussed the C language code in my program with
//    anyone other than my instructor or the teaching assistants
//    assigned to this course.
//
//  - I have not used C language code obtained from another student,
//    the Internet, or any other unauthorized source, either modified
//    or unmodified.
//
//  - If any C language code or documentation used in my program
//    was obtained from an authorized source, such as a text book or
//    course notes, that has been clearly noted with a proper citation
//    in the comments of my program.
//
//  - I have not designed this program in such a way as to defeat or
//    interfere with the normal operation of the grading code.
//
//    Vishesh Fogla
//    visheshfogla

#include "Payload.h"
#include <stdlib.h>


/** Creates a dynamically-allocated Payload object initialized so that
 *  name is a deep copy of nom and weight == wt.
 * 
 *  Pre:  nom points to a proper C-string
 *        wt is initialized
 * 
 *  Returns: pointer to new Payload object   
 */
Payload* Payload_create(uint32_t wt) {
	
	Payload* newLoad = malloc(sizeof(Payload));
	newLoad->weight = wt;
	
	return newLoad;
}

/** Compares two Payload objects by comparing their name fields.
 * 
 *  Pre: left and right point to proper Payload objects
 * 
 *  Returns:  < 0 if left->name <  right->name  // UPDATE COMMENT!!
 *              0 if left->name == right->name
 *            > 0 if left->name >  right->name
 */
int32_t Payload_compare(const Payload* const left, const Payload* const right) {
   
   /***  Implementation here is up to you. ***/
   
   if(left->weight < right->weight)
   {
	   return -1;
   }
   else if(left->weight > right->weight)
   {
	   return 1;
   }
   return 0;
}

/** Destroys a user payload object; this one's trivial but a more
 *  interesting Payload type would require a more interesting
 *  destructor.
 *  Here, we do need to take into account whether:
 *    - the Payload object has dynamic content (here, no)
 *    - the Payload object itself was allocated dynamically (yes)
 */
void Payload_destroy(Payload* pLoad) {
   
   free(pLoad);
}

/** Writes a formatted representation of a Payload object.
 * 
 *  Pre:  fp is open on an output device
 *        pLoad points to a proper Payload object
 */
void Payload_display(FILE* fp, const Payload* const pLoad) {

	fprintf(fp, "%"PRIu32, pLoad->weight);
}
