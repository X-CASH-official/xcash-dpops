#ifndef SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define SHARED_DELEGATE_WEBSITE_THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

long long int add_block_to_blocks_found(void);
int get_delegates_total_voters(struct voters* voters);
int calculate_block_reward_for_each_delegate(long long int block_reward);
void* block_height_timer_thread(void* parameters);
void* payment_timer_thread(void* parameters);
#endif