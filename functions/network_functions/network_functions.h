#ifndef NETWORK_FUNCTIONS_H_   /* Include guard */
#define NETWORK_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int send_and_receive_data_socket(char *result, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int send_data_socket(const char* HOST, const int PORT, const char* DATA, const char* TITLE, const int MESSAGE_SETTINGS);
int send_data(const int SOCKET, char* data, const int APPEND_STRING_SETTINGS);
int receive_data(const int SOCKET, char *message, const char* STRING, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT);
int sync_all_block_verifiers_list();
int get_synced_block_verifiers();
int sync_check_reserve_proofs_database();
int sync_reserve_proofs_database();
int sync_check_reserve_bytes_database(const char* BLOCK_HEIGHT);
int sync_reserve_bytes_database(const char* BLOCK_HEIGHT);
int sync_check_delegates_database();
int sync_delegates_database();
int sync_check_statistics_database();
int sync_statistics_database();
int get_delegate_online_status(const char* HOST);
#endif