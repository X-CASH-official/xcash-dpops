#ifndef STRUCTURES_H_   /* Include guard */
#define STRUCTURES_H_

#include "define_macros.h"

/*
-----------------------------------------------------------------------------------------------------------
Global Structures
-----------------------------------------------------------------------------------------------------------
*/

struct block_verifiers_list {
    char* block_verifiers_name[BLOCK_VERIFIERS_AMOUNT]; // The block verifiers name
    char* block_verifiers_public_address[BLOCK_VERIFIERS_AMOUNT]; // The block verifiers public address
    char* block_verifiers_IP_address[BLOCK_VERIFIERS_AMOUNT]; // The block verifiers IP address
};

struct current_round_part_consensus_node_data {
    char* vrf_public_key; // The VRF public key
    char* vrf_alpha_string; // The VRF alpha string
    char* vrf_proof; // The VRF proof
    char* vrf_beta_string; // The VRF beta string
    char* block_blob; // The block_blob
};

struct VRF_data_block_verifiers {
    char* public_address[BLOCK_VERIFIERS_AMOUNT]; // The public_address of the block verifier
    char* vrf_public_key_round_part_1[BLOCK_VERIFIERS_AMOUNT]; // The vrf_public_key_round_part_1 that the block verifier received
    char* vrf_alpha_string_round_part_1[BLOCK_VERIFIERS_AMOUNT]; // The vrf_alpha_string_round_part_1 that the block verifier received
    char* vrf_proof_round_part_1[BLOCK_VERIFIERS_AMOUNT]; // The vrf_proof_round_part_1 that the block verifier received
    char* vrf_beta_string_round_part_1[BLOCK_VERIFIERS_AMOUNT]; // The vrf_beta_string_round_part_1 that the block verifier received
    char* vrf_public_key_round_part_2[BLOCK_VERIFIERS_AMOUNT]; // The vrf_public_key_round_part_2 that the block verifier received
    char* vrf_alpha_string_round_part_2[BLOCK_VERIFIERS_AMOUNT]; // The vrf_alpha_string_round_part_2 that the block verifier received
    char* vrf_proof_round_part_2[BLOCK_VERIFIERS_AMOUNT]; // The vrf_proof_round_part_2 that the block verifier received
    char* vrf_beta_string_round_part_2[BLOCK_VERIFIERS_AMOUNT]; // The vrf_beta_string_round_part_2 that the block verifier received
    char* vrf_public_key_round_part_3[BLOCK_VERIFIERS_AMOUNT]; // The vrf_public_key_round_part_3 that the block verifier received
    char* vrf_alpha_string_round_part_3[BLOCK_VERIFIERS_AMOUNT]; // The vrf_alpha_string_round_part_3 that the block verifier received
    char* vrf_proof_round_part_3[BLOCK_VERIFIERS_AMOUNT]; // The vrf_proof_round_part_3 that the block verifier received
    char* vrf_beta_string_round_part_3[BLOCK_VERIFIERS_AMOUNT]; // The vrf_beta_string_round_part_3 that the block verifier received
};

struct blockchain_reserve_bytes {
    char* block_producer_delegates_name_data; // The block_producer_delegates_name
    char* block_producer_delegates_name; // The block_producer_delegates_name_data text
    size_t block_producer_delegates_name_data_length; // The amount of characters of the block_producer_delegates_name_data
    char* block_producer_public_address_data; // The block_producer_public_address
    char* block_producer_public_address; // The block_producer_public_address_data text
    size_t block_producer_public_address_data_length; // The amount of characters of the block_producer_public_address_data
    char* block_producer_node_backup_count_data; // The block_producer_node_backup_count
    char* block_producer_node_backup_count; // The block_producer_node_backup_count_data text
    size_t block_producer_node_backup_count_data_length; // The amount of characters of the block_producer_node_backup_count_data
    char* block_producer_backup_nodes_names_data; // The block_producer_backup_nodes_names
    char* block_producer_backup_nodes_names; // The block_producer_backup_nodes_names_data text
    size_t block_producer_backup_nodes_names_data_length; // The amount of characters of the block_producer_backup_nodes_names_data
    char* vrf_node_public_and_secret_key_delegates_name_data; // The vrf_node_public_and_secret_key_delegates_name
    char* vrf_node_public_and_secret_key_delegates_name; // The vrf_node_public_and_secret_key_delegates_name_data text
    size_t vrf_node_public_and_secret_key_delegates_name_data_length; // The amount of characters of the vrf_node_public_and_secret_key_delegates_name_data
    char* vrf_node_public_and_secret_key_public_address_data; // The vrf_node_public_and_secret_key_public_address
    char* vrf_node_public_and_secret_key_public_address; // The vrf_node_public_and_secret_key_public_address_data text
    size_t vrf_node_public_and_secret_key_public_address_data_length; // The amount of characters of the vrf_node_public_and_secret_key_public_address_data
    char* vrf_node_public_and_secret_key_node_backup_count_data; // The vrf_node_public_and_secret_key_node_backup_count
    char* vrf_node_public_and_secret_key_node_backup_count; // The vrf_node_public_and_secret_key_node_backup_count_data text
    size_t vrf_node_public_and_secret_key_node_backup_count_data_length; // The amount of characters of the vrf_node_public_and_secret_key_node_backup_count_data
    char* vrf_node_public_and_secret_key_backup_nodes_names_data; // The vrf_node_public_and_secret_key_backup_nodes_names
    char* vrf_node_public_and_secret_key_backup_nodes_names; // The vrf_node_public_and_secret_key_backup_nodes_names_data text
    size_t vrf_node_public_and_secret_key_backup_nodes_names_data_length; // The amount of characters of the vrf_node_public_and_secret_key_backup_nodes_names_data
    char* vrf_node_random_data_delegates_name_data; // The vrf_node_random_data_delegates_name
    char* vrf_node_random_data_delegates_name; // The vrf_node_random_data_delegates_name_data text
    size_t vrf_node_random_data_delegates_name_data_length; // The amount of characters of the vrf_node_random_data_delegates_name_data
    char* vrf_node_random_data_public_address_data; // The vrf_node_random_data_public_address
    char* vrf_node_random_data_public_address; // The vrf_node_random_data_public_address_data text
    size_t vrf_node_random_data_public_address_data_length; // The amount of characters of the vrf_node_random_data_public_address_data
    char* vrf_node_random_data_node_backup_count_data; // The vrf_node_random_data_node_backup_count
    char* vrf_node_random_data_node_backup_count; // The vrf_node_random_data_node_backup_count_data text
    size_t vrf_node_random_data_node_backup_count_data_length; // The amount of characters of the vrf_node_random_data_node_backup_count_data
    char* vrf_node_random_data_backup_nodes_names_data; // The vrf_node_random_data_backup_nodes_names
    char* vrf_node_random_data_backup_nodes_names; // The vrf_node_random_data_backup_nodes_names_data text
    size_t vrf_node_random_data_backup_nodes_names_data_length; // The amount of characters of the vrf_node_random_data_backup_nodes_names_data
    char* vrf_public_key_round_part_1; // The VRF public key for round part 1
    size_t vrf_public_key_length_round_part_1; // The length of the VRF public key for round part 1
    char* vrf_alpha_string_round_part_1; // The VRF alpha string (input string) for round part 1
    size_t vrf_alpha_string_length_round_part_1; // The length of the VRF alpha string (input string) for round part 1
    char* vrf_proof_round_part_1; // The VRF proof for round part 1
    size_t vrf_proof_length_round_part_1; // The length of the VRF proof for round part 1
    char* vrf_beta_string_round_part_1; // The VRF beta string (output string) for round part 1
    size_t vrf_beta_string_length_round_part_1; // The length of the VRF beta string (output string) for round part 1
    char* vrf_data_round_part_1; // The VRF data for round part 1
    char* vrf_public_key_round_part_2; // The VRF public key for round part 1
    size_t vrf_public_key_length_round_part_2;// The length of the VRF public key for round part 1
    char* vrf_alpha_string_round_part_2; // The VRF alpha string (input string) for round part 2
    size_t vrf_alpha_string_length_round_part_2; // The length of the VRF alpha string (input string) for round part 2
    char* vrf_proof_round_part_2; // The VRF proof for round part 2
    size_t vrf_proof_length_round_part_2; // The length of the VRF proof for round part 2
    char* vrf_beta_string_round_part_2; // The VRF beta string (output string) for round part 2
    size_t vrf_beta_string_length_round_part_2; // The length of the VRF beta string (output string) for round part 2
    char* vrf_data_round_part_2; // The VRF data for round part 2
    char* vrf_public_key_round_part_3; // The VRF public key for round part 3
    size_t vrf_public_key_length_round_part_3;// The length of the VRF public key for round part 3
    char* vrf_alpha_string_round_part_3; // The VRF alpha string (input string) for round part 3
    size_t vrf_alpha_string_length_round_part_3; // The length of the VRF alpha string (input string) for round part 3
    char* vrf_proof_round_part_3; // The VRF proof for round part 3
    size_t vrf_proof_length_round_part_3; // The length of the VRF proof for round part 3
    char* vrf_beta_string_round_part_3; // The VRF beta string (output string) for round part 3
    size_t vrf_beta_string_length_round_part_3; // The length of the VRF beta string (output string) for round part 3
    char* vrf_data_round_part_3; // The VRF data for round part 3
    char* vrf_data; // The VRF data for all of the round parts
    size_t vrf_data_length; // The length of the VRF data for all of the round parts
    char* previous_block_hash_data; // The previous_block_hash
    size_t previous_block_hash_data_length; // The amount of characters of the previous_block_hash_data
    char* block_validation_node_signature_data[BLOCK_VALIDATION_NODES_AMOUNT]; // The block_validation_node_signature
    char* block_validation_node_signature[BLOCK_VALIDATION_NODES_AMOUNT]; // The block_validation_node_signature_data text
    size_t block_validation_node_signature_data_length; // The amount of characters of the block_validation_node_signature_data
};

struct blockchain_data {
    char* network_version_data; // The network_version
    size_t network_version_data_length; // The amount of characters of the network_version_data
    char* timestamp_data; // The timestamp
    size_t timestamp; // Variant decoded timestamp
    size_t timestamp_data_length; // The amount of characters of the timestamp_data
    char* previous_block_hash_data; // The previous_block_hash
    size_t previous_block_hash_data_length; // The amount of characters of the previous_block_hash_data
    char* nonce_data; // The nonce
    size_t nonce_data_length; // The amount of characters of the nonce_data
    char* block_reward_transaction_version_data; // The block_reward_transaction_version
    size_t block_reward_transaction_version_data_length; // The amount of characters of the block_reward_transaction_version_data
    char* unlock_block_data; // The unlock_block
    size_t unlock_block; // Variant decoded unlock_block
    size_t unlock_block_data_length; // The amount of characters of the unlock_block_data
    char* block_reward_input_data; // The block_reward_input
    size_t block_reward_input_data_length; // The amount of characters of the block_reward_input_data
    char* vin_type_data; // The vin_type
    size_t vin_type_data_length; // The amount of characters of the vin_type_data
    char* block_height_data; // The block_height
    size_t block_height; // Variant decoded block_height
    size_t block_height_data_length; // The amount of characters of the block_height_data
    char* block_reward_output_data; // The block_reward_output
    size_t block_reward_output_data_length; // The amount of characters of the block_reward_output_data
    char* block_reward_data; // The block_reward
    size_t block_reward; // Variant decoded block_reward
    size_t block_reward_data_length; // The amount of characters of the block_reward_data
    char* stealth_address_output_tag_data; // The stealth_address_output_tag
    size_t stealth_address_output_tag_data_length; // The amount of characters of the stealth_address_output_tag_data
    char* stealth_address_output_data; // The stealth_address_output
    size_t stealth_address_output_data_length; // The amount of characters of the stealth_address_output_data
    char* extra_bytes_size_data; // The extra_bytes_size
    size_t extra_bytes_size; // Variant decoded extra_bytes_size
    size_t extra_bytes_size_data_length; // The amount of characters of the extra_bytes_size
    char* transaction_public_key_tag_data; // The transaction_public_key_tag
    size_t transaction_public_key_tag_data_length; // The amount of characters of the transaction_public_key_tag
    char* transaction_public_key_data; // The transaction_public_key
    size_t transaction_public_key_data_length; // The amount of characters of the transaction_public_key
    char* extra_nonce_tag_data; // The extra_nonce_tag
    size_t extra_nonce_tag_data_length; // The amount of characters of the extra_nonce_tag
    char* reserve_bytes_size_data; // The reserve_bytes_size
    size_t reserve_bytes_size; // Variant decoded reserve_bytes_size
    size_t reserve_bytes_size_data_length; // The amount of characters of the reserve_bytes_size
    struct blockchain_reserve_bytes blockchain_reserve_bytes; // A blockchain_reserve_bytes struct that holds all of the reserve bytes
    char* ringct_version_data; // The ringct_version
    size_t ringct_version_data_length; // The amount of characters of the ringct_version
    char* transaction_amount_data; // The transaction_amount
    size_t transaction_amount; // Variant decoded transaction_amount
    size_t transaction_amount_data_length; // The amount of characters of the transaction_amount
    char* transactions[1000000]; // All of the transactions in the block.
};


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
    pid_t process_id; // Holds the forked process ID that the client is connected to
    int data_received; // 1 if the node has received data from the main node, otherwise 0
    char* main_node; // The main node (VRF_PUBLIC_AND_PRIVATE_KEY, VRF_RANDOM_DATA, BLOCK_PRODUCER)
    char* current_round_part; // The current round part (1-4).
    char* current_round_part_backup_node; // The current main node in the current round part (0-5)
};

struct node_to_node_timeout_thread_parameters {
    pid_t process_id; // Holds the forked process ID that the client is connected to
};
#endif