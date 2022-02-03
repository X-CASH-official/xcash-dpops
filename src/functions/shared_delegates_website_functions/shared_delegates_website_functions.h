#ifndef SHARED_DELEGATES_WEBSITE_FUNCTIONS_H_   /* Include guard */
#define SHARED_DELEGATES_WEBSITE_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_for_valid_start_and_amount_parameters(const char* START, const char* AMOUNT);
int server_receive_data_socket_shared_delegates_website_get_statistics(const int CLIENT_SOCKET);
int server_receive_data_socket_get_blocks_found(const int CLIENT_SOCKET,const char* DATA);
int server_receive_data_socket_get_public_address_information(const int CLIENT_SOCKET, const char* DATA);
int server_receive_data_socket_get_public_address_payment_information(const int CLIENT_SOCKET, const char* DATA);
#endif
