#ifndef NETWORK_WALLET_FUNCTIONS_H_   /* Include guard */
#define NETWORK_WALLET_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(const int MESSAGE_SETTINGS);
int send_payment(const char* PUBLIC_ADDRESS, const char* TOTAL, char *tx_hash, char *tx_key);
int sign_network_block_string(char *data, const char* message, const int HTTP_SETTINGS);
int data_verify(const int MESSAGE_SETTINGS, const char* PUBLIC_ADDRESS, const char* DATA_SIGNATURE, const char* DATA);
int check_reserve_proofs(char *result, const char* public_address, const char* reserve_proof, const int HTTP_SETTINGS);
#endif