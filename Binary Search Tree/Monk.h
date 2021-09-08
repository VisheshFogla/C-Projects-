#ifndef MONK_H
#define MONK_H
#include "BST.h"
#include "PayloadWrapper.h"
#include "Payload.h"
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>


/*** DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/


/**  Sets a seed for rand(), which is used to generate test data.
 */
void setSeed(uint32_t seed);

/**  Runs tests for BST_create().
 * 
 *   Returns:  true if all tests are passed; false otherwise
 */
bool checkTreeInitialization( );


/**  Runs tests for BST_insert().
 * 
 *   Returns:  true if all tests are passed; false otherwise
 */
bool checkTreeInsertion();


/**  Runs tests for BST_find().
 * 
 *   Returns:  true if all tests are passed; false otherwise
 */
bool checkTreeFind();


/**  Write summary of scores to a log file.
 * 
 *   Pre:  fp is open on a log file
 */
void writeSummary(FILE* fp);

 
#endif
