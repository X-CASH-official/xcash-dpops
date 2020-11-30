#ifndef SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_previous_block_producer(void);
long long int add_block_to_blocks_found(void);
int get_delegates_total_voters_count(const char* DELEGATES_PUBLIC_ADDRESS);
long long int get_delegates_total_voters(struct voters* voters);
int calculate_block_reward_for_each_delegate(long long int block_reward);
void* block_height_timer_thread(void* parameters);
long long int payment_timer_send_payment_and_update_databases(const char* PUBLIC_ADDRESS,const char* CURRENT_TOTAL,const char* TOTAL,const char* TX_HASH,const char* TX_KEY);
int payment_timer_update_inactivity_count(const char* PUBLIC_ADDRESS,const char* CURRENT_TOTAL,const char* INACTIVITY_COUNT);
void* payment_timer_thread(void* parameters);
#endif