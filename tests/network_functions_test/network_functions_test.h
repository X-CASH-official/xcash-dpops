#ifndef NETWORK_FUNCTIONS_TEST_H_   /* Include guard */
#define NETWORK_FUNCTIONS_TEST_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

void network_daemon_test(void);
void* create_server_on_separate_thread();
void general_network_test(void);
void network_security_test(void);
void network_wallet_test(void);
int network_functions_test(void);
#endif