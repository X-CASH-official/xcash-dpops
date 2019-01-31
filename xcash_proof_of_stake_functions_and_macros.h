#ifndef XCASH_PROOF_OF_STAKE_FUNCTIONS_AND_MACROS_H_   /* Include guard */
#define XCASH_PROOF_OF_STAKE_FUNCTIONS_AND_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define SEND_DATA_PORT 8000 // The port that is used by all nodes to send and receive data
#define XCASH_WALLET_PREFIX "XCA" // The prefix of a XCA address
#define XCASH_WALLET_LENGTH 98 // The length of a XCA address
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a xcash_proof_of_stake_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a xcash_proof_of_stake_signature for the signed data
#define CONSENSUS_NODE_PUBLIC_ADDRESS "XCA" // The consensus nodes public address
#define CONSENSUS_BACKUP_NODE_PUBLIC_ADDRESS "XCA" // The consensus backup nodes public address
#define CONSENSUS_NODES_IP_ADDRESS "" // The consensus nodes IP address
#define CONSENSUS_BACKUP_NODES_IP_ADDRESS "" // The consensus backup nodes IP address
#define RANDOM_STRING_LENGTH 100 // The length of the random string
#define BUFFER_SIZE 64000
#define NODES_UPDATED_TIME_FILE_NAME "nodes_updated_time.txt" // The last time the node has updated the list of nodes from the consensus node
#define NODES_PUBLIC_ADDRESS_LIST_FILE_NAME "nodes_public_address_list.txt" // The current enabled nodes public address list
#define NODES_NAME_LIST_FILE_NAME "nodes_name_list.txt" // The current enabled nodes name list
#define MAXIMUM_CONNECTIONS 100 // The maximum connections a node can have at one time
#define SOCKET_TIMEOUT_SETTINGS 1 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT 5 // The total time given from when the consensus node sends to the node who the main node is, to the time the main node sends the data to the node.
#define SOCKET_END_STRING "|END|" // End string when sending data between nodes, to signal the end of sending data
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

Color available values:
  red = Error
  green = Success
  lightblue = Start of a new round
  blue = Start of a new part of a round
  purple = Voting information
  yellow = Alerts when you are one of the main nodes
  white = General information
-----------------------------------------------------------------------------------------------------------
*/

#define color_print(string,color) \
if (strncmp(color,"red",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;31m%s\033[0m\n",string); \
} \
else if (strncmp(color,"green",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;32m%s\033[0m\n",string); \
} \
else if (strncmp(color,"yellow",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;33m%s\033[0m\n",string); \
} \
else if (strncmp(color,"blue",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;34m%s\033[0m\n",string); \
} \
else if (strncmp(color,"purple",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;35m%s\033[0m\n",string); \
} \
else if (strncmp(color,"lightblue",BUFFER_SIZE) == 0) \
{ \
  printf("\033[1;36m%s\033[0m\n",string); \
} \
else \
{ \
  printf("%s",string); \
}


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
strncat(string1,string2,BUFFER_SIZE - strnlen(string1,BUFFER_SIZE) - 1);


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

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result);
int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int send_and_receive_data_socket(char *result, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS);
int send_data_socket(const char* HOST, const int PORT, const char* DATA, const char* TITLE, const int MESSAGE_SETTINGS);
int string_replace(char *data, const char* STR1, const char* STR2);
int send_data(const int SOCKET, char* data, const int APPEND_STRING_SETTINGS);
int receive_data(const int SOCKET, char *message, const char* STRING, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT);
int random_string(char *result, const size_t LENGTH);
int get_public_address(const int HTTP_SETTINGS);
int get_block_template(char *result, const int HTTP_SETTINGS);
int get_current_block_height(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS);
int sign_data(char *message, const int HTTP_SETTINGS);
int verify_data(const char* MESSAGE, const int HTTP_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS);
int read_file(char *result, const char* FILE_NAME);
int write_file(const char* data, const char* FILE_NAME);
int create_server(const int MESSAGE_SETTINGS);
// server functions
int get_current_consensus_nodes_IP_address();
int get_updated_node_list();
int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* message);
int server_receive_data_socket_consensus_node_to_node(const int CLIENT_SOCKET, pthread_t thread_id, char* message);
int create_server(const int MESSAGE_SETTINGS);
// thread functions
void* read_file_thread(void* parameters);
void* write_file_thread(void* parameters);
int thread_settings(pthread_t thread_id);
// server thread functions
void* total_connection_time_thread(void* parameters);
void* mainnode_timeout_thread(void* parameters);

/*
-----------------------------------------------------------------------------------------------------------
Global Structures
-----------------------------------------------------------------------------------------------------------
*/



 // Thread functions
 struct read_file_thread_parameters {
    char* result; // The data read from the file
    const char* FILE_NAME; // The file name
};

 struct write_file_thread_parameters {
    const char* DATA; // The data to write to the file
    const char* FILE_NAME; // The file name
};



 // Server thread functions
 struct total_connection_time_thread_parameters {
    pid_t process_id; // Holds the forked process ID that the client is connected to
    char* client_address; // Holds the IP address of the client
    char* port; // Holds the port number of the client 
    int data_received; // 1 if the server has received data from the client, otherwise 0
};

 struct mainnode_timeout_thread_parameters {
    const pid_t process_id; // Holds the forked process ID that the client is connected to
    int data_received; // 1 if the node has received data from the main node, otherwise 0
    const char* main_node; // The main node (VRF_PUBLIC_AND_PRIVATE_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    const char* current_round_part; // The current round part (1-4).
    const char* current_round_part_backup_node; // The current main node in the current round part (0-5)
};



/*
-----------------------------------------------------------------------------------------------------------
Global Variables
-----------------------------------------------------------------------------------------------------------
*/

char* xcash_wallet_public_address; // Holds your wallets public address
char* nodes_public_address_list; // The list of the enabled nodes public address (100 of them) (node1|node2|)
char* nodes_name_list; // The list of the enabled nodes public address (100 of them) (node1|node2|)
char* nodes_public_address_list_received_data; // The list of enabled nodes public address that have sent data, to prevent multiple votes (node1|node2|)
char* server_message; // The message the server should run. It will ignore any other messages.
char* current_consensus_nodes_IP_address; // The current consensus nodes IP address
char* main_nodes_public_address; // The current main nodes public address
char* current_round_part; // The current round part (1-4)
char* current_round_part_backup_node; // The current main node in the current round part (0-5)

#endif