#ifndef BLOCK_VERIFIERS_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int start_new_round(void);
int start_current_round_start_blocks(void);
int data_network_node_create_block(void);
int start_part_4_of_round(void);
int update_block_verifiers_list(void);
int update_databases(void);
int add_block_verifiers_round_statistics(const char* BLOCK_HEIGHT);
int add_round_statistics(void);
int calculate_main_nodes_roles(void);
int get_delegates_online_status();
#endif