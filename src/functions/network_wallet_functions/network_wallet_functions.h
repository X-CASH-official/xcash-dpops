#ifndef NETWORK_WALLET_FUNCTIONS_H_   /* Include guard */
#define NETWORK_WALLET_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(const int MESSAGE_SETTINGS);
int send_payment(const char* PUBLIC_ADDRESS, const char* TOTAL, char *tx_hash, char *tx_key);
int check_reserve_proofs(char *result, const char* public_address, const char* reserve_proof, const int HTTP_SETTINGS);
#endif