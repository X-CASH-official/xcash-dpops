#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "convert.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: create_random_VRF_keys
Description: Creates a random seed, and uses the seed to generate random VRF public key and a random VRF secret key
Parameters:
  public_key - The VRF public key
  secret_key - The VRF secret key
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_random_VRF_keys(unsigned char *public_key, unsigned char *secret_key)
{  
  // Variables
  unsigned char data[crypto_vrf_SEEDBYTES];
  size_t count;

  // define macros
  #define MINIMUM 1
  #define MAXIMUM 255

  start:

  memset(public_key,0,strlen(public_key));
  memset(secret_key,0,strlen(public_key));

  for (count = 0; count < crypto_vrf_SEEDBYTES; count++)
  {
    data[count] = ((rand() % (MAXIMUM - MINIMUM + 1)) + MINIMUM);
  }

  // create the VRF private and secret key
  crypto_vrf_keypair_from_seed(public_key, secret_key, data);

  // check the byte amounts
  if (strnlen(public_key,crypto_vrf_PUBLICKEYBYTES) != crypto_vrf_PUBLICKEYBYTES || strnlen(secret_key,crypto_vrf_SECRETKEYBYTES) != crypto_vrf_SECRETKEYBYTES)
  {
    goto start;
  }

  return 1;
}