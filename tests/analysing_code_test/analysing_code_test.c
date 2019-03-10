#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "string_functions.h"

#include "variables_test.h"
#include "analysing_code_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: analysing_code_test
Description: Test all of the code in the program for the following 
All return or exit statements reset all the variables allocated on the heap before they exit the function
All define statments are then undefined at the end of the function
No unsafe string functions are ran in the code
Uninitialized variables are initialized before their code is run
Return: The number of passed analysing_code test
-----------------------------------------------------------------------------------------------------------
*/

int analysing_code_test()
{ 
  // structures
  struct uninitialized_variable_data {
      char* data[100];
      size_t count;
  };
  
  struct analysing_code_test_files {
      char* data[100];
      size_t count;
  };

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data1 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data4 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data5 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct uninitialized_variable_data uninitialized_variable_data;
  struct analysing_code_test_files analysing_code_test_files;
  FILE* file;  
  size_t analysing_code_test_files_count = 0;
  size_t count = 0;
  size_t counter = 0;
  size_t pointer_reset_all_count = 0;
  size_t pointer_reset_all_counter = 0;
  size_t define_count = 0;
  size_t undefine_count = 0;
  size_t uninitialized_variable_counter = 0;
  size_t data_count = 0;
  size_t data_counter = 0;
  size_t return_count = 0;
  size_t return_count_total = 0;
  size_t string_count = 0;
  size_t line_count = 1;  

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data1); \
  data1 = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL; \
  free(data4); \
  data4 = NULL; \
  free(data5); \
  data5 = NULL;

  // reset the variables
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;

  // initialize the arrays
  for (data_count = 0; data_count < 100; data_count++)
  {
    uninitialized_variable_data.data[data_count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  }  
  uninitialized_variable_data.count = 0;

  for (data_count = 0; data_count < 100; data_count++)
  {
    analysing_code_test_files.data[data_count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  }  
  memcpy(analysing_code_test_files.data[0],"xcash_proof_of_stake_website_server_test.c",strnlen("xcash_proof_of_stake_website_server_test.c",BUFFER_SIZE));
  analysing_code_test_files.count = 1;
  data_count = 0;

  // run the test

  // read the file
  for (analysing_code_test_files_count = 0; analysing_code_test_files_count < analysing_code_test_files.count; analysing_code_test_files_count++)
  {  
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  line_count = 1;
  // check if the file exist
  file = fopen(analysing_code_test_files.data[analysing_code_test_files_count],"r");
  if (file != NULL)
  {
    while(fgets(data_test,BUFFER_SIZE,file) != NULL)
    {
      // check for the function name
      if (strstr(data_test,"Name: ") != NULL && strstr(data_test,"\"Name: \"") == NULL)
      {
        // save the function name
        string_replace(data_test, "Name: ", "");
        memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
        memcpy(result_test,data_test,strnlen(data_test,BUFFER_SIZE));
        // reset the counters
        count = 0;
        counter = 0;
        pointer_reset_all_count = 0;
        pointer_reset_all_counter = 0;
        define_count = 0;
        undefine_count = 0;
        uninitialized_variable_counter = 0;
        data_count = 0;
        data_counter = 0;
        return_count = 0;
        return_count_total = 0;
        string_count = 0;
        memset(data,0,strnlen(data,BUFFER_SIZE));
        // initialize the arrays
        for (count = 0; count < 100; count++)
        {
          uninitialized_variable_data.data[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
        }  
        uninitialized_variable_data.count = 0;        
        count = 0;
        printf("Analysing function %s",data_test);
      }



      // check if the pointer_reset_all has the same amount of resets as malloc or calloc
      if ((strstr(data_test,"malloc") != NULL || strstr(data_test,"calloc") != NULL) && (strstr(data_test,".") == NULL))
      {
        count++;
      }      
      if (strstr(data_test,"#define pointer_reset_all") != NULL)
      {
        pointer_reset_all_counter = 1;
      }
      if (pointer_reset_all_counter == 1 && strstr(data_test,"free") != NULL)
      {
        pointer_reset_all_count++;
      }
      if (pointer_reset_all_counter == 1 && strncmp(data_test,"\n",BUFFER_SIZE) == 0)
      {
        pointer_reset_all_counter = 0;
        if (count != pointer_reset_all_count)
        {
          printf("\033[1;31mpointer_reset_all does not contain enough resets\033[0m\n");
        }
        else
        {
          printf("\033[1;32mpointer_reset_all does contain enough resets\033[0m\n");
          count_test++;
        }
      }



     // check if each return, exit, _exit statement has a pointer_reset_all or all of the pointer_resets 
      if ((strstr(data_test,"return") != NULL || strstr(data_test,"exit(0);") != NULL || strstr(data_test,"_exit(0);") != NULL) && strstr(data_test,"//") == NULL)
      {        
        // the previous 5 lines should contain either a pointer_reset_all or the same amount of pointer_reset as calloc or malloc
        if (((strstr(data1,"pointer_reset_all") != NULL || strstr(data2,"pointer_reset_all") != NULL || strstr(data3,"pointer_reset_all") != NULL || strstr(data4,"pointer_reset_all") != NULL || strstr(data5,"pointer_reset_all") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
        {
          return_count++;
        }
        else
        {
          if (((strstr(data1,"pointer_reset(") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
          {
            counter++;
          }
          if (((strstr(data2,"pointer_reset(") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
          {
            counter++;
          }
          if (((strstr(data3,"pointer_reset(") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
          {
            counter++;
          }
          if (((strstr(data4,"pointer_reset(") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
          {
            counter++;
          }
          if (((strstr(data5,"pointer_reset(") != NULL) && (strstr(data1,"return") == NULL && strstr(data2,"return") == NULL && strstr(data3,"return") == NULL && strstr(data4,"return") == NULL && strstr(data5,"return") == NULL) && (strstr(data1,"exit(0);") == NULL && strstr(data2,"exit(0);") == NULL && strstr(data3,"exit(0);") == NULL && strstr(data4,"exit(0);") == NULL && strstr(data5,"exit(0);") == NULL) && (strstr(data1,"_exit(0);") == NULL && strstr(data2,"_exit(0);") == NULL && strstr(data3,"_exit(0);") == NULL && strstr(data4,"_exit(0);") == NULL && strstr(data5,"_exit(0);") == NULL)) || ((strstr(data1,"}") != NULL || strstr(data2,"}") != NULL || strstr(data3,"}") != NULL || strstr(data4,"}") != NULL || strstr(data5,"}") != NULL)))
          {
            counter++;
          }
          if (counter == count)
          {
            return_count++;
          }
          else if (counter != count)
          {
            printf("\033[1;31mreturn statement has not reset all variables allocated on the heap at line %zu \033[0m\n",line_count);
          }
          counter = 0;  
        }
        return_count_total++;
      }



      // check if each define statment in the function is undef at the end of the function
      if (strstr(data_test,"#define") != NULL)
      {
        define_count++;
      } 
      if (strstr(data_test,"#undef") != NULL)
      {
        undefine_count++;
      }  
      if (memcmp(data_test,"}",1) == 0)
      {
        if (define_count != undefine_count)
        {
          printf("\033[1;31mAll define statments are not undefined at the end of the function\033[0m\n");
        }
        else
        {
          printf("\033[1;32mAll define statments are undefined at the end of the function\033[0m\n");
          count_test++;
        }
        
        // check if any variables have been uninitialized and have ran in the code
        for (data_count = 0; data_count < uninitialized_variable_data.count; data_count++)
        {
          if (memcmp(uninitialized_variable_data.data[data_count],"0",1) != 0)
          {
            printf("\033[1;31mVariable %s is run in code and not initialized\033[0m\n",uninitialized_variable_data.data[data_count]);
          }
          else
          {
            data_counter++;
          }
        } 
        if (data_counter == uninitialized_variable_data.count)
        {
          printf("\033[1;32mAll variables are initialized before they are ran in the code\033[0m\n");
          count_test++;
        }  

        // check if any return or exit statements have not reset all variables allocated on the heap  
        if (return_count == return_count_total)
        {
          printf("\033[1;32mAll return or exit statements have reset all variables allocated on the heap\033[0m\n");
          count_test++;
        }

        // check if no unsafe string functions are ran in the code
        if (string_count == 0)
        {
          printf("\033[1;32mNo unsafe string functions are ran in the code\033[0m\n");
          count_test++;
        }

        // reset the arrays
        for (count = 0; count < 100; count++)
        {
          pointer_reset(uninitialized_variable_data.data[count]);
        }
        uninitialized_variable_data.count = 0;        
        count = 0;
      }



      // check for unsafe functions
      if ((strstr(data_test,"strcmp") != NULL || strstr(data_test,"strcpy") != NULL || strstr(data_test,"strcat") != NULL || strstr(data_test,"sprintf") != NULL || strstr(data_test,"strlen") != NULL) && (strstr(data_test,"\"strcmp\"") == NULL && strstr(data_test,"\"strcpy\"") == NULL && strstr(data_test,"\"strcat\"") == NULL && strstr(data_test,"\"sprintf\"") == NULL && strstr(data_test,"\"strlen\"") == NULL))
      {
        printf("\033[1;31mUnsafe string function at line %zu \033[0m\n",line_count);
        string_count++;
      }



      // check to see if variables that are uninitialized are initialized before their code is run
      if (strstr(data_test,"// Variables") != NULL && strstr(data_test,"\"// Variables\"") == NULL)
      {
        uninitialized_variable_counter = 1;
      }
      else if (uninitialized_variable_counter == 1 && (strstr(data_test,"=") == NULL && strstr(data_test,"malloc") == NULL && strstr(data_test,"calloc") == NULL && strstr(data_test,"struct") == NULL) && strncmp(data_test,"\n",BUFFER_SIZE) != 0)
      {
        string_replace(data_test,"(char*)malloc(BUFFER_SIZE)","");
        string_replace(data_test,"(char*)calloc(BUFFER_SIZE,sizeof(char))","");        
        string_replace(data_test,"=","");
        string_replace(data_test,"[BUFFER_SIZE]","");
        string_replace(data_test,"FILE*","");
        string_replace(data_test,"char*","");
        string_replace(data_test,"char","");
        string_replace(data_test,"int","");
        string_replace(data_test,"size_t","");
        string_replace(data_test," ","");
        string_replace(data_test,"\n","");
        string_replace(data_test,";","");
        memcpy(uninitialized_variable_data.data[uninitialized_variable_data.count],data_test,strnlen(data_test,BUFFER_SIZE));
        uninitialized_variable_data.count++;
      }
      if (uninitialized_variable_counter == 1 && strncmp(data_test,"\n",BUFFER_SIZE) == 0)
      {
        uninitialized_variable_counter = 0;
      }
      if (uninitialized_variable_counter == 0 && (strstr(data_test,"=") != NULL || strstr(data_test,"memset") != NULL || strstr(data_test,"fopen") != NULL))
      {
        // check for each variable in the uninitialized_variable_data
        for (data_count = 0; data_count < uninitialized_variable_data.count; data_count++)
        {
          // if its is match and is being initialized then remove it from the struct
          if (strstr(data_test,uninitialized_variable_data.data[data_count]) != NULL)
          {
            // check if the variable is being initialized
            memset(data,0,strnlen(data,BUFFER_SIZE));
            memcpy(data,"memset(",7);
            memcpy(data+7,uninitialized_variable_data.data[data_count],strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
            if (strstr(data_test,data) != NULL)
            {
              memset(uninitialized_variable_data.data[data_count],0,strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
              memcpy(uninitialized_variable_data.data[data_count],"0",1);
            }
            memset(data,0,strnlen(data,BUFFER_SIZE));
            memcpy(data,"memset(&",8);
            memcpy(data+8,uninitialized_variable_data.data[data_count],strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
            if (strstr(data_test,data) != NULL)
            {
              memset(uninitialized_variable_data.data[data_count],0,strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
              memcpy(uninitialized_variable_data.data[data_count],"0",1);
            }
            memset(data,0,strnlen(data,BUFFER_SIZE));
            memcpy(data,"pointer_reset(",14);
            memcpy(data+14,uninitialized_variable_data.data[data_count],strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
            if (strstr(data_test,data) != NULL)
            {
              memset(uninitialized_variable_data.data[data_count],0,strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
              memcpy(uninitialized_variable_data.data[data_count],"0",1);
            }
            memset(data,0,strnlen(data,BUFFER_SIZE));            
            memcpy(data,uninitialized_variable_data.data[data_count],strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
            memcpy(data+strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE)," = ",3);
            if (strstr(data_test,data) != NULL)
            {
              memset(uninitialized_variable_data.data[data_count],0,strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
              memcpy(uninitialized_variable_data.data[data_count],"0",1);
            }
            if (strstr(data_test,"fopen") != NULL)
            {
              memset(uninitialized_variable_data.data[data_count],0,strnlen(uninitialized_variable_data.data[data_count],BUFFER_SIZE));
              memcpy(uninitialized_variable_data.data[data_count],"0",1);
            }
          }
        }
      }


      // save the previous 5 lines of the file
      memcpy(data5,data4,strnlen(data4,BUFFER_SIZE)); 
      memcpy(data4,data3,strnlen(data3,BUFFER_SIZE));
      memcpy(data3,data2,strnlen(data2,BUFFER_SIZE));
      memcpy(data2,data1,strnlen(data1,BUFFER_SIZE));
      memcpy(data1,data_test,strnlen(data_test,BUFFER_SIZE));

      // reset the varaible
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));

      line_count++;


    }
   }
  }
  
  pointer_reset_all;  
  return count_test;
  
  #undef pointer_reset_all
}