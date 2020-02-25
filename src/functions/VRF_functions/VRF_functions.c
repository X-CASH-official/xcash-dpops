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
  int count;

  // define macros
  #define MINIMUM 1
  #define MAXIMUM 255

  do
  {
    memset(VRF_public_key,0,strlen((char*)VRF_public_key));
    memset(VRF_secret_key,0,strlen((char*)VRF_secret_key));

    for (count = 0; count < (int)crypto_vrf_SEEDBYTES; count++)
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
  int count;
  int count2; 

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
  for (count2 = 0, count = 0; count2 < (int)crypto_vrf_SECRETKEYBYTES; count2++, count += 2)
  {
    snprintf(vrf_secret_key+count,VRF_SECRET_KEY_LENGTH-1,"%02x",vrf_secret_key_data[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < (int)crypto_vrf_PUBLICKEYBYTES; count2++, count += 2)
  {
    snprintf(vrf_public_key+count,VRF_PUBLIC_KEY_LENGTH-1,"%02x",vrf_public_key_data[count2] & 0xFF);
  }

  fprintf(stderr,"\n");
  color_print("Public Key:","green");
  color_print(vrf_public_key,"green");
  fprintf(stderr,"\n");
  color_print("Secret Key:","green");
  color_print(vrf_secret_key,"green"); 
  
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sign_network_block_string
Description: Signs the network block string
Parameters:
  data - The signed data
  MESSAGE - The sign_data
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sign_network_block_string(char *data, const char* MESSAGE)
{
  // Variables
  char beta_string[VRF_BETA_LENGTH+1];
  char proof[VRF_PROOF_LENGTH+1];

  // define macros
  #define SIGN_NETWORK_BLOCK_STRING_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sign_network_block_string",25); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  memset(beta_string,0,sizeof(beta_string));
  memset(proof,0,sizeof(proof));

  // sign data
  if (VRF_sign_data(beta_string,proof,MESSAGE) == 0)
  {
    SIGN_NETWORK_BLOCK_STRING_ERROR("Could not sign the network block string");
  }
  memset(data,0,strlen(data));
  memcpy(data,proof,VRF_PROOF_LENGTH);
  memcpy(data+VRF_PROOF_LENGTH,beta_string,VRF_BETA_LENGTH);
  return 1;
  
  #undef SIGN_NETWORK_BLOCK_STRING_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: VRF_sign_data
Description: Sign data using the block verifiers ECDSA key
Parameters:
  beta - The beta string
  proof - The proof
  data - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int VRF_sign_data(char *beta_string, char *proof, const char* data)
{
  // Variables
  unsigned char proof_data[crypto_vrf_PROOFBYTES+1];
  unsigned char beta_string_data[crypto_vrf_OUTPUTBYTES+1];
  int count;
  int count2;

  memset(beta_string,0,strlen(beta_string));
  memset(proof,0,strlen(proof));
  memset(proof_data,0,sizeof(proof_data));
  memset(beta_string_data,0,sizeof(beta_string_data));

  // sign data
  if (crypto_vrf_prove((unsigned char*)proof_data,(const unsigned char*)secret_key_data,(const unsigned char*)data,(unsigned long long)strlen((const char*)data)) != 0 || crypto_vrf_proof_to_hash((unsigned char*)beta_string_data,(const unsigned char*)proof_data) != 0)
  {
    return 0;
  }

  // convert the data to a string
  for (count2 = 0, count = 0; count2 < (int)crypto_vrf_PROOFBYTES; count2++, count += 2)
  {
    snprintf(proof+count,VRF_PROOF_LENGTH,"%02x",proof_data[count2] & 0xFF);
  }
  for (count2 = 0, count = 0; count2 < (int)crypto_vrf_OUTPUTBYTES; count2++, count += 2)
  {
    snprintf(beta_string+count,VRF_BETA_LENGTH,"%02x",beta_string_data[count2] & 0xFF);
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: VRF_data_verify
Description: Verifies data
Parameters:
  PUBLIC_ADDRESS - The public address
  DATA_SIGNATURE - The data signature
  DATA - The data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int VRF_data_verify(const char* BLOCK_VERIFIERS_PUBLIC_KEY, const char* BLOCK_VERIFIERS_DATA_SIGNATURE, const char* DATA)
{
  // Variables
  char data[3];
  unsigned char public_key_data[crypto_vrf_PUBLICKEYBYTES+1];
  unsigned char proof_data[crypto_vrf_PROOFBYTES+1];
  unsigned char beta_string_data[crypto_vrf_OUTPUTBYTES+1];
  int count;
  int count2;

  memset(public_key_data,0,sizeof(public_key_data));
  memset(proof_data,0,sizeof(proof_data));
  memset(beta_string_data,0,sizeof(beta_string_data));

  // convert the public key, proof and beta string to a string
  for (count = 0, count2 = 0; count < VRF_PUBLIC_KEY_LENGTH; count2++, count += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&BLOCK_VERIFIERS_PUBLIC_KEY[count],2);
    public_key_data[count2] = (int)strtol(data, NULL, 16);
  }
  for (count = 0, count2 = 0; count < VRF_PROOF_LENGTH; count2++, count += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&BLOCK_VERIFIERS_DATA_SIGNATURE[count],2);
    proof_data[count2] = (int)strtol(data, NULL, 16);
  }
  for (count = VRF_PROOF_LENGTH, count2 = 0; count < VRF_BETA_LENGTH; count2++, count += 2)
  {
    memset(data,0,sizeof(data));
    memcpy(data,&BLOCK_VERIFIERS_DATA_SIGNATURE[count],2);
    beta_string_data[count2] = (int)strtol(data, NULL, 16);
  }

  return crypto_vrf_verify((unsigned char*)beta_string_data,(const unsigned char*)public_key_data,(const unsigned char*)proof_data,(const unsigned char*)DATA,(unsigned long long)strlen((const char*)DATA)) != 0 ? 0 : 1;
}