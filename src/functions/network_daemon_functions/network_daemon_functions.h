#ifndef NETWORK_DAEMON_FUNCTIONS_H_   /* Include guard */
#define NETWORK_DAEMON_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result, const int HTTP_SETTINGS);
int submit_block_template(char* data, const int HTTP_SETTINGS);
int get_block_settings(char* block_height, const int HTTP_SETTINGS);
int get_block_reserve_byte_data_hash(char *reserve_byte_data_hash, const char* BLOCK_HEIGHT, const int HTTP_SETTINGS);
int verify_blockchain_network_transactions(char* transactions[], const size_t AMOUNT_OF_TRANSACTIONS, const int TRANSACTION_SETTINGS, const int MESSAGE_SETTINGS);
int get_current_block_height(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS);
int get_path(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_information(char *block_hash, char *block_reward, char *block_date_and_time);
int check_found_block(void);
#endif