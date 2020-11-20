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
Name: get_file_size
Description: Gets the file size
Parameters:
  file_name - The file name
Return: 0 if an error has occured, otherwise the file length
-----------------------------------------------------------------------------------------------------------
*/

long get_file_size(const char* FILE_NAME)
{
  // Variables
  int test;
  FILE* file;
  long file_size = 0;

  // check if the file exist
  if ((file = fopen(FILE_NAME,"rb")) != NULL)
  {
    // the file exist, get the file size
    fseek(file, 0, SEEK_END);
    if ((file_size = ftell(file)) == -1)
    {
      file_size = 0;
    }
    fclose(file);
  }
  return file_size;
}



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
  if ((file = fopen(FILE_NAME,"rb")) != NULL)
  {
    // the file exist, read the data in the result
    fseek(file, 0, SEEK_END);
    if ((file_size = ftell(file)) == -1)
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

  if ((file = fopen(FILE_NAME,"w")) != NULL)
  {
    fprintf(file,"%s",DATA);
    fclose(file);
    settings = 1;
  }
  return settings;  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: append_file
Description: Appends data to a file
Parameters:
  DATA = The data to write to the file
  file_name - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int append_file(const char* DATA, const char* FILE_NAME)
{
  // Variables
  FILE* file;
  int settings = 0;

  if ((file = fopen(FILE_NAME,"a")) != NULL)
  {
    fprintf(file,"%s",DATA);
    fclose(file);
    settings = 1;
  }
  return settings;  
}
