//BST.c

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

#include "BST.h"
#include <stdlib.h>
#include <stdio.h>

// Declare static functions here, as needed:

static void BST_displayHelper(FILE* fp, const BSTNode* pNode, uint32_t level,
                              void (*displayFn)(FILE* fp, const BSTNode* const pNode));

static void writeIndentation(FILE* fp, uint32_t level);

static BSTNode* insertHelp(BSTNode* root, const BSTNode* const userNode, int32_t (*compare)(const BSTNode* const left, const BSTNode* const right));

static BSTNode* BST_findHelper(const BSTNode* pNode, const BSTNode* const userNode, int32_t (*compare)(const BSTNode* const left, const BSTNode* const right));

static void destroyHelper(const BSTNode* pNode, void (*destroy)(BSTNode* pNode));

//////////////////////////////////////////////////////////////////////////////////// creation fn
/** Create a proper, empty binary search tree object.
 * 
 *  Pre: compare is the name of a user-defined function satisfying the BST specification
 *       display is the name of a user-defined function satisfying the BST specification
 *       destroy is the name of a user-defined function satisfying the BST specification
 * 
 *  Returns: a BST object with NULL root and configured to use the two user-supplied
 *           functions for comparings and printing user-defined data objects stored
 *           in the tree
 */
BST BST_create(int32_t (*compare)(const BSTNode* const left, const BSTNode* const right),
              void     (*display)(FILE* fp, const BSTNode* const pD),
              void     (*destroy)(BSTNode* pNode)) {
	
   BST newTree;
   
   /***  Implementation here is up to you. ***/
   
   newTree.root = NULL;
   newTree.compare = compare;
   newTree.display = display;
   newTree.destroy = destroy;
   
   return newTree;
}

//////////////////////////////////////////////////////////////////////////////////// insertion fns
/** Inserts user data object into a BST, unless it duplicates an existing object.
 * 
 *  Pre:  pTree points to a proper BST object
 *        userNode points to a BSTNode in a client wrapper object
 * 
 *  Returns: true iff the insertion was performed; the implementation will not
 *           insert a new element that is equal to one that's already in the
 *           BST (according to the user-supplied comparison function)
 */
bool BST_insert(BST* const pTree, const BSTNode* const userNode) {

	if(pTree->root == NULL)
	{
		pTree->root = (BSTNode*)userNode;
		
		return true;
	}	
	
	if(BST_find(pTree, userNode) != NULL)
	{
		return false;		
	}
	
	
	if(pTree->compare(userNode, pTree->root) < 0)
	{
		if(pTree->root->lchild == NULL)
		{
			pTree->root->lchild = (BSTNode*)userNode;			
		}
		
		pTree->root->lchild = insertHelp(pTree->root->lchild, userNode, pTree->compare);
		return true;
	}
	else if(pTree->compare(userNode, pTree->root) > 0)
	{
		if(pTree->root->rchild == NULL)
		{
			pTree->root->rchild = (BSTNode*)userNode;			
		}
		
		pTree->root->rchild = insertHelp(pTree->root->rchild, userNode, pTree->compare);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////// search fns
/** Searches a proper BST for an occurence of a user data object that equals
 *  *pData (according to the user-supplied comparison function).
 * 
 *  Pre: pTree points to a proper BST object
 *       userNode points to a proper BSTNode inside a user's wrapper object
 * 
 *  Returns: pointer to BSTNode containing a matching user data object; 
 *           NULL if no match is found
 */
BSTNode* BST_find(const BST* const pTree, const BSTNode* const userNode) {
   
	return BST_findHelper(pTree->root, userNode, pTree->compare);
	
}


//////////////////////////////////////////////////////////////////////////////////// destruction fns
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
void BST_destroy(BST* const pTree) {
   
   if(pTree->root == NULL)
   {
	   return;
   }  
	
   destroyHelper(pTree->root->rchild, pTree->destroy);
   destroyHelper(pTree->root->lchild, pTree->destroy);
   
   pTree->destroy(pTree->root);
}

/**
 * 
 * A Helper method for inserting node in binary tree.
 * 
 * retuns the node that is inserted in the tree. 
 */
BSTNode* insertHelp(BSTNode* root, const BSTNode* const userNode, int32_t (*compare)(const BSTNode* const left, const BSTNode* const right))
{
	if(root == NULL)
	{
		root = (BSTNode*)userNode;
		return root;
	}	
	
	if(compare(userNode, root) < 0)
	{
		root->lchild = insertHelp(root->lchild, userNode, compare);
	}
	else if(compare(userNode, root) > 0)
	{
		root->rchild = insertHelp(root->rchild, userNode, compare);
	}
	
	return root;
	
}


/**
 * A helper method to find a given node in the binary tree
 * 
 * 
 * returns the node that is to be found.
 * 
 */
BSTNode* BST_findHelper(const BSTNode* pNode, const BSTNode* const userNode, int32_t (*compare)(const BSTNode* const left, const BSTNode* const right))
{
	
	
	if((pNode == NULL) || (compare(userNode, pNode) == 0))
	{
		return (BSTNode*)pNode;
	}
	
	if(compare(userNode, pNode) > 0)
	{
		return BST_findHelper(pNode->rchild, userNode, compare);
	}

	return BST_findHelper(pNode->lchild, userNode, compare);
}


/**
 * A helper method to destroy all nodes in the binary tree
 *  
 */
void destroyHelper(const BSTNode* pNode,void (*destroy)(BSTNode* pNode))
{
   
   if(pNode->rchild != NULL)
   {
	   destroyHelper(pNode->rchild, destroy);
	   
   }
   if(pNode->lchild != NULL)
   {
		   destroyHelper(pNode->lchild, destroy);
   }
   
   
   destroy((BSTNode*)pNode);   
   
}


//////////////////////////////////////////////////////////////////////////////////// display fns
/** Writes a formatted display of the contents of a proper BST.
 * 
 *  Pre: fp is open on an output device
 *       pTree points to a proper BST object
 */
void BST_display(FILE* fp, const BST* const pTree) {
	
	if ( pTree->root == NULL ) {
		fprintf(fp, "Tree is empty.\n");
		return;
	}
	
	BST_displayHelper(fp, pTree->root, 0, pTree->display);
}


static void BST_displayHelper(FILE* fp, const BSTNode* pNode, uint32_t level,
                              void (*display)(FILE* fp, const BSTNode* const pD)) {
	
	if ( pNode == NULL ) {
		writeIndentation(fp, level);
		fprintf(fp, "%c\n", '*');
		return;
	}
	
	BST_displayHelper(fp, pNode->rchild, level + 1, display);
	
	writeIndentation(fp, level);
	display(fp, pNode);
	
	BST_displayHelper(fp, pNode->lchild, level + 1, display);
}

static void writeIndentation(FILE* fp, uint32_t level) {
	
	uint32_t spaces = 10 * level;
	char fmt[100] = {0};   // want: "%3*<level>c"
	fmt[0] = '%';
	sprintf(fmt+1, "%"PRIu32"c", spaces);
	fprintf(fp, fmt, ' ');
}



