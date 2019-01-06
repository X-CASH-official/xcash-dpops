#ifndef XCASH_PROOF_OF_STAKE_TEST_H_   /* Include guard */
#define XCASH_PROOF_OF_STAKE_TEST_H_

#define TEST_OUTLINE "---------------------------------------------------------------------------------"

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int append_string_test();
int parse_json_data_test();
int random_string_test();
void* create_server_on_separate_thread(void* parameters);
int send_wallet_http_request_test();
int send_and_receive_data_socket_test();
void test();

#endif