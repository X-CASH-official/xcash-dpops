#ifndef ORGANIZE_FUNCTIONS_H_   /* Include guard */
#define ORGANIZE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int organize_invalid_reserve_proofs_settings(const void* STRING1, const void* STRING2);
int organize_delegates_settings(const void* DELEGATES1, const void* DELEGATES2);
int organize_delegates(struct delegates* delegates, const char* DATABASE_COLLECTION);
#endif