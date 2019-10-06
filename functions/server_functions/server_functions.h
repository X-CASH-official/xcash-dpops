#ifndef SERVER_FUNCTIONS_H_   /* Include guard */
#define SERVER_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int create_server(const int MESSAGE_SETTINGS);
int new_socket_thread(void);
int socket_thread(int client_socket);
#endif