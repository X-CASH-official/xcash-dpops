#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

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