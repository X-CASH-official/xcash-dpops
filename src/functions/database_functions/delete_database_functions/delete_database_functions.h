#ifndef DELETE_DATABASE_FUNCTIONS_H_   /* Include guard */
#define DELETE_DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int delete_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA);
int delete_collection_from_database(const char* DATABASE, const char* COLLECTION);
int delete_database(const char* DATABASE);
#endif