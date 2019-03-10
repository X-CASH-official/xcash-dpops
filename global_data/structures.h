#ifndef STRUCTURES_H_   /* Include guard */
#define STRUCTURES_H_

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
    int message_settings; // 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
};

 struct mainnode_timeout_thread_parameters {
    const pid_t process_id; // Holds the forked process ID that the client is connected to
    int data_received; // 1 if the node has received data from the main node, otherwise 0
    const char* main_node; // The main node (VRF_PUBLIC_AND_PRIVATE_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    const char* current_round_part; // The current round part (1-4).
    const char* current_round_part_backup_node; // The current main node in the current round part (0-5)
};
#endif