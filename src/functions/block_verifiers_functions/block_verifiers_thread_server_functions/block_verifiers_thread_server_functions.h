#ifndef BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_   /* Include guard */
#define BLOCK_VERIFIERS_THREAD_SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_replayed_round(void);
void* current_block_height_timer_thread(void* parameters);
int check_reserve_proofs_timer_create_message(char *block_verifiers_message);
int check_reserve_proofs_timer_get_database_data(const int CURRENT_RESERVE_PROOF_COUNT);
int check_reserve_proofs_timer_update_delegates_total_vote_count(const int CURRENT_RESERVE_PROOF_COUNT);
int check_reserve_proofs_timer_update_delegates_score(const int CURRENT_RESERVE_PROOF_COUNT);
void check_reserve_proofs_timer_delete_reserve_proof(const int CURRENT_RESERVE_PROOF_COUNT);
int check_reserve_proofs_timer_update_database(void);
int select_random_unique_reserve_proof(struct reserve_proof* reserve_proof);
int send_invalid_reserve_proof_to_block_verifiers(const struct reserve_proof* reserve_proof);
void* check_reserve_proofs_timer_thread(void* parameters);
void remove_inactive_delegates(void);
void remove_block_heights_from_delegates(void);
#endif
