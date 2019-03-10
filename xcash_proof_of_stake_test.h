#ifndef XCASH_PROOF_OF_STAKE_TEST_H_   /* Include guard */
#define XCASH_PROOF_OF_STAKE_TEST_H_

#define TEST_OUTLINE "-----------------------------------------------------------------------------------------------"

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int append_string_test();
int parse_json_data_test();
int random_string_test();
int string_count_test();
int string_replace_test();
int send_wallet_http_request_test();
void* create_server_on_separate_thread();
int read_and_write_file_test();
int send_and_receive_data_socket_test();
int analysing_code_test();
size_t get_program_memory_usage(char* process_id_file);
int reset_variables_allocated_on_the_heap_test();
void test();

#endif