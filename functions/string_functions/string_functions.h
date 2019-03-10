#ifndef STRING_FUNCTIONS_H_   /* Include guard */
#define STRING_FUNCTIONS_H_


/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result);
size_t string_count(const char* DATA, const char* STRING);
int string_replace(char *data, const char* STR1, const char* STR2);
int random_string(char *result, const size_t LENGTH);
#endif