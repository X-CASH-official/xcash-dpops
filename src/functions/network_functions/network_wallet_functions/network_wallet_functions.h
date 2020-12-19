#ifndef NETWORK_WALLET_FUNCTIONS_H_   /* Include guard */
#define NETWORK_WALLET_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(void);
long long int get_total_amount(void);
int send_payment(const char* DATA, char *tx_hash, char *tx_key, const int SETTINGS);
int check_reserve_proofs(char *result, const char* public_address, const char* reserve_proof);
#endif