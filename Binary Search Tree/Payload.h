// Payload.h
#ifndef PAYLOAD_H
#define PAYLOAD_H
#include <stdio.h>
#include <inttypes.h>


/*** DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/



/** User data type for testing BST implementation.
 * 
 *  A Payload object is proper iff:
 *    name points to a valid C-string
 *    weight is initialized
 */
struct _Payload {

   uint32_t weight;
};
typedef struct _Payload Payload;

/** Creates a dynamically-allocated Payload object initialized so that
 *  weight == wt.
 * 
 *  Pre:  wt is initialized
 * 
 *  Returns: pointer to new Payload object   
 */
Payload* Payload_create(uint32_t wt);

/** Destroys a user payload object
 *  Pre:  pLoad points to a proper Payload object
 * 
 *  Post: all dynamic memory associated with *pPayload, and the
 *           object *pPayload have been freed
 */
void Payload_destroy(Payload* pLoad);

/** Compares two Payload objects by comparing their name fields.
 * 
 *  Pre: left and right point to proper Payload objects
 * 
 *  Returns:  < 0 if left->weight <  right->weight
 *              0 if left->weight == right->weight
 *            > 0 if left->weight >  right->weight
 */
int32_t Payload_compare(const Payload* const left, const Payload* const right);

/** Writes a formatted representation of a Payload object.
 * 
 *  Pre:  fp is open on an output device
 *        pLoad points to a proper Payload object
 */
void Payload_display(FILE* fp, const Payload* const pLoad);

#endif
