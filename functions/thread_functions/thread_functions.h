#ifndef THREAD_FUNCTIONS_H_   /* Include guard */
#define THREAD_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* read_file_thread(void* parameters);
void* write_file_thread(void* parameters);
int thread_settings(pthread_t thread_id);
#endif