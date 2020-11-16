#ifndef BLOCK_VERIFIERS_UPDATE_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_UPDATE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_block_verifiers_from_network_block(const int TOTAL_DELEGATES, const struct delegates* delegates, const size_t CURRENT_BLOCK_HEIGHT, const int SETTINGS);
int update_block_verifiers_list(void);
int update_databases(void);
int add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT);
int add_round_statistics(void);
int calculate_main_nodes_roles(void);
void check_for_updates(void);
int get_delegates_online_status(void);
#endif