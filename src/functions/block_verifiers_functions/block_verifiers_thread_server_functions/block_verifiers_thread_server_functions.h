#ifndef BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread(void* parameters);
void* check_reserve_proofs_timer_thread(void* parameters);
void* send_and_receive_data_socket_thread(void* parameters);
void* sync_network_data_nodes_database_timer_thread(void* parameters);
#endif