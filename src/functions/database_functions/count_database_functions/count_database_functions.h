#ifndef COUNT_DATABASE_FUNCTIONS_H_   /* Include guard */
#define COUNT_DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int count_documents_in_collection(const char* DATABASE, const char* COLLECTION, const char* DATA);
int count_all_documents_in_collection(const char* DATABASE, const char* COLLECTION);
#endif