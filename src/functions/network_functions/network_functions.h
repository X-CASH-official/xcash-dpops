#ifndef NETWORK_FUNCTIONS_H_   /* Include guard */
#define NETWORK_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int check_if_IP_address_or_hostname(const char* HOST);
int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS);
int send_and_receive_data_socket(char *result, const size_t RESULT_LENGTH, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS);
int send_data_socket(const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS);
int send_data(const int SOCKET, unsigned char* data, const long DATA_LENGTH, const int MESSAGE_SETTINGS, const char* MESSAGE_DATA_SETTINGS);
int receive_data(const int SOCKET, char *message, const size_t LENGTH, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT);
#endif