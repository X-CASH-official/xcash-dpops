#ifndef BLOCK_VERIFIERS_UPDATE_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_UPDATE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int update_block_verifiers_list(void);
int update_databases(void);
int add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT);
int add_round_statistics(void);
int calculate_main_nodes_roles(void);
int get_delegates_online_status(void);
#endif