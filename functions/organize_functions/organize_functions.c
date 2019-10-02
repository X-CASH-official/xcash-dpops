#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "organize_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: organize_invalid_reserve_proofs
Description: organize invalid reserve proofs
-----------------------------------------------------------------------------------------------------------
*/

int organize_invalid_reserve_proofs(const void* STRING1, const void* STRING2)
{
  return (strcmp((char*)STRING1, (char*)STRING2));
}



/*
-----------------------------------------------------------------------------------------------------------
Name: organize_delegates
Description: organize delegates
-----------------------------------------------------------------------------------------------------------
*/

int organize_delegates(const void* DELEGATES1, const void* DELEGATES2)
{
  // Variables
  size_t count;
  size_t count2;
  struct delegates* delegates1 = (struct delegates*)DELEGATES1;
  struct delegates* delegates2 = (struct delegates*)DELEGATES2;
  
  sscanf(delegates1->total_vote_count, "%zu", &count);
  sscanf(delegates2->total_vote_count, "%zu", &count2);

  return count2 - count;
}