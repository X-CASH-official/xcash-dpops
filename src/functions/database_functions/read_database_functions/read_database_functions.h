#ifndef READ_DATABASE_FUNCTIONS_H_   /* Include guard */
#define READ_DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int read_document_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, char *result);
int read_document_field_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, const char* FIELD_NAME, char *result);
int database_document_parse_json_data(const char* DATA, struct database_document_fields *result);
int database_multiple_documents_parse_json_data(const char* data, struct database_multiple_documents_fields *result, const int document_count);
int read_document_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_document_fields *result);
int read_multiple_documents_all_fields_from_collection(const char* DATABASE, const char* COLLECTION, const char* DATA, struct database_multiple_documents_fields *result, const size_t DOCUMENT_COUNT_START, const size_t DOCUMENT_COUNT_TOTAL, const int DOCUMENT_OPTIONS, const char* DOCUMENT_OPTIONS_DATA);
#endif