// BST.h
#ifndef BST_H
#define BST_H
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>


/*** DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/



/** Generic binary tree node type.
 * 
 *  A BSTNode object is proper iff:
 *    - it is encapsulated in a PayloadWrapper object, and
 *    - lchild is NULL or points to a proper BSTNode object, and
 *    - rchild is NULL or points to a proper BSTNode object
 */
struct _BSTNode {
   struct _BSTNode* lchild;
   struct _BSTNode* rchild;
};
typedef struct _BSTNode BSTNode;

//struct _BSTNode;

/** Generic binary search tree type.  The implementation depends on three 
 *  user-supplied functions:
 * 
 *  compare takes pointers to two proper BSTNode objectss; the function
 *            accesses the user payloads, compares them, and returns:
 *              < 0 if user payload on left < user payload on right
 *                0 if user payload on left == user payload on right
 *              > 0 if user payload on left >  user payload on right
 *            (Of course what <, ==, and > mean is entirely up to the user.)
 *  
 *  display  takes an open file pointer and a pointer to a proper BSTNode
 *            object and writes a formatted display of the user-defined data
 *            object in the associated wrapper
 * 
 *  destroy takes a pointer to a proper BSTNode object; the function
 *            deallocates all dynamic memory associated with the payload
 *            wrapper and the user's payload data 
 * 
 *  A BST object is proper iff:
 *    - compare points to a user-supplied function as described, and
 *    - display points to a user-supplied function as described, and
 *    - root is NULL, or the user data object in *root is larger than every
 *      user data object in the left subtree of *root, and smaller than every
 *      user data object in the right subtree of *root, and the left and right
 *      subtrees of *root are both proper BST objects
 */
struct _BST {
   struct _BSTNode* root;
	int32_t (*compare)(const BSTNode* const left, const BSTNode* const right);   // pointer to the user's comparison function
	void    (*display)(FILE* fp, const BSTNode* const pNode);                    // pointer to the user's display function
	void    (*destroy)(BSTNode* pNode);                                          // pointer to user's destructor function
}; 
typedef struct _BST BST;

// Public interface for the BST:

/** Create a proper, empty binary search tree object.
 * 
 *  Pre: compare is the name of a user-defined function satisfying the conditions
 *               stated above
 *       display is the name of a user-defined function satisfying the conditions
 *               stated above
 * 
 *  Returns: a BST object with NULL root and configured to use the two user-supplied
 *           functions for comparing and printing user-defined data objects stored
 *           in the tree
 */
BST BST_create(int32_t (*compare)(const BSTNode* const left, const BSTNode* const right),
               void     (*display)(FILE* fp, const BSTNode* const pD),
               void     (*destroy)(BSTNode* pNode));

/** Inserts user data object into a BST, unless it duplicates an existing object.
 * 
 *  Pre:  pTree points to a proper BST object
 *        userData points to a proper BSSTNode object
 * 
 *  Returns: true iff the insertion was performed; the implementation will not
 *           insert a new element that is equal to one that's already in the
 *           BST (according to the user-supplied comparison function)
 */
bool  BST_insert(BST* const pTree, const BSTNode* const userNode);

/** Searches a proper BST for an occurence of a user data object that equals
 *  *pData (according to the user-supplied comparison function).
 * 
 *  Pre: pTree points to a proper BST object
 *       pData points to a proper BSTNode object
 * 
 *  Returns: pointer to matching user data object; NULL if no match is found
 */
BSTNode* BST_find(const BST* const pTree, const BSTNode* const userNode);

/** Deallocates all dynamic memory associated with a proper BST object.
 * 
 *  Pre:   *pTree is a proper BST object
 *  Post:  all the user payloads and payload wrappers associated with *pTree
 *             have been freed
 *         the BST object itself is NOT freed since it may or may not have
 *             been allocated dynamically; that's the responsibility of the caller
 *  
 *  Calls: Payload_destroy() to handle destruction of the user's data object
 */
void BST_destroy(BST* const pTree);

/** Writes a formatted display of the contents of a proper BST.
 * 
 *  Pre: fp is open on an output device
 *       pTree points to a proper BST object
 */
void  BST_display(FILE* fp, const BST* const pTree);


#endif
