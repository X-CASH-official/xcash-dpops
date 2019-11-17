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

int create_random_VRF_keys(unsigned char *VRF_public_key, unsigned char *VRF_secret_key)
{  
  // Variables
  unsigned char data[crypto_vrf_SEEDBYTES];
  size_t count;

  // define macros
  #define MINIMUM 1
  #define MAXIMUM 255

  do
  {
    memset(VRF_public_key,0,strlen((char*)VRF_public_key));
    memset(VRF_secret_key,0,strlen((char*)VRF_secret_key));

    for (count = 0; count < crypto_vrf_SEEDBYTES; count++)
    {
      data[count] = ((rand() % (MAXIMUM - MINIMUM + 1)) + MINIMUM);
    }

    // create the VRF private and secret key
    crypto_vrf_keypair_from_seed(VRF_public_key, VRF_secret_key, data);

  } while (strnlen((char*)VRF_public_key,crypto_vrf_PUBLICKEYBYTES) != crypto_vrf_PUBLICKEYBYTES || strnlen((char*)VRF_secret_key,crypto_vrf_SECRETKEYBYTES) != crypto_vrf_SECRETKEYBYTES);
  
  return 1;

  #undef MINIMUM
  #undef MAXIMUM
}



/*
-----------------------------------------------------------------------------------------------------------
Name: generate_key
Description: Generates a random public and private key, to be used for the signing and verifying of messages between the block verifiers
-----------------------------------------------------------------------------------------------------------
*/

void generate_key()
{  
  // Variables
  unsigned char vrf_secret_key_data[crypto_vrf_SECRETKEYBYTES+1];
  unsigned char vrf_public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  char vrf_secret_key[VRF_SECRET_KEY_LENGTH+1];
  char vrf_public_key[VRF_PUBLIC_KEY_LENGTH+1];
  size_t count;
  size_t count2; 

  memset(vrf_secret_key,0,sizeof(vrf_secret_key)); 
  memset(vrf_public_key,0,sizeof(vrf_public_key)); 
  memset(vrf_secret_key_data,0,sizeof(vrf_secret_key_data)); 
  memset(vrf_public_key_data,0,sizeof(vrf_public_key_data)); 
  
  if (create_random_VRF_keys((unsigned char*)vrf_public_key_data,(unsigned char*)vrf_secret_key_data) != 1 || crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key_data) != 1)
  {
    color_print("Could not generate keys","red");
    return;
  }

  // convert the VRF data to a string
  for (count2 = 0, count = 0; count2 < crypto_vrf_SECRETKEYBYTES; count2++, count += 2)
  {
    snprintf(vrf_secret_key+count,VRF_SECRET_KEY_LENGTH-1,"%02x",vrf_secret_key_data[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
  {
    snprintf(vrf_public_key+count,VRF_PUBLIC_KEY_LENGTH-1,"%02x",vrf_public_key_data[count2] & 0xFF);
  }

  fprintf(stderr,"\n");
  color_print("Secret Key:","green");
  color_print(vrf_secret_key,"green");
  fprintf(stderr,"\n");
  color_print("Public Key:","green");
  color_print(vrf_public_key,"green");  
  
  return;
}