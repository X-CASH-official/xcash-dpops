#ifndef BLOCKCHAIN_FUNCTIONS_H_   /* Include guard */
#define BLOCKCHAIN_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

size_t varint_decode(size_t varint);
int network_block_string_to_blockchain_data(char* data);
int blockchain_data_to_network_block_string(char* result);
int verify_network_block_data();
#endif