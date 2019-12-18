#ifndef BLOCKCHAIN_FUNCTIONS_H_   /* Include guard */
#define BLOCKCHAIN_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int varint_encode(const uint_fast64_t number, char *result, const uint_fast64_t RESULT_TOTAL_LENGTH);
uint_fast64_t varint_decode(uint_fast64_t varint);
int network_block_string_to_blockchain_data(const char* DATA, const char* BLOCK_HEIGHT, const int BLOCK_VERIFIERS_TOTAL);
int blockchain_data_to_network_block_string(char *result, const int BLOCK_VERIFIERS_TOTAL);
int add_data_hash_to_network_block_string(char* network_block_string, char *network_block_string_data_hash);
int verify_network_block_data(const int BLOCK_VALIDATION_SIGNATURES_SETTINGS, const int PREVIOUS_BLOCK_HASH_SETTINGS, const int TRANSACTIONS_SETTINGS, const char* BLOCK_HEIGHT, char* PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES, const int BLOCK_VERIFIERS_TOTAL);
#endif