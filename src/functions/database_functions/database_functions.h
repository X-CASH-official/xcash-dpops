#ifndef DATABASE_FUNCTIONS_H_   /* Include guard */
#define DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int create_database_connection(void);
int insert_document_into_collection_array(const char* DATABASE, const char* COLLECTION, char** field_name_array, char** field_data_array, const size_t DATA_COUNT);
int insert_document_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
int insert_multiple_documents_into_collection_json(const char* DATABASE, const char* COLLECTION, const char* DATA, const size_t DATA_TOTAL_LENGTH, const int THREAD_SETTINGS);
int read_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, char *result, const int THREAD_SETTINGS);
int read_document_field_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME, char *result, const int THREAD_SETTINGS);
int database_document_parse_json_data(const char* DATA, struct database_document_fields *result);
int database_multiple_documents_parse_json_data(const char* data, struct database_multiple_documents_fields *result, const int document_count);
int read_document_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_document_fields *result, const int THREAD_SETTINGS);
int read_multiple_documents_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_multiple_documents_fields *result, const size_t DOCUMENT_COUNT_START, const size_t DOCUMENT_COUNT_TOTAL, const int DOCUMENT_OPTIONS, const char* DOCUMENT_OPTIONS_DATA, const int THREAD_SETTINGS);
int update_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME_AND_DATA, const int THREAD_SETTINGS);
int update_all_documents_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
int delete_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
int delete_collection_from_database(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS);
int count_documents_in_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const int THREAD_SETTINGS);
int count_all_documents_in_collection(const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS);
int get_delegates_total_voters_count(const char* DELEGATES_PUBLIC_ADDRESS);
int get_database_data(char *database_data, const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS);
int get_database_data_hash(char *data_hash, const char* DATABASE, const char* COLLECTION);
#endif