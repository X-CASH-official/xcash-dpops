#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "string_functions.h"
#include "convert.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "VRF_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: VRF_test
Description: Test the VRF functions
Return: The number of passed VRF test
-----------------------------------------------------------------------------------------------------------
*/

int VRF_test()
{
  // Variables
	unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
	unsigned char vrf_proof[crypto_vrf_PROOFBYTES];
	unsigned char vrf_beta[crypto_vrf_OUTPUTBYTES];
	size_t count;

  // define macros
  #define VRF_TOTAL_TEST 4

  // reset the variables
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
  count_test = 0;

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mVRF test - Total test: %d\033[0m\n",VRF_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test
  // create a random VRF public key and secret key
  if (create_random_VRF_keys(vrf_public_key,vrf_secret_key) == 1)
  {
    color_print("PASSED! Test for creating a random VRF public key and secret key","green");
  }
  else
  {
    color_print("FAILED! Test for creating a random VRF public key and secret key","red");
  }
  
  



  // write the end test message
  if (count_test == VRF_TOTAL_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mVRF test - Passed test: %d, Failed test: 0\033[0m\n",VRF_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mVRF test - Passed test: %d, Failed test: %d\033[0m\n",count_test,VRF_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  } 
  return count_test;

  #undef VRF_TOTAL_TEST
}