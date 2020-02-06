#ifndef BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread(void* parameters);
int check_reserve_proofs_timer_create_message(char *block_verifiers_message);
int check_reserve_proofs_timer_update_database(void);
int select_random_unique_reserve_proof(struct votes* votes);
int send_invalid_reserve_proof_to_block_verifiers(struct votes* votes);
void* check_reserve_proofs_timer_thread(void* parameters);
void* send_and_receive_data_socket_thread(void* parameters);
void remove_inactive_delegates(int total_delegates, int total_inactive_delegates);
void* remove_inactive_delegates_timer_thread(void* parameters);
void* sync_network_data_nodes_database_timer_thread(void* parameters);
void* sync_all_block_verifiers_list_timer_thread(void* parameters);
#endif