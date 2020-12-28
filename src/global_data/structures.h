#ifndef STRUCTURES_H_   /* Include guard */
#define STRUCTURES_H_

#include "define_macros.h"
#include "convert.h"
#include "vrf.h"
#include "crypto_vrf.h"

/*
-----------------------------------------------------------------------------------------------------------
Global Structures
-----------------------------------------------------------------------------------------------------------
*/

struct previous_block_verifiers_list {
    char block_verifiers_name[BLOCK_VERIFIERS_TOTAL_AMOUNT][MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1]; // The block verifiers name
    char block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][XCASH_WALLET_LENGTH+1]; // The block verifiers public address
    char block_verifiers_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT][VRF_PUBLIC_KEY_LENGTH+1]; // The block verifiers public key
    char block_verifiers_IP_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block verifiers IP address
};

struct current_block_verifiers_list {
    char block_verifiers_name[BLOCK_VERIFIERS_TOTAL_AMOUNT][MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1]; // The block verifiers name
    char block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][XCASH_WALLET_LENGTH+1]; // The block verifiers public address
    char block_verifiers_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT][VRF_PUBLIC_KEY_LENGTH+1]; // The block verifiers public key
    char block_verifiers_IP_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block verifiers IP address
};

struct next_block_verifiers_list {
    char block_verifiers_name[BLOCK_VERIFIERS_TOTAL_AMOUNT][MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1]; // The block verifiers name
    char block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][XCASH_WALLET_LENGTH+1]; // The block verifiers public address
    char block_verifiers_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT][VRF_PUBLIC_KEY_LENGTH+1]; // The block verifiers public key
    char block_verifiers_IP_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block verifiers IP address
};

struct synced_block_verifiers {
    char synced_block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][XCASH_WALLET_LENGTH+1]; // The block verifiers public address
    char synced_block_verifiers_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT][VRF_PUBLIC_KEY_LENGTH+1]; // The block verifiers public key
    char synced_block_verifiers_IP_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block verifiers IP address
    char vote_settings[BLOCK_VERIFIERS_TOTAL_AMOUNT][20]; // The block verifiers vote, either true, false or connection_timeout
    int vote_settings_true; // The amount of block verifiers that voted true    
    int vote_settings_false; // The amount of block verifiers that voted false
    int vote_settings_connection_timeout; // The amount of block verifiers that had a connection timeout
    time_t last_refresh_time_of_synced_block_verifiers; // the last refresh time of the synced block verifiers
};

struct main_nodes_list {
    char block_producer_public_address[XCASH_WALLET_LENGTH+1]; // The block producers public address
    char block_producer_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers IP address
    char block_producer_backup_block_verifier_1_public_address[XCASH_WALLET_LENGTH+1]; // The block producers backup node 1 public address
    char block_producer_backup_block_verifier_1_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers backup node 1 IP address
    char block_producer_backup_block_verifier_2_public_address[XCASH_WALLET_LENGTH+1]; // The block producers backup node 2 public address
    char block_producer_backup_block_verifier_2_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers backup node 2 IP address
    char block_producer_backup_block_verifier_3_public_address[XCASH_WALLET_LENGTH+1]; // The block producers backup node 3 public address
    char block_producer_backup_block_verifier_3_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers backup node 3 IP address
    char block_producer_backup_block_verifier_4_public_address[XCASH_WALLET_LENGTH+1]; // The block producers backup node 4 public address
    char block_producer_backup_block_verifier_4_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers backup node 4 IP address
    char block_producer_backup_block_verifier_5_public_address[XCASH_WALLET_LENGTH+1]; // The block producers backup node 5 public address
    char block_producer_backup_block_verifier_5_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block producers backup node 5 IP address    
};

struct network_data_nodes_list {
    char network_data_nodes_public_address[NETWORK_DATA_NODES_AMOUNT][XCASH_WALLET_LENGTH+1]; // The block verifiers public address
    char network_data_nodes_IP_address[NETWORK_DATA_NODES_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1]; // The block verifiers IP address
    int online_status[NETWORK_DATA_NODES_AMOUNT]; // 1 for online, 0 for offline
};

struct current_round_part_vote_data {
    char current_vote_results[DATA_HASH_LENGTH+1]; // The SHA2-512 hash of the vote results that the block verifier calculated
    int vote_results_valid; // The current part of the round vote count for a valid vote.
    int vote_results_invalid; // The current part of the round vote count for a invalid vote.
};

struct current_block_verifiers_majority_vote {
    char data[BLOCK_VERIFIERS_AMOUNT][BLOCK_VERIFIERS_AMOUNT][500]; // The data for each received data from each block verifier
};


struct network_data_nodes_sync_database_list {
    char network_data_node_public_address[NETWORK_DATA_NODES_AMOUNT][XCASH_WALLET_LENGTH+1];
    char network_data_nodes_IP_address[NETWORK_DATA_NODES_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1];
    char network_data_nodes_database_data_hash[NETWORK_DATA_NODES_AMOUNT][DATA_HASH_LENGTH+1];
    int network_data_nodes_previous_block_settings[NETWORK_DATA_NODES_AMOUNT];
};

struct block_verifiers_sync_database_list {
    char block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][XCASH_WALLET_LENGTH+1];
    char block_verifiers_IP_address[BLOCK_VERIFIERS_TOTAL_AMOUNT][BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1];
    char block_verifiers_database_data_hash[DATABASE_TOTAL][BLOCK_VERIFIERS_TOTAL_AMOUNT][DATA_HASH_LENGTH+1];
    int block_verifiers_previous_block_settings[BLOCK_VERIFIERS_TOTAL_AMOUNT];
};

struct VRF_data { 
    char* vrf_secret_key_data; // The VRF secret key text for round part 4
    unsigned char* vrf_secret_key; // The VRF secret key for round part 4
    char* vrf_public_key_data; // The VRF public key text for round part 4
    unsigned char* vrf_public_key; // The VRF public key for round part 4
    char* vrf_alpha_string_data; // The VRF alpha string (input string) text for round part 4
    unsigned char* vrf_alpha_string; // The VRF alpha string (input string) for round part 4
    char* vrf_proof_data; // The VRF proof text for round part 4
    unsigned char* vrf_proof; // The VRF proof for round part 4
    char* vrf_beta_string_data; // The VRF beta string (output string) text for round part 4
    unsigned char* vrf_beta_string; // The VRF beta string (output string) for round part 4
    char* block_verifiers_vrf_secret_key_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF secret key text that all of the block verifiers create
    unsigned char* block_verifiers_vrf_secret_key[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF secret key that all of the block verifiers create
    char* block_verifiers_vrf_public_key_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF public key text that all of the block verifiers create
    unsigned char* block_verifiers_vrf_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF public key that all of the block verifiers create
    char* block_verifiers_random_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The random data that all of the block verifiers create
    char* block_blob_signature[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The signed block data text
    char* block_blob; // The block_blob that the block producer creates to send to the consensus node
    char* reserve_bytes_data_hash; // The reserve bytes data hash
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
    char* vrf_secret_key_data; // The VRF secret key text for round part 3
    unsigned char* vrf_secret_key; // The VRF secret key for round part 3
    size_t vrf_secret_key_data_length; // The length of the VRF public key for round part 3   
    char* vrf_public_key_data; // The VRF public key text for round part 3
    unsigned char* vrf_public_key; // The VRF public key for round part 3
    size_t vrf_public_key_data_length; // The length of the VRF public key for round part 3
    char* vrf_alpha_string_data; // The VRF alpha string (input string) text for round part 3
    unsigned char* vrf_alpha_string; // The VRF alpha string (input string) for round part 3
    size_t vrf_alpha_string_data_length; // The length of the VRF alpha string (input string) for round part 3
    char* vrf_proof_data; // The VRF proof text for round part 3
    unsigned char* vrf_proof; // The VRF proof for round part 3
    size_t vrf_proof_data_length; // The length of the VRF proof for round part 3
    char* vrf_beta_string_data; // The VRF beta string (output string) text for round part 3
    unsigned char* vrf_beta_string; // The VRF beta string (output string) for round part 3
    size_t vrf_beta_string_data_length; // The length of the VRF beta string (output string) for round part 3
    char* vrf_data_round; // The VRF data for round part 3  
    char* vrf_data; // The VRF data for all of the round parts
    size_t vrf_data_length; // The length of the VRF data for all of the round parts
    char* block_verifiers_vrf_secret_key_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF secret key text that all of the block verifiers create
    unsigned char* block_verifiers_vrf_secret_key[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF secret key that all of the block verifiers create
    size_t block_verifiers_vrf_secret_key_data_length; // The amount of characters of the block_verifiers_vrf_secret_key_data
    char* block_verifiers_vrf_public_key_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF public key text that all of the block verifiers create
    unsigned char* block_verifiers_vrf_public_key[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The VRF public key that all of the block verifiers create
    size_t block_verifiers_vrf_public_key_data_length; // The amount of characters of the block_verifiers_vrf_public_key_data
    char* block_verifiers_random_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The random data that all of the block verifiers create
    char* block_verifiers_random_data_text[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The random data text that all of the block verifiers create
    size_t block_verifiers_random_data_length; // The amount of characters of the block_verifiers_random_data
    char* next_block_verifiers_public_address_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The next_block_verifiers_public_address
    char* next_block_verifiers_public_address[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The next_block_verifiers_public_address_data text
    size_t next_block_verifiers_public_address_data_length; // The amount of characters of the next_block_verifiers_public_address_data
    char* previous_block_hash_data; // The previous_block_hash
    size_t previous_block_hash_data_length; // The amount of characters of the previous_block_hash_data
    char* block_validation_node_signature_data[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The block_validation_node_signature
    char* block_validation_node_signature[BLOCK_VERIFIERS_TOTAL_AMOUNT]; // The block_validation_node_signature_data text
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
    char* transactions[MAXIMUM_TRANSACATIONS_PER_BLOCK]; // All of the transactions in the block.
};

struct error_message {
    char* function[TOTAL_ERROR_MESSAGES]; // The error function
    char* data[TOTAL_ERROR_MESSAGES]; // The error message
    int total; // the amount of error messages
};

struct invalid_reserve_proofs {  
    char* block_verifier_public_address[MAXIMUM_INVALID_RESERVE_PROOFS]; // The block verifiers public address that added the invalid reserve proof
    char* public_address_created_reserve_proof[MAXIMUM_INVALID_RESERVE_PROOFS]; // The public address that created the reserve proof
    char* public_address_voted_for[MAXIMUM_INVALID_RESERVE_PROOFS]; // The public address that was voted for
    size_t reserve_proof_amount[MAXIMUM_INVALID_RESERVE_PROOFS]; // The reserve proof amount
    char* reserve_proof[MAXIMUM_INVALID_RESERVE_PROOFS]; // The reserve proof
    int count; // The amount of reserve proofs in the invalid_reserve_proofs struct
};

struct reserve_proof {  
    char block_verifier_public_address[XCASH_WALLET_LENGTH+1]; // The block verifiers public address that added the invalid reserve proof
    char public_address_created_reserve_proof[XCASH_WALLET_LENGTH+1]; // The public address that created the reserve proof
    char public_address_voted_for[XCASH_WALLET_LENGTH+1]; // The public address that was voted for
    char reserve_proof_amount[100]; // The reserve proof amount
    char reserve_proof[BUFFER_SIZE_RESERVE_PROOF+1]; // The reserve proof
};

// database struct
struct database_document_fields {
    size_t count; // The amount of items in the database_document_fields struct
    char* item[TOTAL_DELEGATES_DATABASE_FIELDS+1]; // The item
    char* value[TOTAL_DELEGATES_DATABASE_FIELDS+1]; // The value
};

struct database_multiple_documents_fields {
    size_t document_count; // The amount of documents in the database_multiple_documents_fields
    size_t database_fields_count; // The amount of items in each document
    char* item[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE][TOTAL_DELEGATES_DATABASE_FIELDS+1]; // The item
    char* value[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE][TOTAL_DELEGATES_DATABASE_FIELDS+1]; // The value
};

struct block_verifiers_send_data_socket {
    char IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH+1];
    int socket;
    int settings;
};

struct delegates {
    char* public_address;
    char* total_vote_count;
    char* IP_address;
    char* delegate_name;
    char* about;
    char* website;
    char* team;
    char* shared_delegate_status;
    char* delegate_fee;
    char* server_specs;
    char* block_verifier_score;
    char* online_status;
    char* block_verifier_total_rounds;
    char* block_verifier_online_total_rounds;
    char* block_verifier_online_percentage;
    char* block_producer_total_rounds;
    char* block_producer_block_heights;
    char* public_key;
};

struct voters {
    char* public_address;
    char* total_vote_count;
    long long int total_votes;
};

struct votes {
    char* public_address_created_reserve_proof;
    char* public_address_voted_for;
    char* total;
    char* reserve_proof;
};

struct delegates_online_status {
    char public_address[XCASH_WALLET_LENGTH+1];
    int settings;
};

struct block_height_start_time {
    int block_height_start_time; // 1 to start the current_block_height_timer_thread at a specific time, 0 if not
    int block_height_start_time_year; // The year to start the current_block_height_timer_thread
    int block_height_start_time_month; // The month to start the current_block_height_timer_thread
    int block_height_start_time_day; // The day to start the current_block_height_timer_thread
    int block_height_start_time_hour; // The hour to start the current_block_height_timer_thread
    int block_height_start_time_minute; // The minute to start the current_block_height_timer_thread
};

struct private_group {
    char private_group_file[256]; // The full name and path to the file location
    int private_group_settings; // 0 for off, 1 for on
    char private_group_name[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP][MAXIMUM_BUFFER_SIZE_DELEGATES_NAME+1]; // The name for the entry in the private group
    char private_group_voting_public_address[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP][XCASH_WALLET_LENGTH+1]; // The voting public address
    char private_group_payment_public_address[MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE_PRIVATE_GROUP][XCASH_WALLET_LENGTH+1]; // the payment public address
};

 // Thread functions
 struct socket_thread_parameters {
    int client_socket;  // The client socket
    char client_address[BUFFER_SIZE];  // The client address
};

 struct read_file_thread_parameters {
    unsigned char* result; // The data read from the file
    const char* FILE_NAME; // The file name
};

 struct write_file_thread_parameters {
    const char* DATA; // The data to write to the file
    const char* FILE_NAME; // The file name
};

struct append_file_thread_parameters {
    const char* DATA; // The data to append to the file
    const char* FILE_NAME; // The file name
};

struct insert_document_into_collection_json_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to insert into the collection
};

struct insert_multiple_documents_into_collection_json_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to insert into the collection
    const size_t DATA_TOTAL_LENGTH; // The total length of the data
};

struct read_document_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to search the collection for
    char *result; // The document read from the collection
};

struct read_document_field_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to search the collection for
    const char* FIELD_NAME; // The field of the document data to read
    char *result; // The document data read from the collection
};

struct read_document_all_fields_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to search the collection for
    struct database_document_fields* result; // A pointer to the database_document_fields struct
};

struct read_multiple_documents_all_fields_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name,
    const char* DATA; // The data to search for in each document, set to "" for all items
    struct database_multiple_documents_fields* result; // A pointer to the database_multiple_documents_fields struct
    const size_t DOCUMENT_COUNT_START; // The document to start at when reading the data
    const size_t DOCUMENT_COUNT_TOTAL; // The total amount of documents to read
    const int DOCUMENT_OPTIONS; // 1 to use the sort document option, 0 to not use the document option
    const char* DOCUMENT_OPTIONS_DATA; // The item to sort the documents in the collection
};

struct update_document_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to search the collection for
    const char* FIELD_NAME_AND_DATA; // The json data to use to update the document
};

struct update_all_documents_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to update the documents
};

struct delete_document_from_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to delete the document
};

struct delete_collection_from_database_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
};

struct delete_database_thread_parameters {
    const char* DATABASE; // The database name
};

struct count_documents_in_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
    const char* DATA; // The json data to use to search the collection for
};

struct count_all_documents_in_collection_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
};

struct get_database_data_thread_parameters {
    char *database_data; // The database data
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
};

struct get_database_data_hash_thread_parameters {
    char *data_hash; // The data hash
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
};

struct update_delegates_online_status_thread_parameters {
    const char* DATABASE; // The database name
    const char* COLLECTION; // The collection name
};
#endif