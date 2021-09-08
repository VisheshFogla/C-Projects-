// Monk.c -- BST test harness adapted from Java original
//  Modified to allow seed value to be set by user
//
#include "Monk.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// file-scoped seed value, set by user
static uint32_t randSeed = 1506282647;

// file-scoped score data used by writeSummary()
static uint32_t earnedPtsInit = 0;
static uint32_t maxPtsInit = 0;
static uint32_t earnedPtsInsert = 0;
static uint32_t maxPtsInsert = 0;
static uint32_t earnedPtsFind = 0;
static uint32_t maxPtsFind = 0;

/////////////////////////////////////////////////////////////////////////////////////////// reference soln
static BST refBST_create(int32_t (*compare)(const BSTNode* const left, const BSTNode* const right),
                      void     (*display)(FILE* fp, const BSTNode* const pD),
                      void     (*destroy)(BSTNode* pNode));
static bool refBST_insert(BST* const pTree, const BSTNode* const userNode);
static bool refBST_insertHelper(BSTNode** pNode, const BSTNode* const userNode, 
                             int32_t (*compare)(const BSTNode* const left, const BSTNode* const right));

////////////////////////////////////////////////////////////////////////////////////////// helper fns
static void Display(FILE* fp, const BST* pTree);
static void DisplayHelper(FILE* fp, const BSTNode* sRoot, uint32_t Level);
static void writePadding(FILE* fp, uint32_t Level);
static bool compareTrees(FILE* fp, const BST* stuTree, const BST* refTree, uint32_t* pScore, uint32_t itemValue);
static bool Equals(FILE* fp, const BST* stuTree, const BST* refTree, uint32_t* pScore, uint32_t itemValue);
static bool equalHelper(FILE* fp, const BSTNode* stu, const BSTNode* ref, uint32_t* pScore, uint32_t itemValue);
static uint32_t* fillBST(FILE* fp, BST* const stuTree, BST* const refTree, uint32_t Size, bool showIt);
static uint32_t* genData(uint32_t nValues);
static void randomShuffle(uint32_t* data, uint32_t sz);

void setSeed(uint32_t seed) {
   
   randSeed = seed;
}

static void logPoints(FILE* fp, uint32_t pts) {
    
   if ( pts > 0 )
      fprintf(fp, "[%2"PRIu32"]  ", pts);
   else
      fprintf(fp, "      ");
}


bool checkTreeInitialization( ) {
   
   srand( randSeed );
   
   FILE* fp = fopen("initLog.txt", "w");
   if ( fp == NULL ) {
      fprintf(stdout, "Failed to create log file for tree initialization test!\n");
      fprintf(stdout, "Aborting testing...\n");
      exit(1);
   }
   bool Passed = true;
   uint32_t Score = 0;
   uint32_t Max = 0;
   
   logPoints(fp, 0);
   fprintf(fp, "checkTreeInitialization results:\n");
   logPoints(fp, 0);
   fprintf(fp, "   Creating a new BST using BST_create().\n");
   BST myTree = BST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   
   // check whether root pointer is NULL
   Max += 30;
   if ( myTree.root != NULL ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  BST root was NOT null.\n");
      Passed = false;
   }
   else {
      Score += 30;
      logPoints(fp, 30);
      fprintf(fp, "   BST root was OK.\n");
   }
   
   // check comparison function pointer
   Max += 10;
   logPoints(fp, 0);
   if ( myTree.compare != PayloadWrapper_compare ) {
      logPoints(fp, 10);
      fprintf(fp, "   Bummer: pointer to comparison function is incorrect.\n");
      Passed = false;
   }
   else {
      Score += 10;
      fprintf(fp, "Pointer to comparison function is OK.\n");
   }
   
   // check destroy function pointer
   Max += 10;
   logPoints(fp, 0);
   if ( myTree.destroy != PayloadWrapper_destroy ) {
      logPoints(fp, 10);
      fprintf(fp, "   Bummer: pointer to destroy function is incorrect.\n");
      Passed = false;
   }
   else {
      Score += 10;
      fprintf(fp, "Pointer to destroy function is OK.\n");
   }
   
   // check display function pointer
   Max += 10;
   if ( myTree.display != PayloadWrapper_display ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer: pointer to display function is incorrect.\n");
      Passed = false;
   }
   else {
      Score += 10;
      logPoints(fp, 10);
      fprintf(fp, "Pointer to display fucntion is OK.\n");
   }
  
   fprintf(fp, "\n");
   fprintf(fp, "1 >> Tree initialization score:  %"PRIu32" / %"PRIu32"\n", Score, Max);
   fclose(fp);
   
   // set score data
   earnedPtsInit = Score;
   maxPtsInit = Max;
   
   // clean up tree
   BST_destroy(&myTree);
   
   return Passed;
}

bool checkTreeInsertion() {
   
   srand( randSeed );

   bool Passed = true;
   uint32_t Score = 0;
   uint32_t maxPoints = 0;
   
   FILE* fp = fopen("insertLog.txt", "w");
   if ( fp == NULL ) {
      fprintf(stdout, "Failed to create log file for tree initialization test!\n");
      fprintf(stdout, "Aborting testing...\n");
      goto exitInsert;
   }

   logPoints(fp, 0);
   fprintf(fp, "Begin test of BST insertion.\n");
   
   //////////////////////////////////////////////////////////////////////////// insertion of value into empty tree
   logPoints(fp, 0);
   fprintf(fp, "Checking insertion of value to empty tree...\n");
   
   BST T1 = BST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   BST refT1 = refBST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   
   uint32_t rootValueKey = 100 + rand() % 1000;
   Payload* pData = Payload_create( rootValueKey);
   PayloadWrapper* pW = PayloadWrapper_create(pData);
   logPoints(fp, 0);
   fprintf(fp, "Inserting value %"PRIu32"\n", rootValueKey);
   BST_insert( &T1, &pW->node );                         // tree with root only
   refBST_insert( &refT1, &pW->node ); 
   
   if ( T1.root == NULL ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  null root pointer.\n");
      fprintf(fp, "   Aborting insertion test.\n");
      goto exitInsert;
   }
   else {
      Score += 5;
      maxPoints += 5;
      logPoints(fp, 5);
      fprintf(fp, "   Root pointer is not null.\n");
      
      PayloadWrapper* pW = PayloadWrapper_getPtr(T1.root);
      if ( pW->userdata->weight != rootValueKey ) {
         logPoints(fp, 0);
         fprintf(fp, "   Bummer: root node contains wrong data value.\n");
         fprintf(fp, "   root node contains %"PRIu32"", pW->userdata->weight);
         logPoints(fp, 0);
         fprintf(fp, "   Aborting insertion test.\n");
         goto exitInsert;
      }
      else {
         logPoints(fp, 5);
         Score += 5;
         maxPoints += 5;
         fprintf(fp, "   Root node contains correct data value.\n");
      }
      if ( pW->node.lchild != NULL || T1.root->rchild != NULL ) { 
         logPoints(fp, 0);
         fprintf(fp, "   Bummer: root node contains one or more non-NULL pointers.\n");
         logPoints(fp, 0);
         fprintf(fp, "   Aborting insertion test.\n");
         goto exitInsert;
      }
      else {
         logPoints(fp, 5);
         Score += 5;
         maxPoints += 5;
         fprintf(fp, "   Root node pointers are both null.\n");
      }
   }
   
   //////////////////////////////////////////////////////////////////////////// creation of full two-level BST
   logPoints(fp, 0);
   fprintf(fp, "Checking insertion of two children of the current root node...\n");
   BSTNode* rootAddress = T1.root;                     // save address of root node; should NOT change on insertion
   
   uint32_t leftValue  = rootValueKey - 10;
   uint32_t rightValue = rootValueKey + 10;

   Payload* pDataL = Payload_create( leftValue );
   PayloadWrapper* pWL = PayloadWrapper_create(pDataL);
   if ( !BST_insert( &T1, &pWL->node ) ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer: BST_insert() returned false.\n");
      goto exitInsert;
   }

   Payload* pDataR = Payload_create( rightValue );
   PayloadWrapper* pWR = PayloadWrapper_create(pDataR);
   if ( !BST_insert( &T1, &pWR->node ) ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer: BST_insert() returned false.\n");
      goto exitInsert;
   }

   BSTNode* leftChildAddress  = T1.root->lchild;
   BSTNode* rightChildAddress = T1.root->rchild;
   
   if ( rootAddress != T1.root ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  insertion messed up root address.\n");
      logPoints(fp, 0);
      fprintf(fp, "   Aborting insertion test.\n");
      fclose(fp);
      goto exitInsert;
   }
   else {
      Score += 5;
      maxPoints += 5;
      logPoints(fp, 5);
      fprintf(fp, "   Insertion did not change root address.\n");
   }
   if ( leftChildAddress == NULL || rightChildAddress == NULL ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  root node has one or more NULL child pointers.\n");
      logPoints(fp, 0);
      fprintf(fp, "   Aborting insertion test.\n");
      fclose(fp);
      goto exitInsert;
   }
   else {
      maxPoints += 5;
      Score += 5;
      logPoints(fp, 5);
      fprintf(fp, "   Root node has two non-null child pointers.\n");
   }
   
   pW = PayloadWrapper_getPtr(T1.root->lchild);
   if ( pW->userdata->weight != leftValue ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  left child stores wrong value.\n");
      logPoints(fp, 0);
      fprintf(fp, "   Left child stores %"PRIu32"\n", pW->userdata->weight);
      Passed = false;
   }
   else {
      Score += 5;
      maxPoints += 5;
      logPoints(fp, 5);
      fprintf(fp, "   Left child stores correct value.\n");
   }

   pW = PayloadWrapper_getPtr(T1.root->rchild);
   if ( pW->userdata->weight != rightValue ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  right child stores wrong value.\n");
      logPoints(fp, 0);
      fprintf(fp, "   Right child stores %"PRIu32"\n", pW->userdata->weight);
      Passed = false;
   }
   else {
      maxPoints += 5;
      Score += 5;
      logPoints(fp, 5);
      fprintf(fp, "   Right child stores correct value.\n");
   }
   
   logPoints(fp, 0);
   fprintf(fp, "\n");
   
   //////////////////////////////////////////////////////////////////////////// try insertion of a duplicate
   logPoints(fp, 0);
   fprintf(fp, "   Let's see what the tree looks like now:\n");
   Display(fp, &T1);
   
   logPoints(fp, 0);
   fprintf(fp, "   Try inserting a value, %"PRIu32", that's already in the tree.\n", leftValue);

   if ( BST_insert( &T1, &pWL->node ) ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer: BST_insert() returned true; this should have been rejected.\n");
      fprintf(fp, "   Aborting insertion test...");
      Passed = false;
      goto exitInsert;
   }
   else {
      Score += 5;
      maxPoints += 5;
      logPoints(fp, 5);
      fprintf(fp, "   BST_insert() returned false; this was correctly rejected.\n");
   }
   
   logPoints(fp, 0);
   fprintf(fp, "   Let's actually check the tree now:\n");
   if ( !compareTrees(fp, &T1, &refT1, &Score, 1) ) {
      fprintf(fp, "    Bummer:  the student's tree did not match the reference tree.\n");
      fprintf(fp, "Here is the student's tree:\n\n");
      Display(fp, &T1);
      fprintf(fp, "Here is the reference tree:\n\n");
      Display(fp, &refT1);
      fprintf(fp, "    Aborting insertion test...\n");
      goto exitInsert;
   }
   else {
      fprintf(fp, "   Good, the student tree matched the reference tree.\n");
   }
   maxPoints += 3;   // 1 point ofr each node in the current tree

   BST_destroy(&T1);
   
   //////////////////////////////////////////////////////////////////////////// test of mass insertion
   logPoints(fp, 0);
   fprintf(fp, "Beginning general insertion test with a new, empty tree...\n");
   
   BST T2 = BST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   BST refT2 = refBST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   uint32_t treeSize = 69;
   maxPtsInsert = maxPoints + treeSize * 3;

   uint32_t* d1 = fillBST(fp, &T2, &refT2, treeSize, true);
   free(d1);
   fprintf(fp, "\n");
   
   logPoints(fp, 0);
   fprintf(fp, "   Resulting tree follows.  If the log ends suddenly, it's likely a pointer exception occurred.\n");
   Display(fp, &T2);

   if ( !compareTrees(fp, &T2, &refT2, &Score, 3) ) {
      fprintf(fp, "    Bummer:  the student's tree did not match the reference tree.\n");
      fprintf(fp, "Here is the reference tree:\n\n");
      Display(fp, &refT1);
      fprintf(fp, "    Aborting insertion test...\n");
      goto exitInsert;
   }
   else {
      fprintf(fp, "   Good, the student tree matched the reference tree.\n");
   }

   fprintf(fp, "\n");
   BST_destroy(&T2);
   BST_destroy(&refT2);

   //////////////////////////////////////////////////////////////////////////// report final results
exitInsert:
   fprintf(fp, "2 >> Tree insertion score:  %"PRIu32" / %"PRIu32"\n", Score, maxPtsInsert);
   fclose(fp);
   
   earnedPtsInsert = Score;
   
   return Passed;
}

bool checkTreeFind() {
   
   srand( randSeed );

   bool Passed = true;
   uint32_t Score = 0;
   FILE* fp = fopen("findLog.txt", "w");
   if ( fp == NULL ) {
      fprintf(stdout, "Failed to create log file for tree find test!\n");
      fprintf(stdout, "Aborting testing...\n");
      goto exitFind;
   }

   fprintf(fp, "Begin test of BST find.\n");
   fprintf(fp, "\n");

   BST T1 = BST_create(PayloadWrapper_compare, PayloadWrapper_display, PayloadWrapper_destroy);
   uint32_t treeSize = 50;
   uint32_t maxPoints = 5 * treeSize;

   uint32_t* data = fillBST(fp, &T1, NULL, treeSize, false);
   fprintf(fp, "\n");
   
   logPoints(fp, 0);
   fprintf(fp, "   Resulting tree follows.  If the log ends suddenly, it's likely a pointer exception occurred.\n");
   Display(fp, &T1);
   
   for (uint32_t idx = 0; idx < treeSize; idx++) {
	   
	   uint32_t currData = data[idx];
	   Payload* pPL = Payload_create(currData);
	   PayloadWrapper* pPW = PayloadWrapper_create(pPL);

	   Payload* pPL2 = Payload_create(currData);
	   
	   BSTNode* pFound = BST_find(&T1, &pPW->node);
	   
	   if ( pFound == NULL ) {
		  logPoints(fp, 0);
		  fprintf(fp, "   Bummer:  failed to find %"PRIu32"\n", currData);
		  Passed = false;
	   }
	   else {
		   PayloadWrapper* pFoundPW = PayloadWrapper_getPtr(pFound);
		   Payload* pFoundPayload = pFoundPW->userdata;
		   if ( Payload_compare(pFoundPayload, pPL2) != 0 ) {
			   logPoints(fp, 0);
			   fprintf(fp, "   Bummer:  found ");
			   Payload_display(fp, pFoundPayload);
			   fprintf(fp, " instead of %"PRIu32, currData);
			   fprintf(fp, "\n");
			   Passed = false;
		   }
		   else {
			   Score += 5;
			   logPoints(fp, 5);
			   fprintf(fp, "   Found %"PRIu32, currData);
			   fprintf(fp, "\n");
		   }
	   }
	   free(pPL);
	   free(pPW);
	   free(pPL2);
   }
   free(data);
   BST_destroy(&T1);
   
   //////////////////////////////////////////////////////////////////////////// report final results
exitFind:
   fprintf(fp, "3 >> Tree find score:  %"PRIu32" / %"PRIu32"\n", Score, maxPoints);
   fclose(fp);
   
   earnedPtsFind = Score;
   maxPtsFind = maxPoints;
   
   return Passed;
}

//////////////////////////////////////////////////////////////////////////////////////////////// static dislay fns
static void Display(FILE* fp, const BST* pTree) {

   if ( pTree->root == NULL ) {
      fprintf(fp, "   Tree is empty.\n");
      return;
   }
   return DisplayHelper(fp, pTree->root, 0);
}

static void DisplayHelper(FILE* fp, const BSTNode* sRoot, uint32_t Level) {

   if (sRoot == NULL) {
      writePadding(fp, Level);
      fprintf(fp, "*\n");
      return;
   }

   DisplayHelper(fp, sRoot->rchild, Level + 1);

   writePadding(fp, Level);
   PayloadWrapper* pW = PayloadWrapper_getPtr(sRoot);
   Payload* pPL = pW->userdata;
   Payload_display(fp, pPL);
   fprintf(fp, "\n");

   DisplayHelper(fp, sRoot->lchild, Level + 1);
}

static void writePadding(FILE* fp, uint32_t Level) {

    for (uint32_t i = 0; i < Level; i++) {
       fprintf(fp, "---");
    }
    if ( Level > 0 )
       fprintf(fp, " ");
}

/////////////////////////////////////////////////////////////////////////////////////// static tree filling fns
    
static uint32_t* fillBST(FILE* fp, BST* const stuTree, BST* const refTree, uint32_t Size, bool showIt) {

   logPoints(fp, 0);
   fprintf(fp, "   Filling BST with %"PRIu32" random integer values.\n", Size);
   
   if ( showIt ) {
      logPoints(fp, 0);
      fprintf(fp, "Inserted values: ");
   }
   
   uint32_t* data = genData(Size);
   
   for (uint32_t Count = 0; Count < Size; Count++) {
      
      uint32_t keyValue = data[Count];
      
      if ( showIt ) {
         fprintf(fp, "  %"PRIu32, keyValue);
      }

      Payload* pstuData = Payload_create( keyValue );
      PayloadWrapper* pstuWR = PayloadWrapper_create(pstuData);
      BST_insert( stuTree, &pstuWR->node );

      if ( refTree != NULL ) {
         Payload* prefData = Payload_create( keyValue );
         PayloadWrapper* prefWR = PayloadWrapper_create(prefData);
         refBST_insert( refTree, &prefWR->node );
	  }
   }
   
   if ( showIt ) {
      fprintf(fp, "\n");
   }
   
   return data;
}

static uint32_t* genData(uint32_t nValues) {
	
	uint32_t* data = calloc(nValues, sizeof(uint32_t));
	
	data[0] = rand() % 100;
	
	for (uint32_t idx = 1; idx < nValues; idx++) {
		data[idx] = data[idx-1] + 1 + rand() % 25;
	}
	
	randomShuffle(data, nValues);
	
	return data;
}

static void randomShuffle(uint32_t* data, uint32_t sz) {

	uint32_t* shuffled = calloc(sz, sizeof(uint32_t));
	
	for (uint32_t srcIdx = 0; srcIdx < sz; srcIdx++) {
		uint32_t targIdx = rand() % sz;
		while ( shuffled[targIdx] != 0 ) {
			targIdx = (targIdx + 1) % sz;
		}
		shuffled[targIdx] = data[srcIdx];
	}
	
	memcpy(data, shuffled, sz * sizeof(uint32_t));
	free(shuffled);
}

/////////////////////////////////////////////////////////////////////////////////////// static comparison fn
static bool compareTrees(FILE* fp, const BST* stuTree, const BST* refTree, uint32_t* pScore, uint32_t itemValue) {
    
    return Equals(fp, stuTree, refTree, pScore, itemValue);
}

static bool Equals(FILE* fp, const BST* stuTree, const BST* refTree, uint32_t* pScore, uint32_t itemValue) {
      
   if ( stuTree == NULL && refTree == NULL ) {
       logPoints(fp, itemValue);
       fprintf(fp, "     Good, said two nonexistent trees were equal.\n");
       *pScore += itemValue;
      return true;
   }
   if ( stuTree == NULL || refTree == NULL ) return false;
   return equalHelper(fp, stuTree->root, refTree->root, pScore, itemValue);
}

static bool equalHelper(FILE* fp, const BSTNode* stu, const BSTNode* ref, uint32_t* pScore, uint32_t itemValue) {
      
   if ( stu == NULL && ref == NULL ) {
     logPoints(fp, 0);
     fprintf(fp, "     Matched two empty leaves.\n");
     return true;
   }
   if ( stu == NULL && ref != NULL ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  student tree has empty leaf where reference tree has:\n");
      PayloadWrapper* pWR = PayloadWrapper_getPtr(ref);
      Payload* pPL = pWR->userdata;
      Payload_display(fp, pPL);
      fprintf(fp, "\n");
      return false;
   }
   if ( stu != NULL && ref == NULL ) {
      logPoints(fp, 0);
      fprintf(fp, "   Bummer:  reference tree has empty leaf where student tree has:\n");
      PayloadWrapper* pWR = PayloadWrapper_getPtr(stu);
      Payload* pPL = pWR->userdata;
      Payload_display(fp, pPL);
      fprintf(fp, "\n");
      return false;
   }

      
   PayloadWrapper* pstuWR = PayloadWrapper_getPtr(stu);
   Payload* pstuPL = pstuWR->userdata;
   PayloadWrapper* prefWR = PayloadWrapper_getPtr(ref);
   Payload* prefPL = prefWR->userdata;
   
   if ( Payload_compare(pstuPL, prefPL) != 0 ) {
      logPoints(fp, 0);
      fprintf(fp, "  Bummer:  student tree and reference tree have different values\n");
      fprintf(fp, "     student tree has value:   ");
      Payload_display(fp, pstuPL);
      fprintf(fp, "\n");
      fprintf(fp, "     where\n");
      fprintf(fp, "     reference tree has value: ");
      Payload_display(fp, prefPL);
      fprintf(fp, "\n");
      return false;
   }
   logPoints(fp, itemValue);
   fprintf(fp, "   Student tree and reference tree have value ");
   Payload_display(fp, pstuPL);
   fprintf(fp, " in same position\n");
   *pScore += itemValue;

   return ( equalHelper(fp, stu->lchild, ref->lchild, pScore, itemValue) &&
            equalHelper(fp, stu->rchild, ref->rchild, pScore, itemValue));
}

/////////////////////////////////////////////////////////////////////////////////////// score logger
void writeSummary(FILE* fp) {
	
	fprintf(fp, "\nScores from testing\n\n");
	
	fprintf(fp, "1 >> BST initialization:  %3"PRIu32" / %3"PRIu32"\n", earnedPtsInit, maxPtsInit);
	fprintf(fp, "2 >> BST insertion:       %3"PRIu32" / %3"PRIu32"\n", earnedPtsInsert, maxPtsInsert);
	fprintf(fp, "3 >> BST search:          %3"PRIu32" / %3"PRIu32"\n", earnedPtsFind, maxPtsFind);
	fprintf(fp, "\n");
}

/////////////////////////////////////////////////////////////////////////////////////// static fns from reference BST
static BST refBST_create(int32_t (*compare)(const BSTNode* const left, const BSTNode* const right),
              void     (*display)(FILE* fp, const BSTNode* const pD),
              void     (*destructor)(BSTNode* pNode)) {
	
	BST newTree;
	newTree.root = NULL;
	newTree.compare = compare;
	newTree.display = display;
	newTree.destroy = destructor;
	return newTree;
}

static bool refBST_insert(BST* const pTree, const BSTNode* const userNode) {
    
    if ( pTree == NULL || userNode == NULL ) return false;
    
    return refBST_insertHelper(&pTree->root, userNode, pTree->compare);
}

static bool refBST_insertHelper(BSTNode** pNode, const BSTNode* const userNode, 
                             int32_t (*compare)(const BSTNode* const left, const BSTNode* const right)) {
    
    if ( *pNode == NULL ) {
        *pNode = (BSTNode*) userNode;
        return true;
    }
    
    int32_t direction = compare(userNode, *pNode);
    
    if ( direction < 0 ) {
        return refBST_insertHelper(&((*pNode)->lchild), userNode, compare);
    }
    else if ( direction > 0 ) {
        return refBST_insertHelper(&((*pNode)->rchild), userNode, compare);
    }
    else {
        // duplicate entry; do not insert
        return false;
    }
}



