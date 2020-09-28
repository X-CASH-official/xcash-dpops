#ifndef BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void sync_block_verifiers_database(void);
int sync_all_block_verifiers_list(const int SETTINGS);
int get_synced_block_verifiers(void);
void get_block_verifier_for_syncing_database(int settings, const char* DELEGATES_IP_ADDRESS, char *block_verifiers_ip_address);
int sync_check_reserve_proofs_specific_database(const char* DATABASE_DATA, const char* BLOCK_VERIFIERS_IP_ADDRESS);
int sync_check_reserve_bytes_specific_database(const char* DATABASE_DATA, const char* BLOCK_VERIFIERS_IP_ADDRESS, size_t starting_reserve_bytes_database, const size_t CURRENT_RESERVE_BYTES_DATABASE);
int sync_reserve_proofs_database(int settings, const char* DELEGATES_IP_ADDRESS);
int sync_reserve_bytes_database(int settings, const int RESERVE_BYTES_START_SETTINGS, const char* DELEGATES_IP_ADDRESS);
int sync_delegates_database(int settings, const char* DELEGATES_IP_ADDRESS);
int sync_statistics_database(int settings, const char* DELEGATES_IP_ADDRESS);
#endif