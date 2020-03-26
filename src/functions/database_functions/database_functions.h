#ifndef DATABASE_FUNCTIONS_H_   /* Include guard */
#define DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_database_collection_exist(const char* DATABASE, const char* COLLECTION);
int get_database_data(char *database_data, const char* DATABASE, const char* COLLECTION);
int get_database_data_hash(char *data_hash, const char* DATABASE, const char* COLLECTION);
#endif