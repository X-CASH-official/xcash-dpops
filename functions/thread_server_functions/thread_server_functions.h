#ifndef THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread();
void* check_reserve_proofs_timer_thread();
void* check_delegates_online_status_timer_thread();
void* send_data_socket_thread_1(void* parameters);
void* send_data_socket_thread_2(void* parameters);
void* send_data_socket_thread_3(void* parameters);
void* send_data_socket_thread_4(void* parameters);
#endif