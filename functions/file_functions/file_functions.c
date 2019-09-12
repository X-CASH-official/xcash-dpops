#include <stdio.h>
#include <stdlib.h>

#include "file_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: read_file
Description: Reads the file
Parameters:
  result - The data read from the file
  file_name - The file name
Return: 0 if an error has occured, otherwise the file length
-----------------------------------------------------------------------------------------------------------
*/

long read_file(unsigned char *result, const char* FILE_NAME)
{
  // Variables
  FILE* file;
  long file_size;

  // check if the file exist
  file = fopen(FILE_NAME,"rb");
  if (file != NULL)
  {
    // the file exist, read the data in the result
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    if (file_size == -1)
    {
      fclose(file);
      return 0;
    }
    fseek(file, 0, SEEK_SET); 
    if (fread(result, sizeof(unsigned char), file_size, file) != (size_t)file_size)
    {
      fclose(file);
      return 0;
    }
    fclose(file);
  }
  else
  {
    return 0;
  }  
  return file_size;
}

/*
-----------------------------------------------------------------------------------------------------------
Name: write_file
Description: Writes data to a file
Parameters:
  DATA = The data to write to the file
  file_name - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int write_file(const char* DATA, const char* FILE_NAME)
{
  // Variables
  FILE* file;
  int settings = 0;

  file = fopen(FILE_NAME,"w");
  if (file != NULL)
  {
    fprintf(file,"%s",DATA);
    fclose(file);
    settings = 1;
  }
  return settings;  
}