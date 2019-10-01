#ifndef THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread(void* parameters);
int sort_invalid_reserve_proofs(const void* STRING1, const void* STRING2);
void* check_reserve_proofs_timer_thread(void* parameters);
void* check_delegates_online_status_timer_thread(void* parameters);
void* check_maximum_delegates_timer_thread(void* parameters);
long long int add_block_to_blocks_found(void);
int calculate_block_reward_for_each_delegate(long long int block_reward);
void* block_height_timer_thread(void* parameters);
void* payment_timer_thread(void* parameters);
void* send_data_socket_thread(void* parameters);
void* send_and_receive_data_socket_thread(void* parameters);
void* socket_receive_data_thread(void* parameters);
#endif