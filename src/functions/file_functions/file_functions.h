#ifndef FILE_FUNCTIONS_H_   /* Include guard */
#define FILE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

long get_file_size(const char* FILE_NAME);
long read_file(unsigned char *result, const char* FILE_NAME);
int write_file(const char* DATA, const char* FILE_NAME);
int append_file(const char* DATA, const char* FILE_NAME);
#endif