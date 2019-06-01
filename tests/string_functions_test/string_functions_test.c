#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"

#include "string_functions.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "string_functions_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: string_functions_test
Description: Test the string functions
Return: The number of passed string_functions test
-----------------------------------------------------------------------------------------------------------
*/

int string_functions_test()
{
  // define macros
  #define STRING_FUNCTIONS_TOTAL_TEST 45

  #define DATA1 "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
  #define DATA2 "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}]"
 
  #define SIGN_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"xcash_proof_of_stake_signature\": \"SIGN_RPC_CALL_TEST_DATA\"\r\n  }\r\n}"
  #define VERIFY_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"good\": VERIFY_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
 
  
/* 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_public_key - The VRF public key.
  vrf_secret_key – The VRF secret key.
  vrf_alpha_string - The VRF alpha string. (this is the vrf_public_key for this part of the round)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_1_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_secret_key\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_1_OF_ROUND\",\r\n}"
 
 
 
/* 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_public_key - The VRF public key.
  vrf_alpha_string - The VRF alpha string. (this is the previous_block_hash|vrf_random_character_string)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_2_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_random_character_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_2_OF_ROUND\",\r\n}"
 
 
 
/* 
  The purpose of this message is for the main node to send the created data for the part of the round to the nodes, so they can verify the data.
 
  message_settings - The type of the message
  vrf_public_key - The same VRF public key from part 1 of the round.
  vrf_alpha_string - The VRF alpha string. (this is the vrf_beta_string created in round 2)
  vrf_proof - The VRF proof.
  vrf_beta_string - The VRF beta_string.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_3_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_random_character_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_public_key\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_alpha_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_proof\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"vrf_beta_string\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_3_OF_ROUND\",\r\n}"
 
 
 
/* 
  The purpose of this message is for the block producer to send the created block for the part of the round to the nodes, so they can verify the block.
 
  message_settings - The type of the message
  block_blob - The block_blob.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define MAIN_NODES_TO_NODES_PART_4_OF_ROUND "{\r\n \"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"block_blob\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"public_address\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"previous_block_hash\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"current_round_part\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"current_round_part_backup_node\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"data\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n \"xcash_proof_of_stake_signature\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND\",\r\n}"

 
 
/*
  This message is sent from the nodes in the mainnode_to_node_message_part_1 - 4 function and received by the nodes in the node_to_node function.
 
  The purpose of this message is for the nodes to verify what the other nodes received from the main node.
 
  message_settings - The type of the message
  vote_settings - Valid or Invalid if the data was not verified.
  vote_data - The SHA2-512 data that the node received.
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_NODES_VOTE_RESULTS "{\r\n \"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_settings\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"vote_data\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"public_address\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"previous_block_hash\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"current_round_part\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"current_round_part_backup_node\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"data\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_NODES_VOTE_RESULTS\",\r\n}"



  /*
  The purpose of this message is for the block verifiers to send the block verifiers their block blob signature
 
  message_settings - The type of the message
  block_blob_signature - The block blob signature
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"block_blob_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\",\r\n}"



/*
  The purpose of this message is for the block verifiers to send the main network data node a message so it knows to be the block producer.
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\",\r\n}"



/*
  The purpose of this message is for a node to send all block verifiers its delegate data to register

  message_settings - The type of the message
  delegate_name - The delegate name
  delegates_IP_address - The delegates IP address
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE "{\r\n \"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"delegate_name\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"delegates_IP_address\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"public_address\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"previous_block_hash\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"current_round_part\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"current_round_part_backup_node\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"data\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE\",\r\n}"




/*
  The purpose of this message is for a node to send all block verifiers the message to remove a delegate

  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE "{\r\n \"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"delegate_name\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"delegate_public_address\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"delegates_IP_address\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"public_address\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"previous_block_hash\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"current_round_part\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"current_round_part_backup_node\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"data\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE\",\r\n}"




/*
  The purpose of this message is for a node to send all block verifiers a message to update the delegates information

  message_settings - The type of the message
  item - The field name to update (about, website, team, pool_mode, fee_structure, server_settings)
  value - The field names value
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE "{\r\n \"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"item\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"value\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"public_address\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"previous_block_hash\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"current_round_part\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"current_round_part_backup_node\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"data\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n \"xcash_proof_of_stake_signature\": \"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE\",\r\n}"



 /*
  The purpose of this message is for the block verifiers to send the block verifiers the invalid reserve proofs they have found
 
  message_settings - The type of the message
  public_address_that_created_the_reserve_proof - The public address that created the reserve proof
  reserve_proof - The reserve proof
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address_that_created_the_reserve_proof\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n,\r\n \"reserve_proof\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\",\r\n}"




/*
  The purpose of this message is for a node to retrieve a list of the current block verifiers from a network data node
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \r\n \"public_address\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_hash\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"current_round_part\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"current_round_part_backup_node\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"data\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"xcash_proof_of_stake_signature\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}"
 
 
 
/*
  The purpose of this message is for the network data node to send the current block verifiers list to a node
 
  message_settings - The type of the message
  block_verifiers_list - The list of all block verifiers IP addresses IP_address1|IP_address2
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST "{\r\n \"message_settings\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"block_verifiers_list\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"public_address\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"previous_block_hash\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"current_round_part\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"current_round_part_backup_node\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"data\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n \"xcash_proof_of_stake_signature\": \"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}"
  
 
 
/*
  The purpose of this message is for a node to retrieve the reserve bytes for a given block
 
  message_settings - The type of the message
  block_height - The block height
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES "{\r\n \"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"block_height\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"public_address\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"previous_block_hash\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"current_round_part\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"current_round_part_backup_node\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"data\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n \"xcash_proof_of_stake_signature\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifier to send the reserve bytes for a given block to a node
 
  message_settings - The type of the message
  reserve_bytes - The reserve_bytes
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"reserve_bytes\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES\",\r\n}"
 
 
 
/*
  The purpose of this message is for a node to send a reserve proof to the block verifiers to add to the reserve proofs database
 
  message_settings - The type of the message
  delegates_public_address - The delegates public address
  reserve_proof - The reserve proof
  public_address - The public address of the node that is sending the data.
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF "{\r\n \"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n \"delegates_public_address\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n \"reserve_proof\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n \"public_address\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n \"data\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n \"xcash_proof_of_stake_signature\": \"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF\",\r\n}"


 
/*
  The purpose of this message is for a block verifiers to check if they are synced up to the reserve bytes decentralized database
 
  message_settings - The type of the message
  data_hash - The data hash of the reserve_bytes database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their reserve bytes database is up to date
 
  message_settings - The type of the message
  reserve_bytes_database - True if the database file is synced, false if not
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n}"
 


/*
  The purpose of this message is for a block verifiers to check if they are synced up to the reserve bytes decentralized database
 
  message_settings - The type of the message
  file - The file number
  data_hash - The data hash of the file
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their reserve bytes database is up to date
 
  message_settings - The type of the message
  reserve_bytes_database - True if the database file is synced, false if not
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifiers to download the up to date reserve bytes database
 
  message_settings - The type of the message
  file - The file number
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier the up to date reserve bytes database
 
  message_settings - The type of the message
  reserve_bytes_database - The reserve_bytes_database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_bytes_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifiers to check if they are synced up to the reserve proofs decentralized database
 
  message_settings - The type of the message
  data_hash - The data hash of the reserve proofs database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their reserve proofs database is up to date
 
  message_settings - The type of the message
  reserve_proofs_database - true if the database is synced, false if not
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD\",\r\n}"
 


/*
  The purpose of this message is for a block verifiers to check if they are synced up to the reserve proofs decentralized database
 
  message_settings - The type of the message
  file - The file number
  data_hash - The data hash of the file
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their reserve proofs database is up to date
 
  message_settings - The type of the message
  reserve_proofs_database - True if the database file is synced, false if not, empty if the database file is empty
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifiers to download the up to date reserve proofs database
 
  message_settings - The type of the message
  file - The file number
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier the up to date reserve proofs database
 
  message_settings - The type of the message
  reserve_proofs_database - The reserve proofs database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"reserve_proofs_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n}"
 
  
 
/*
  The purpose of this message is for a block verifiers to check if they are synced up to the delegates decentralized database
 
  message_settings - The type of the message
  data_hash - The data hash of the file
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their delegates database is up to date
 
  message_settings - The type of the message
  delegates_database - True if the database file is synced, false if not
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"delegates_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifiers to download the up to date delegates database
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier the up to date delegates database
 
  message_settings - The type of the message
  delegates_database - The delegates database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"delegates_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n}"



/*
  The purpose of this message is for a block verifiers to check if they are synced up to the statistics decentralized database
 
  message_settings - The type of the message
  data_hash - The data hash of the file
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier a message if their statistics database is up to date
 
  message_settings - The type of the message
  statistics_database - True if the database file is synced, false if not
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"statistics_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD\",\r\n}"
 
 
 
/*
  The purpose of this message is for a block verifiers to download the up to date statistics database
 
  message_settings - The type of the message
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
 
 
 
/*
  The purpose of this message is for the block verifiers to send a block verifier the up to date statistics database
 
  message_settings - The type of the message
  statistics_database - The statistics database
  public_address - The public address of the node that is sending the data.
  previous_block_hash - The previous block hash.
  current_round_part - The current round part (1-4).
  current_round_part_backup_node - The current main node in the current round part (0-5)
  data - A random 100 character string. This is the data that the xcash_proof_of_stake_signature is used for. The random data  will create a different xcash_proof_of_stake_signature for every message, even if the message data is the same.
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the data, used for verifying that the sender of the message is the sender.
  */
  #define BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"statistics_database\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"public_address\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"previous_block_hash\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"current_round_part_backup_node\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"data\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n \"xcash_proof_of_stake_signature\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD\",\r\n}"
 
  // variables
  int count;
  int counter;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;

  // reset the variables
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  count_test = 0;
 
  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mstring functions test - Total test: %d\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");
 
  // run the test
 
  // test parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field
  memcpy(result_test,SIGN_RPC_CALL_TEST_DATA,strnlen(SIGN_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"SIGN_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the sign RPC call for the xcash_proof_of_stake_signature field","red");
  }
 
  // test parsing the return data for the verify RPC call for the good field
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,VERIFY_RPC_CALL_TEST_DATA,strnlen(VERIFY_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"good",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"VERIFY_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the verify RPC call for the good field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the verify RPC call for the good field","red");
  }
 
  // test parsing the return data for the get_block_template RPC call for the blocktemplate_blob field
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,strnlen(GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,BUFFER_SIZE));
  if (parse_json_data(result_test,"blocktemplate_blob",data_test) != 0)
  {
    // check if the data is correct
    if (strncmp(data_test,"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA",BUFFER_SIZE) == 0)
    {
      color_print("PASSED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","green");
      count_test++;
    }
    else
    {
      color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
    }
  }
  else
  {
    color_print("FAILED! Test for parsing the return data for the get_block_template RPC call for the blocktemplate_blob field","red");
  }

  // test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_1_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_1_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_secret_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_1_OF_ROUND","green");
    count_test++;
  }

  // test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_2_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_2_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_random_character_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_2_OF_ROUND","green");
    count_test++;
  }

  // test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_3_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_3_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_public_key",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_alpha_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_proof",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vrf_beta_string",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_3_OF_ROUND","green");
    count_test++;
  }

  // test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,MAIN_NODES_TO_NODES_PART_4_OF_ROUND,strnlen(MAIN_NODES_TO_NODES_PART_4_OF_ROUND,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"block_blob",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND","red");
  }
  else
  {
    color_print("PASSED! Test for parsing MAIN_NODES_TO_NODES_PART_4_OF_ROUND","green");
    count_test++;
  }

  // test for parsing NODES_TO_NODES_VOTE_RESULTS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_NODES_VOTE_RESULTS,strnlen(NODES_TO_NODES_VOTE_RESULTS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vote_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"vote_data",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_NODES_VOTE_RESULTS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_NODES_VOTE_RESULTS","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"block_blob_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK,strnlen(BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK","green");
    count_test++;
  }

  // test for parsing NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE,strnlen(NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"delegate_name",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"delegates_IP_address",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE","green");
    count_test++;
  }

  // test for parsing NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE,strnlen(NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE","green");
    count_test++;
  }

  // test for parsing NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE,strnlen(NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"item",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"value",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address_that_created_the_reserve_proof",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_proof",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS","green");
    count_test++;
  }

  // test for parsing NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST,strnlen(NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }

  // test for parsing NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST,strnlen(NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"block_verifiers_list",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST","green");
    count_test++;
  }

  // test for parsing NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES,strnlen(NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"block_height",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES,strnlen(BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_bytes",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES","green");
    count_test++;
  }

  // test for parsing NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF,strnlen(NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 || parse_json_data(result_test,"delegates_public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_proof",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF","red");
  }
  else
  {
    color_print("PASSED! Test for parsing NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_bytes_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"file",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_bytes_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"file",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_bytes_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_proofs_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"file",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_proofs_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"file",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"reserve_proofs_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"delegates_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"delegates_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"statistics_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE","green");
    count_test++;
  }

  // test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(result_test,BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,strnlen(BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD,BUFFER_SIZE));
  if (parse_json_data(result_test,"message_settings",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"statistics_database",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"public_address",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"previous_block_hash",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"current_round_part_backup_node",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"data",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0 || parse_json_data(result_test,"xcash_proof_of_stake_signature",data_test) == 0 || strncmp(data_test,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD",BUFFER_SIZE) != 0)
  {
    color_print("FAILED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","red");
  }
  else
  {
    color_print("PASSED! Test for parsing BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD","green");
    count_test++;
  }

  // test for parse_reserve_bytes_data
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  memcpy(data_test,"XCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cXCASH_PROOF_OF_STAKE_TEST_DATA",876);
  if (parse_reserve_bytes_data(result_test,data_test,5,30) == 1 && memcmp(result_test,"XCASH_PROOF_OF_STAKE_TEST_DATA",30) == 0 && parse_reserve_bytes_data(result_test,data_test,10,30) == 0)
  {   
    color_print("PASSED! Test for parsing the reserve bytes data","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for parsing the reserve bytes data","red");
  }

  // test for random_string
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  if (random_string(result_test,RANDOM_STRING_LENGTH) == 1)
  {   
    color_print("PASSED! Test for creating a random string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for creating a random string","red");
  }

  // initialize the database_document_fields struct 
  for (count = 0; count < 14; count++)
  {
    database_data.item[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
    database_data.value[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));

    if (database_data.item[count] == NULL || database_data.value[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }

  memcpy(database_data.item[0],"username",8);
  memcpy(database_data.item[1],"most_total_rounds_delegate_name",31);
  memcpy(database_data.item[2],"most_total_rounds",17);
  memcpy(database_data.item[3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_data.item[4],"best_block_verifier_online_percentage",37);
  memcpy(database_data.item[5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_data.item[6],"most_block_producer_total_rounds",32);
  memcpy(database_data.item[7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_data.item[8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_data.item[9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_data.item[10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_data.item[11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_data.item[12],"total_coins_in_proof_of_stake",29);
  memcpy(database_data.item[13],"total_circulating_supply_percentage_in_proof_of_stake",53);

  memcpy(database_data.value[0],"XCASH",5);  
  memcpy(database_data.value[1],"DELEGATE_NAME",13);
  memcpy(database_data.value[2],"5",1);
  memcpy(database_data.value[3],"DELEGATE_NAME",13);
  memcpy(database_data.value[4],"10",2);
  memcpy(database_data.value[5],"DELEGATE_NAME",13);  
  memcpy(database_data.value[6],"15",2);
  memcpy(database_data.value[7],"DELEGATE_NAME",13);  
  memcpy(database_data.value[8],"5",1);
  memcpy(database_data.value[9],"DELEGATE_NAME",13);  
  memcpy(database_data.value[10],"10",2);  
  memcpy(database_data.value[11],"15",2);
  memcpy(database_data.value[12],"5",1);
  memcpy(database_data.value[13],"10",2);

  database_data.count = 14;

  if (create_json_data_from_database_document_array(&database_data,result_test,"") == 1)
  {
    if (strncmp(result_test,DATA1,BUFFER_SIZE) == 0)  
    {
      color_print("PASSED! Test for creating json data from a database document array","green");
      count_test++;
    }
    else
    { 
      color_print("FAILED! Test for creating json data from a database document array","red");
    }
  }
  else
  {
    color_print("FAILED! Test for creating json data from a database document array","red");
  }

  for (count = 0; count < 14; count++)
  {
    pointer_reset(database_data.item[count]);
    pointer_reset(database_data.value[count]);
  }



  // create_json_data_from_database_multiple_documents_array
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));

  // initialize the database_multiple_documents_fields struct 
  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 14; counter++)
    {
      database_multiple_documents_fields.item[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));
      database_multiple_documents_fields.value[count][counter] = (char*)calloc(BUFFER_SIZE,sizeof(char));

      if (database_multiple_documents_fields.item[count][counter] == NULL || database_multiple_documents_fields.value[count][counter] == NULL)
      {
        color_print("Could not allocate the memory needed on the heap","red");
        exit(0);
      }
    }
  }

  memcpy(database_multiple_documents_fields.item[0][0],"username",8);
  memcpy(database_multiple_documents_fields.item[0][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[0][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[0][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[0][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[0][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[0][6],"most_block_producer_total_rounds",32);
  memcpy(database_multiple_documents_fields.item[0][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_multiple_documents_fields.item[0][8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_multiple_documents_fields.item[0][9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_multiple_documents_fields.item[0][10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_multiple_documents_fields.item[0][11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_multiple_documents_fields.item[0][12],"total_coins_in_proof_of_stake",29);
  memcpy(database_multiple_documents_fields.item[0][13],"total_circulating_supply_percentage_in_proof_of_stake",53);
  memcpy(database_multiple_documents_fields.item[1][0],"username",8);
  memcpy(database_multiple_documents_fields.item[1][1],"most_total_rounds_delegate_name",31);
  memcpy(database_multiple_documents_fields.item[1][2],"most_total_rounds",17);
  memcpy(database_multiple_documents_fields.item[1][3],"best_block_verifier_online_percentage_delegate_name",51);
  memcpy(database_multiple_documents_fields.item[1][4],"best_block_verifier_online_percentage",37);
  memcpy(database_multiple_documents_fields.item[1][5],"most_block_producer_total_rounds_delegate_name",46);
  memcpy(database_multiple_documents_fields.item[1][6],"most_block_producer_total_rounds",32);
  memcpy(database_multiple_documents_fields.item[1][7],"most_VRF_node_public_and_private_key_total_rounds_delegate_name",63);
  memcpy(database_multiple_documents_fields.item[1][8],"most_VRF_node_public_and_private_key_total_rounds",49);
  memcpy(database_multiple_documents_fields.item[1][9],"most_VRF_node_random_data_total_rounds_delegate_name",52);
  memcpy(database_multiple_documents_fields.item[1][10],"most_VRF_node_random_data_total_rounds",38);
  memcpy(database_multiple_documents_fields.item[1][11],"total_XCASH_proof_of_stake_rounds",33);
  memcpy(database_multiple_documents_fields.item[1][12],"total_coins_in_proof_of_stake",29);
  memcpy(database_multiple_documents_fields.item[1][13],"total_circulating_supply_percentage_in_proof_of_stake",53);

  memcpy(database_multiple_documents_fields.value[0][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[0][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][2],"5",1);
  memcpy(database_multiple_documents_fields.value[0][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[0][4],"10",2);
  memcpy(database_multiple_documents_fields.value[0][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][6],"15",2);
  memcpy(database_multiple_documents_fields.value[0][7],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][8],"5",1);
  memcpy(database_multiple_documents_fields.value[0][9],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[0][10],"10",2);  
  memcpy(database_multiple_documents_fields.value[0][11],"15",2);
  memcpy(database_multiple_documents_fields.value[0][12],"5",1);
  memcpy(database_multiple_documents_fields.value[0][13],"10",2);
  memcpy(database_multiple_documents_fields.value[1][0],"XCASH",5);  
  memcpy(database_multiple_documents_fields.value[1][1],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][2],"5",1);
  memcpy(database_multiple_documents_fields.value[1][3],"DELEGATE_NAME",13);
  memcpy(database_multiple_documents_fields.value[1][4],"10",2);
  memcpy(database_multiple_documents_fields.value[1][5],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][6],"15",2);
  memcpy(database_multiple_documents_fields.value[1][7],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][8],"5",1);
  memcpy(database_multiple_documents_fields.value[1][9],"DELEGATE_NAME",13);  
  memcpy(database_multiple_documents_fields.value[1][10],"10",2);  
  memcpy(database_multiple_documents_fields.value[1][11],"15",2);
  memcpy(database_multiple_documents_fields.value[1][12],"5",1);
  memcpy(database_multiple_documents_fields.value[1][13],"10",2);

  database_multiple_documents_fields.document_count = 2;
  database_multiple_documents_fields.database_fields_count = 14;

  if (create_json_data_from_database_multiple_documents_array(&database_multiple_documents_fields,result_test,"") == 1)
  {
    if (strncmp(result_test,DATA2,BUFFER_SIZE) == 0)  
    {
      color_print("PASSED! Test for creating json data from a database multiple documents array","green");
      count_test++;
    }
    else
    { 
      color_print("FAILED! Test for creating json data from a database multiple documents array","red");
    }
  }
  else
  {
    color_print("FAILED! Test for creating json data from a database multiple documents array","red");
  }

  for (count = 0; count < 2; count++)
  {
    for (counter = 0; counter < 14; counter++)
    {
      pointer_reset(database_multiple_documents_fields.item[count][counter]);
      pointer_reset(database_multiple_documents_fields.value[count][counter]);
    }
  }  

  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,TEST_OUTLINE,strnlen(TEST_OUTLINE,BUFFER_SIZE));

  // test for counting occurences of a substring in a string
  if (string_count(result_test,"-") == strnlen(TEST_OUTLINE,BUFFER_SIZE) && string_count(result_test,"--") == strnlen(TEST_OUTLINE,BUFFER_SIZE)/2)  
  {
    color_print("PASSED! Test for counting occurences of a substring in a string","green");
    count_test++;
  }
  else
  {
    color_print("FAILED! Test for counting occurences of a substring in a string","red");
  }

  // test for string replace
  memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
  memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
  if (string_replace(result_test,"string_replace_test","string_replace") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
  {
    memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
    memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
     if (string_replace(result_test,"\"","\\\"") == 1 && strncmp(result_test,"{\r\n \\\"message_settings\\\": \\\"string_replace_test\\\",\r\n}",BUFFER_SIZE) == 0)
     {
       memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
       memcpy(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}",49);
       if (string_replace(result_test,"_test","") == 1 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
       {
         memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
         memcpy(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",44);
         if (string_replace(result_test,"string_replace_test","") == 0 && strncmp(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}",BUFFER_SIZE) == 0)
         {    
           color_print("PASSED! Test for string replace","green");
           count_test++;
         }
         else
         {
           color_print("FAILED! Test for string replace","red");
         }
       }
       else
       {
         color_print("FAILED! Test for string replace","red");
       }
     }
     else
     {
       color_print("FAILED! Test for string replace","red");
     }
  }
  else
  {
    color_print("FAILED! Test for string replace","red");
  }
 
  // write the end test message
  if (count_test == STRING_FUNCTIONS_TOTAL_TEST)
  {
    printf("\n\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mstring functions test - Passed test: %d, Failed test: 0\033[0m\n",STRING_FUNCTIONS_TOTAL_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mstring functions test - Passed test: %d, Failed test: %d\033[0m\n",count_test,STRING_FUNCTIONS_TOTAL_TEST-count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
  }
  return count_test;

  #undef STRING_FUNCTIONS_TOTAL_TEST
  #undef DATA1
  #undef DATA2
  #undef SIGN_RPC_CALL_TEST_DATA
  #undef VERIFY_RPC_CALL_TEST_DATA
  #undef MAIN_NODES_TO_NODES_PART_1_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_2_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_3_OF_ROUND
  #undef MAIN_NODES_TO_NODES_PART_4_OF_ROUND
  #undef NODES_TO_NODES_VOTE_RESULTS
  #undef BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK
  #undef NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE
  #undef NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE
  #undef NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE
  #undef BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS
  #undef NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST
  #undef NETWORK_DATA_NODE_TO_NODE_SEND_CURRENT_BLOCK_VERIFIERS_LIST
  #undef NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES
  #undef BLOCK_VERIFIERS_TO_NODE_SEND_RESERVE_BYTES
  #undef NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DELEGATES_STATISTICS_DATABASE_SYNC_CHECK_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DELEGATES_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_DOWNLOAD
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE
  #undef BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_DOWNLOAD
}