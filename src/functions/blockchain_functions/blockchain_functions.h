#ifndef BLOCKCHAIN_FUNCTIONS_H_   /* Include guard */
#define BLOCKCHAIN_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int varint_encode(long long int number, char *result, const size_t RESULT_TOTAL_LENGTH);
size_t varint_decode(size_t varint);
double get_generated_supply(const size_t BLOCK_HEIGHT);
int network_block_string_to_blockchain_data(const char* DATA, const char* BLOCK_HEIGHT, const int BLOCK_VERIFIERS_TOTAL);
int blockchain_data_to_network_block_string(char *result, const int BLOCK_VERIFIERS_TOTAL);
int add_data_hash_to_network_block_string(char* network_block_string, char *network_block_string_data_hash);
void* verify_network_block_data_vrf_data_verify_thread(void* parameters);
int verify_network_block_data(const int BLOCK_VALIDATION_SIGNATURES_SETTINGS, const int PREVIOUS_BLOCK_HASH_SETTINGS, const int TRANSACTIONS_SETTINGS, const char* BLOCK_HEIGHT, char* PREVIOUS_NETWORK_BLOCK_RESERVE_BYTES, const int BLOCK_VERIFIERS_TOTAL);

#define RESET_BLOCKCHAIN_DATA \
memset(blockchain_data.network_version_data,0,strlen(blockchain_data.network_version_data)); \
memset(blockchain_data.timestamp_data,0,strlen(blockchain_data.timestamp_data)); \
memset(blockchain_data.previous_block_hash_data,0,strlen(blockchain_data.previous_block_hash_data)); \
memset(blockchain_data.nonce_data,0,strlen(blockchain_data.nonce_data)); \
memset(blockchain_data.block_reward_transaction_version_data,0,strlen(blockchain_data.block_reward_transaction_version_data)); \
memset(blockchain_data.unlock_block_data,0,strlen(blockchain_data.unlock_block_data)); \
memset(blockchain_data.block_reward_input_data,0,strlen(blockchain_data.block_reward_input_data)); \
memset(blockchain_data.vin_type_data,0,strlen(blockchain_data.vin_type_data)); \
memset(blockchain_data.block_height_data,0,strlen(blockchain_data.block_height_data)); \
memset(blockchain_data.block_reward_output_data,0,strlen(blockchain_data.block_reward_output_data)); \
memset(blockchain_data.block_reward_data,0,strlen(blockchain_data.block_reward_data)); \
memset(blockchain_data.stealth_address_output_tag_data,0,strlen(blockchain_data.stealth_address_output_tag_data)); \
memset(blockchain_data.stealth_address_output_data,0,strlen(blockchain_data.stealth_address_output_data)); \
memset(blockchain_data.extra_bytes_size_data,0,strlen(blockchain_data.extra_bytes_size_data)); \
memset(blockchain_data.transaction_public_key_tag_data,0,strlen(blockchain_data.transaction_public_key_tag_data)); \
memset(blockchain_data.transaction_public_key_data,0,strlen(blockchain_data.transaction_public_key_data)); \
memset(blockchain_data.extra_nonce_tag_data,0,strlen(blockchain_data.extra_nonce_tag_data)); \
memset(blockchain_data.reserve_bytes_size_data,0,strlen(blockchain_data.reserve_bytes_size_data)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data)); \
memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_secret_key_data)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_secret_key,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_secret_key)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_public_key)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_alpha_string)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_proof,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_proof)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string,0,strlen((const char*)blockchain_data.blockchain_reserve_bytes.vrf_beta_string)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round)); \
memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_data)); \
memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data)); \
for (count = 0; (int)count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++) \
{ \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key_data[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count],0,strlen((char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_secret_key[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key_data[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count],0,strlen((char*)blockchain_data.blockchain_reserve_bytes.block_verifiers_vrf_public_key[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_verifiers_random_data_text[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count],0,strlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address_data[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count],0,strlen(blockchain_data.blockchain_reserve_bytes.next_block_verifiers_public_address[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count])); \
  memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count],0,strlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count])); \
} \
memset(blockchain_data.ringct_version_data,0,strlen(blockchain_data.ringct_version_data)); \
memset(blockchain_data.transaction_amount_data,0,strlen(blockchain_data.transaction_amount_data)); \
for (count = 0; count < MAXIMUM_TRANSACATIONS_PER_BLOCK; count++) \
{ \
  memset(blockchain_data.transactions[count],0,strlen(blockchain_data.transactions[count])); \
} \
count = 0;
#endif