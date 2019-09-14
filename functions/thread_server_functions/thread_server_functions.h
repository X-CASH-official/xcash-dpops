#ifndef THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread();
int compare(const void* STRING1, const void* STRING2);
void* check_reserve_proofs_timer_thread();
void* check_delegates_online_status_timer_thread();
long long int add_block_to_blocks_found();
int calculate_block_reward_for_each_delegate(long long int block_reward);
void* block_height_timer_thread();
void* send_data_socket_thread(void* parameters);
void* send_and_receive_data_socket_thread(void* parameters);
void* socket_receive_data_thread(void* parameters);
#endif