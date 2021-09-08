#ifndef PAYLOADWRAPPER_H
#define PAYLOADWRAPPER_H
#include <stdio.h>
#include <stddef.h>
#include "Payload.h"
#include "BST.h"


/*** DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/


/** Converts pointer to BSTNode into a pointer to the PayloadWrapper
 *  that the BSTNode is embedded in.
 *    STRUCT is the name of the outer structure type (PayloadWrapper)
 *    MEMBER is the name of the BSTNode element (node)
 *    BST_ELEM is a pointer to a BSTNode in the tree
 */
//#define Wrapper_Ptr(BST_ELEM, STRUCT, MEMBER)                              
//        ((STRUCT *) ((uint8_t *) (BST_ELEM) - offsetof (STRUCT, MEMBER)))


struct _PayloadWrapper {
	
	Payload* userdata;
	BSTNode node;
};
typedef struct _PayloadWrapper PayloadWrapper;

PayloadWrapper* PayloadWrapper_create(Payload* data);

/** Compares two Payload objects by comparing their name fields.
 * 
 *  Pre: left and right point to BSTNode objects in proper PayloadWrapper objects
 * 
 *  Returns:  Payload_compare(leftWrapper, rightWrapper) where
 *              *leftWrapper  == PayloadWrapper surrounding left
 *              *rightWrapper == PayloadWrapper surrounding right
 * 
 *  Calls:  Payload_compare()
 */
int32_t PayloadWrapper_compare(const BSTNode* const leftNode, const BSTNode* const rightNode);

/** Given a pointer to a BSTNode object, computes the address of the
 *  surrounding PayloadWrapper object.
 * 
 *  Pre:  pNode points to a proper BSTNode, contained in a proper PayloadWrapper
 * 
 *  Returns:  a pointer to the surrounding PayloadWrapper
 */
PayloadWrapper* PayloadWrapper_getPtr(const BSTNode* const pNode);

/** Writes a formatted representation of a Payload object.
 * 
 *  Pre:  fp is open on an output device
 *        pLoad points to a proper Payload object
 * 
 *  Calls: Payload_display()
 */
void PayloadWrapper_display(FILE* fp, const BSTNode* const pNode);

/** Deallocates all memory associated with a PayloadWrapper object.
 * 
 *  Pre:  pNode points to a proper BSTNode, contained in a proper 
 *           PayloadWrapper object
 * 
 *  Post: the PayloadWrapper object containing *pNode and
 *           the user data object associated with that wrapper
 *           object have been deallocated
 * 
 *  Calls: PayloadWrapper_GetPtr(), Payload_destroy()
 */
void PayloadWrapper_destroy(BSTNode* pNode);

#endif
