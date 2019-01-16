#ifndef XCASH_PROOF_OF_STAKE_FUNCTIONS_AND_MACROS_H_   /* Include guard */
#define XCASH_PROOF_OF_STAKE_FUNCTIONS_AND_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define XCASH_WALLET_PREFIX "XCA" // The prefix of a XCA address
#define XCASH_WALLET_LENGTH 98 // The length of a XCA address
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a xcash_proof_of_stake_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a xcash_proof_of_stake_signature for the signed data
#define RANDOM_STRING_LENGTH 100 // The length of the random string
#define SEND_DATA_PORT 8000 // The port that is used by all nodes to send and receive data
#define CLIENT_AND_SERVER_DATA_PORT 18288 // The port that is used to send data locally between a nodes client and server program
#define BUFFER_SIZE 64000
#define CLADDR_LEN 100
#define MAXIMUM_CONNECTIONS 100 // The maximum connections a node can have at one time
#define SOCKET_TIMEOUT_SETTINGS 1 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define SOCKET_END_STRING "|END|" // End string when sending data between clients, or client to server on the same computer, to signal the end of sending data
#define INVALID_PARAMETERS_ERROR_MESSAGE \
"Parameters\n" \
"--test - Run the test to make sure the program is compatible with your system\n"

/*
-----------------------------------------------------------------------------------------------------------
Using define macros instead of functions for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: color_print
Description: Prints a string in a color
Parameters:
  string - char*
  color - char*
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define color_print(string,color) \
color == "red" ? printf("\033[1;31m%s\033[0m\n",string) : \
color == "green" ? printf("\033[1;32m%s\033[0m\n",string) : \
color == "yellow" ? printf("\033[1;33m%s\033[0m\n",string) : \
color == "blue" ? printf("\033[1;34m%s\033[0m\n",string) : \
color == "purple" ? printf("\033[1;35m%s\033[0m\n",string) : \
color == "lightblue" ? printf("\033[1;36m%s\033[0m\n",string) : \
printf("%s",string);


/*
-----------------------------------------------------------------------------------------------------------
Name: append_string
Description: Appends strings securely to stop buffer overflows, and to always null terminate the string 
Parameters:
  string1 - char*
  string2 - char*
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define append_string(string1,string2) \
strncat(string1,string2,BUFFER_SIZE - strlen(string1) - 1);


/*
-----------------------------------------------------------------------------------------------------------
Name: pointer_reset
Description: Reset the memory used by the pointer, and sets the pointer to NULL to avoid a dangling pointer
Parameters:
  pointer - Any pointer type
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define pointer_reset(pointer) \
free(pointer); \
pointer = NULL;


/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(char* data, char* field, char *result);
int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int send_data_socket(char *result, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int string_replace(char *data, const char* STR1, const char* STR2);
int send_data(const int SOCKET, char* data, const int APPEND_STRING_SETTINGS);
int receive_data(const int SOCKET, char *message, const char* STRING, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT);
int random_string(char *result, const size_t LENGTH);
int get_public_address(char *result, const int MESSAGE_SETTINGS);
int get_current_block_height(char *result, const int MESSAGE_SETTINGS);
int sign_data(char *message, const int MESSAGE_SETTINGS, const char* CURRENT_ROUND_PART, const char* CURRENT_ROUND_PART_BACKUP_NODE);
int verify_data(char* message, const int HTTP_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_SETTINGS, const char* CURRENT_ROUND_PART, const int VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS, const char* CURRENT_ROUND_PART_BACKUP_NODE);
int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* message);
int create_server(const int MESSAGE_SETTINGS);
// threads
void* total_connection_time_thread(void* parameters);

/*
-----------------------------------------------------------------------------------------------------------
Global structures
-----------------------------------------------------------------------------------------------------------
*/


 struct total_connection_time_thread_parameters {
    pid_t process_id; // Holds the forked process ID that the client is connected to
    char* client_address; // Holds the IP address of the client
    char* port; // Holds the port number of the client 
    int data_received; // 1 if the server has received data from the client, otherwise 0
};

#endif