#ifndef DATABASE_FUNCTIONS_H_   /* Include guard */
#define DATABASE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int create_database_connection(void);
int get_delegates_total_voters_count(const char* DELEGATES_PUBLIC_ADDRESS);
int get_database_data(char *database_data, const char* DATABASE, const char* COLLECTION, const int THREAD_SETTINGS);
int get_database_data_hash(char *data_hash, const char* DATABASE, const char* COLLECTION);
#endif