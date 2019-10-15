#ifndef BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_SYNCHRONIZE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_databases_are_synced(const int SETTINGS);
int sync_all_block_verifiers_list(void);
int get_synced_block_verifiers(void);
int sync_check_reserve_proofs_database(int settings);
int sync_reserve_proofs_database(int settings);
int sync_check_reserve_bytes_database(int settings);
int sync_reserve_bytes_database(int settings);
int sync_check_delegates_database(int settings);
int sync_delegates_database(int settings);
int sync_check_statistics_database(int settings);
int sync_statistics_database(int settings);
#endif