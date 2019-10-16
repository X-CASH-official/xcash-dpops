#ifndef UPDATE_DATABASE_FUNCTIONS_H_   /* Include guard */
#define UPDATE_DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int update_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME_AND_DATA, const int THREAD_SETTINGS);
int update_all_documents_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
#endif