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

#include "PayloadWrapper.h"
#include <string.h>
#include <stdlib.h>

/** Creates a new PayloadWrapper object holding *pData.
 * 
 *  Pre:  *data is a proper Payload object
 * 
 *  Returns:  pointer to a dynamically-allocated proper PayloadWrapper
 *            that encapsulates pData
 */
PayloadWrapper* PayloadWrapper_create(Payload* pData) {
	
	PayloadWrapper* newWrapper = malloc(sizeof(PayloadWrapper));
   
	newWrapper->userdata = pData;
	newWrapper->node.lchild = NULL;
	newWrapper->node.rchild = NULL;
   
	return newWrapper;
}

/** Given a pointer to a BSTNode object, computes the address of the
 *  surrounding PayloadWrapper object.
 * 
 *  Pre:  pNode points to a proper BSTNode, contained in a proper PayloadWrapper
 * 
 *  Returns:  a pointer to the surrounding PayloadWrapper
 */
PayloadWrapper* PayloadWrapper_getPtr(const BSTNode* const pNode) {
  
   return (PayloadWrapper*) ((uint8_t*) pNode - offsetof(PayloadWrapper, node));

}

/** Compares two Payload objects by comparing their name fields.
 * 
 *  Pre: left and right point to proper Payload objects
 * 
 *  Returns:  < 0 if left->name <  right->name
 *              0 if left->name == right->name
 *            > 0 if left->name >  right->name
 */
int32_t PayloadWrapper_compare(const BSTNode* const leftNode, const BSTNode* const rightNode) {
   
   PayloadWrapper* leftWrapper  = NULL;
   PayloadWrapper* rightWrapper = NULL;
   
   leftWrapper = PayloadWrapper_getPtr(leftNode);
   rightWrapper = PayloadWrapper_getPtr(rightNode);
   
   return Payload_compare(leftWrapper->userdata, rightWrapper->userdata);
}

/** Deallocates all memory associated with a PayloadWrapper object.
 */
void PayloadWrapper_destroy(BSTNode* pNode) {
   
   PayloadWrapper* sampleWrapper = NULL;
   
   sampleWrapper = PayloadWrapper_getPtr(pNode);
   
   Payload_destroy(sampleWrapper->userdata);
   
   free(sampleWrapper);
   
}

/** Writes a formatted representation of a Payload object.
 * 
 *  Pre:  fp is open on an output device
 *        pNode points to a proper BSTNode inside a PayloadWrapper object
 */
void PayloadWrapper_display(FILE* fp, const BSTNode* const pNode) {
	
	const PayloadWrapper* wrapper  = PayloadWrapper_getPtr(pNode);
	Payload_display(fp, wrapper->userdata);
}



