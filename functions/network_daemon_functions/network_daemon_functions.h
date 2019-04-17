#ifndef NETWORK_DAEMON_FUNCTIONS_H_   /* Include guard */
#define NETWORK_DAEMON_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result, char* reserve_bytes_length, const int HTTP_SETTINGS);
int get_current_block_height(char *result, const int MESSAGE_SETTINGS);
int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS);
#endif