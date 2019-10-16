#ifndef INSERT_DATABASE_FUNCTIONS_H_   /* Include guard */
#define INSERT_DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int insert_document_into_collection_array(const char* DATABASE, const char* COLLECTION, char** field_name_array, char** field_data_array, const size_t DATA_COUNT);
int insert_document_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
int insert_multiple_documents_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const size_t DATA_TOTAL_LENGTH, const int THREAD_SETTINGS);
#endif