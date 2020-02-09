#ifndef STRING_FUNCTIONS_H_   /* Include guard */
#define STRING_FUNCTIONS_H_


/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result, const size_t RESULT_TOTAL_LENGTH);
int create_json_data_from_database_document_array(struct database_document_fields* database_data, char *result, const char* DOCUMENT_FIELDS);
int create_json_data_from_database_multiple_documents_array(struct database_multiple_documents_fields* database_data, char *result, const char* DOCUMENT_FIELDS);
int create_json_data_from_delegates_array(struct delegates* delegates, char *result, const char* DOCUMENT_FIELDS);
int create_json_data_from_votes_array(struct votes* votes, char *result, const char* DOCUMENT_FIELDS);
size_t string_count(const char* DATA, const char* STRING);
int parse_http_response(char *result);
int check_for_invalid_strings(const char* MESSAGE);
int string_replace(char *data, const size_t DATA_TOTAL_LENGTH, const char* STR1, const char* STR2);
int parse_reserve_bytes_data(char *result, const char* RESERVE_BYTES, const int ITEM);
int random_string(char *result, const size_t LENGTH);
#endif