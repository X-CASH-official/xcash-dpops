#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include "structures.h"

#include "file_functions.h"
#include "thread_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: read_file_thread
Description: Reads a file on a separate thread
Parameters:
  parameters - A pointer to the read_file_thread_parameters struct
  struct read_file_thread_parameters
    result - The data read from the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_file_thread(void* parameters)
{
  struct read_file_thread_parameters* data = parameters;
  int settings = read_file(data->result, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: write_file_thread
Description: Writes a file on a separate thread
Parameters:
  parameters - A pointer to the write_file_thread_parameters struct
  struct write_file_thread_parameters
    DATA - The data to write to the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* write_file_thread(void* parameters)
{
  struct write_file_thread_parameters* data = parameters;
  int settings = write_file(data->DATA, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: thread_settings
Description: Waits for a thread to return a value, and returns the value from the thread
Parameters:
   thread_id - The thread id of the thread
Return: The value that the thread returned
-----------------------------------------------------------------------------------------------------------
*/

int thread_settings(pthread_t thread_id)
{
  void* thread_settings;
  pthread_join(thread_id, &thread_settings);
  int settings = (intptr_t)thread_settings;
  return settings;
}