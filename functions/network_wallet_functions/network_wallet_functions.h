#ifndef NETWORK_WALLET_FUNCTIONS_H_   /* Include guard */
#define NETWORK_WALLET_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(const int MESSAGE_SETTINGS);
int data_verify(const int MESSAGE_SETTINGS, const char* PUBLIC_ADDRESS, const char* DATA_SIGNATURE, const char* DATA);
#endif