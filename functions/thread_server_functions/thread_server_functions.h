#ifndef THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void* current_block_height_timer_thread(void* parameters);
void* check_reserve_proofs_timer_thread(void* parameters);
void* send_and_receive_data_socket_thread(void* parameters);
void* socket_receive_data_thread(void* parameters);
#endif