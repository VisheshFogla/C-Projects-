//  Test driver for BST project used in CS 2506 Spring 2020.
//  Modified to let user specify seed value for testing.
//
//  Depends on the following modules:
//     Payload.h / Payload.c                  user payload
//     PayloadWrapper.h / PayloadWrapper.c    semi-generic payload wrapper
//     BST.h / BST.c                          user's BST implementation
//     Monk.h / Monk.o                        test/grading code
//
//  Compile with:
//
//    gcc -o c01driver -std=c11 -Wall -W -ggdb3 c01driver.c BST.c Payload.c PayloadWrapper.c Monk.o
//
//  Invocation: c01driver [-seed XXXXXXXX]
//    where XXXXXXX is used to seed the random generator.
//
//  The given test/grading code writes a separate log file showing results 
//  from each tested feature:
//     initLog.txt
//     insertLog.txt
//     findLog.txt
//
//  This code writes a short summary log file including score data.
//
//  Use of goto:
//     The implementation below makes use of goto in order to provide a 
//     single exit point from the function main().  This is one use of
//     goto that some C developers consider to be acceptable.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "BST.h"
#include "PayloadWrapper.h"
#include "Payload.h"
#include "Monk.h"
 
int main(int argc, char** argv) {
   
   // Set seed value for testing; use system clock unless user has specified
   // a seed value in the invocation:
   uint32_t seed = time( NULL );            // get system clock, for default
   if ( argc == 3 && strcmp(argv[1], "-seed") == 0 ) {
      seed = atoi(argv[2]);
   }
   
   // Store seed value into testing module:
   setSeed(seed);
   
   // Create file to hold brief summary of testing results.
   FILE* summary = fopen("summaryLog.txt", "w");
   if ( summary == NULL ) {
       printf("Failed to create summary log file.\n");
       exit(1);
   }
   
   // Test tree creation; if this fails, no other features are tested.
   bool treeInitOK = checkTreeInitialization( );
   if ( !treeInitOK ) {
      fprintf(summary, "Tree initialization failed.\n");
      fprintf(summary, "See tree initialization log for details.\n");
      fprintf(summary, "Aborting remaining tests...\n");
      goto exitTests;
   }
   else {
	   fprintf(summary, "Tree initialization was OK.\n");
   }
   
   // Test tree insertion; if this fails, no other features are tested.
   bool treeInsertionOK = checkTreeInsertion( );
   if ( !treeInsertionOK ) {
      fprintf(summary, "Tree insertion failed.\n");
      fprintf(summary, "See tree insertion log for details.\n");
      fprintf(summary, "Aborting remaining tests...\n");
      goto exitTests;
   }
   else {
	   fprintf(summary, "Tree insertion was OK.\n");
   }
   
   // Test tree search:
   bool treeFindOK = checkTreeFind( );
   if ( !treeFindOK ) {
      fprintf(summary, "Tree find failed.\n");
      fprintf(summary, "See tree find log for details.\n");
   }
   else {
	   fprintf(summary, "Tree find was OK.\n");
   }
   
   // Common, single exit point from main(), targeted by goto's above.
exitTests:
   writeSummary(summary);
   fclose(summary);
    
   return 0;
}
