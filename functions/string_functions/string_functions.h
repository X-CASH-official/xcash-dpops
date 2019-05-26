#ifndef STRING_FUNCTIONS_H_   /* Include guard */
#define STRING_FUNCTIONS_H_


/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result);
int create_json_data_from_database_document_array(struct database_document_fields* database_data, char* result, const char* DOCUMENT_FIELDS);
int create_json_data_from_database_multiple_documents_array(struct database_multiple_documents_fields* database_data, char* result, const char* DOCUMENT_FIELDS);
size_t string_count(const char* DATA, const char* STRING);
int string_replace(char *data, const char* STR1, const char* STR2);
int random_string(char *result, const size_t LENGTH);
#endif