#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include <signal.h>
#include "sys/types.h"
#include "sys/socket.h"
#include <netdb.h> 
#include "netinet/in.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h> 

#include "xcash_proof_of_stake_functions_and_macros.h"
#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // iniltize the random number generator
  srand(time(0));
  
  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strncmp(parameters[1],"--test",BUFFER_SIZE) == 0)
    {
      test();
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    exit(0);
  }

  // start the server
  if (create_server(1) == 0)
  {
    exit(0);
  }
  return 0;   
}