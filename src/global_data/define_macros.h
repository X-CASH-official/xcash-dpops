#ifndef DEFINE_MACROS_H_   /* Include guard */
#define DEFINE_MACROS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

// Blockchain
#define XCASH_WALLET_LENGTH 98 // The length of a XCA addres
#define XCASH_WALLET_PREFIX "XCA" // The prefix of a XCA address
#define XCASH_TOTAL_SUPPLY 100000000000 // The total supply
#define XCASH_PREMINE_TOTAL_SUPPLY 40000000000 // The total supply of the premine
#define XCASH_PREMINE_CIRCULATING_SUPPLY 7000000000 // The circulating supply of the premine
#define XCASH_WALLET_DECIMAL_PLACES_AMOUNT 1000000
#define XCASH_EMMISION_FACTOR 524288 // The emmision factor (2^19)
#define XCASH_DPOPS_EMMISION_FACTOR 262144 // The XCASH_DPOPS emmision factor (2^18)
#define FIRST_BLOCK_MINING_REWARD 190734.863281 // The first blocks mining reward
#define XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT 449850 // The start block height for X-CASH proof of stake
//#define XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT 440875 // The start block height for X-CASH proof of stake
#define BLOCK_TIME 5 // The block time in minutes
#define BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME 288 // The blocks per day with a 5 minute block time
#define UNLOCK_BLOCK_AMOUNT 60 // The default unlock block amount for a block reward transaction
#define MAXIMUM_TRANSACATIONS_PER_BLOCK 10000 // The maximum amount of transaction per block
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a XCASH_DPOPS_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a XCASH_DPOPS_signature for the signed datas

// Network block string 
#define BLOCK_PRODUCER_NETWORK_BLOCK_NONCE "00000000" // The network block nonce used when the block producer creates the block
#define CONSENSUS_NODE_NETWORK_BLOCK_NONCE "11111111" // The network block nonce used when the consensus node creates the block
#define NETWORK_VERSION "0d0d" // The network version
#define TIMESTAMP_LENGTH 10 // The length of the timestamp
#define BLOCK_HASH_LENGTH 64 // The length of the block hash
#define STEALTH_ADDRESS_OUTPUT_LENGTH 64 // The length of the stealth address output
#define TRANSACTION_PUBLIC_KEY_LENGTH 66 // The length of the transaction public key
#define TRANSACTION_LENGTH 64 // The length of a transaction
#define BLOCK_REWARD_TRANSACTION_VERSION "02"
#define BLOCK_REWARD_INPUT "01"
#define VIN_TYPE "ff"
#define BLOCK_REWARD_OUTPUT "01"
#define STEALTH_ADDRESS_OUTPUT_TAG "02"
#define TRANSACTION_PUBLIC_KEY_TAG "01"
#define EXTRA_NONCE_TAG "02"
#define RINGCT_VERSION "00"
#define BLOCKCHAIN_RESERVED_BYTES_START "7c424c4f434b434841494e5f52455345525645445f42595445535f53544152547c"
#define BLOCKCHAIN_RESERVED_BYTES_DATA_HASH "02800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // the reserve bytes data hash
#define BLOCKCHAIN_DATA_SEGMENT_STRING "7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c"
#define BLOCKCHAIN_DATA_SEGMENT_SIGN_DATA_STRING "7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5369675631"
#define BLOCKCHAIN_RESERVED_BYTES_END "7c424c4f434b434841494e5f52455345525645445f42595445535f454e447c"
#define BLOCKCHAIN_RESERVED_BYTES_START_DATA "424c4f434b434841494e5f52455345525645445f42595445535f5354415254"
#define BLOCKCHAIN_RESERVED_BYTES_END_DATA "424c4f434b434841494e5f52455345525645445f42595445535f454e44"
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY_DATA "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a VRF secret key, for the VRF secret key data
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_SECRET_KEY "0000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a VRF secret key
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY_DATA "0000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a VRF public key, for the VRF public key data
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_VRF_PUBLIC_KEY "00000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a VRF public key
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING_DATA "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a random string, for the random string data
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_RANDOM_STRING "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a random string
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE_DATA "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a block verifier signature, for the block verifier signature data
#define GET_BLOCK_TEMPLATE_BLOCK_VERIFIERS_SIGNATURE "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" // The place holder bytes for a block verifier that does not create a block verifier signature

// XCASH DPOPS
#define BLOCK_VERIFIERS_TOTAL_AMOUNT 100 // The total amount of block verifiers
#define BLOCK_VERIFIERS_AMOUNT 5 // The amount of block verifiers in a round
#define BLOCK_VERIFIERS_VALID_AMOUNT 5 // The amount of block verifiers that need to vote true for the part of the round to be valid
#define MAXIMUM_AMOUNT_OF_DELEGATES 500 // The maximum amount of delegates that can be registered
#define MAXIMUM_AMOUNT_OF_VOTERS_PER_DELEGATE 1000 // The maximum amount of voters that a delegate can have
#define BLOCK_PRODUCERS_BACKUP_AMOUNT 5 // The amount of backup block producers for a network
#define MINIMUM_AMOUNT_RESERVE_PROOF 2000000000000 // The minimum amount to create a reserve proof

// Network
#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define SEND_DATA_PORT 18283 // The port that is used by all nodes to send and receive data
#define MAXIMUM_CONNECTIONS 1000 // The maximum connections a node can have at one time
#define BLOCK_VERIFIERS_SETTINGS 2 // The time in seconds to wait to send data to the block verifiers
#define TOTAL_CONNECTION_TIME_SETTINGS 5 // The total time a client is given to connect to the server and send data
#define SOCKET_CONNECTION_TIMEOUT_SETTINGS 5000 // The time in milliseconds, to wait before a connection is cancelled
#define SOCKET_DATA_TIMEOUT_SETTINGS 5 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define SEND_PAYMENT_TIMEOUT_SETTINGS 600 // The maximum amount of time to wait in seconds for the send_payment function
#define SOCKET_END_STRING "|END|" // End string when sending data between nodes, to signal the end of sending data
#define HTTP_SOCKET_END_STRING "\r\n\r\n" // End string for a HTTP request, to signal the end of sending data

// Lengths
#define BUFFER_SIZE 164000
#define BUFFER_SIZE_RESERVE_PROOF 10000
#define BUFFER_SIZE_NETWORK_BLOCK_DATA 500
#define BUFFER_SIZE_NETWORK_BLOCK_TRANSACTIONS_DATA 100
#define MAXIMUM_BUFFER_SIZE 52428800 // 50 MB

#define RANDOM_STRING_LENGTH 100 // The length of the random string
#define DATA_HASH_LENGTH 128 // The length of the SHA2-512 hash
#define BITS_IN_BYTE 8 // 8 bits in 1 byte
#define BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH 100 // The maximum length of the block verifiers IP address
#define TOTAL_ERROR_MESSAGES 100 // The total amount of error messages
#define BLOCK_VERIFIERS_NAME_TOTAL_LENGTH 100 // The maximum length of the block verifiers name
#define MAXIMUM_INVALID_RESERVE_PROOFS 50000 // The maximum invalid reserve proofs for the delegates

// VRF
#define VRF_PUBLIC_KEY_LENGTH 64
#define VRF_SECRET_KEY_LENGTH 128
#define VRF_PROOF_LENGTH 160
#define VRF_BETA_LENGTH 128
#define VRF_DATA "74727565" // true when the VRF data is verified

// Database
#define DATABASE_CONNECTION "mongodb://localhost:27017" // The database connection string
#define DATABASE_NAME "XCASH_PROOF_OF_STAKE" // The name of the database
#define DATABASE_NAME_DELEGATES "XCASH_PROOF_OF_STAKE_DELEGATES" // The name of the database for the delegates
#define TOTAL_RESERVE_PROOFS_DATABASES 50 // The total reserve proofs databases
#define TOTAL_RESERVE_BYTES_DATABASE_FIELDS 3 // The reserve bytes database fields
#define TOTAL_RESERVE_PROOFS_DATABASE_FIELDS 4 // The reserve proofs database fields
#define TOTAL_DELEGATES_DATABASE_FIELDS 17 // The total delegates database fields
#define TOTAL_STATISTICS_DATABASE_FIELDS 7 // The total statistics database fields
#define TOTAL_PUBLIC_ADDRESSES_DATABASE_FIELDS 4 // The total public addresses database fields
#define TOTAL_BLOCKS_FOUND_DATABASE_FIELDS 5 // The total blocks found database fields

// Shared delegates settings
#define VOTER_INACTIVITY_COUNT "30" // the number of days to wait to remove an inactive delegates information from the database

// Network data nodes
#define NETWORK_DATA_NODES_AMOUNT 5 // The amount of network data nodes
#define NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12"
#define NETWORK_DATA_NODE_1_IP_ADDRESS "delegates.xcash.foundation"
#define NETWORK_DATA_NODE_2_PUBLIC_ADDRESS "XCA1dNsv9cGc5kPMrgpdTkGttM17uR2JvCpmraGschxYSEt3MK4NRmmgyc13CTYWBGDNefdem5MFsG384DuUpGKc3ShZa4R56e"
#define NETWORK_DATA_NODE_2_IP_ADDRESS "europe1.xcash.foundation"
#define NETWORK_DATA_NODE_3_PUBLIC_ADDRESS "XCA1rU5hFV98QvysF3ByeZSPkt9wPyUxkSErBZADJjsHPMKnmCxKFH2H6aLy3oFbYaGkkYGCJcLF1ERWT5uQweEu8yZodwCtHc"
#define NETWORK_DATA_NODE_3_IP_ADDRESS "europe2.xcash.foundation"
#define NETWORK_DATA_NODE_4_PUBLIC_ADDRESS "XCA1kk9q8H7JNe9aWXLYRpG2oqFQxLD7vTy8s3pPZprBVnLRQNAurnabEHsQCSAUyxC8nForSa2C39qAhtFt4f845ZSz2Xz5Mr"
#define NETWORK_DATA_NODE_4_IP_ADDRESS "europe3.xcash.foundation"
#define NETWORK_DATA_NODE_5_PUBLIC_ADDRESS "XCA1diBcGjRhBEdDkphu5oUTTvDHiSGjmZ7unCwBFgdpMiCQoF1BpMWP2E96iFWoWoD41npDcRUo51ih45We29Hd5XZsikzt71"
#define NETWORK_DATA_NODE_5_IP_ADDRESS "asia1.xcash.foundation"

#define DATABASE_COLLECTION_DELEGATES_DATA_1 "{\"public_address\":\"XCA1h3yJ318hJGTFeXfYyEcyE7G4hX7jrCbvz21VecJGhf64Tw51yWii2Q1e76fJbB26Ea8CRipmdW6ZHQcRrLKx3cxRkr5M12\",\"total_vote_count\":\"0\",\"IP_address\":\"delegates.xcash.foundation\",\"delegate_name\":\"delegates_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"delegates.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 32 threads (Intel 2xE5 - 2660 - 2.2GHz, RAM: 256GB DDR3, Hard drive: 2x240 GB SSD + 4x2TB SATA, Bandwidth Transfer: Unlimited, Bandwidth Speed: 500 Mbps upload and 500 Mbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_2 "{\"public_address\":\"XCA1dNsv9cGc5kPMrgpdTkGttM17uR2JvCpmraGschxYSEt3MK4NRmmgyc13CTYWBGDNefdem5MFsG384DuUpGKc3ShZa4R56e\",\"total_vote_count\":\"0\",\"IP_address\":\"europe1.xcash.foundation\",\"delegate_name\":\"europe1_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 8 threads (Intel D-1521 - 2.40GHz, RAM: 32GB DDR3, Hard drive: 2x HDD SATA 2 TB\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_3 "{\"public_address\":\"XCA1rU5hFV98QvysF3ByeZSPkt9wPyUxkSErBZADJjsHPMKnmCxKFH2H6aLy3oFbYaGkkYGCJcLF1ERWT5uQweEu8yZodwCtHc\",\"total_vote_count\":\"0\",\"IP_address\":\"europe2.xcash.foundation\",\"delegate_name\":\"europe2_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 8 threads (Intel i7-6700 - 3.40GHz, RAM: 64GB DDR4, Hard drive: 2x NVMe SSD 512GB, Bandwidth Transfer: Unlimited, Bandwidth Speed: 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_4 "{\"public_address\":\"XCA1kk9q8H7JNe9aWXLYRpG2oqFQxLD7vTy8s3pPZprBVnLRQNAurnabEHsQCSAUyxC8nForSa2C39qAhtFt4f845ZSz2Xz5Mr\",\"total_vote_count\":\"0\",\"IP_address\":\"europe3.xcash.foundation\",\"delegate_name\":\"europe3_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 8 threads (Intel i7-6700 - 3.40GHz, RAM: 64GB DDR4, Hard drive: 2x NVMe SSD 512GB, Bandwidth Transfer: Unlimited, Bandwidth Speed: 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_5 "{\"public_address\":\"XCA1diBcGjRhBEdDkphu5oUTTvDHiSGjmZ7unCwBFgdpMiCQoF1BpMWP2E96iFWoWoD41npDcRUo51ih45We29Hd5XZsikzt71\",\"total_vote_count\":\"0\",\"IP_address\":\"asia1.xcash.foundation\",\"delegate_name\":\"asia1_xcash_foundation\",\"about\":\"Official X-Network node\",\"website\":\"\",\"team\":\"X-Network Team\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 8 threads (Intel E3-1245 v5 - 3.50GHz, RAM: 32GB DDR3, Hard drive: 2x HDD SATA 2 TB\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_6 "{\"public_address\":\"XCA1kkdrRQ9SC7wfJ3F329giei31r13wKEKiD1ZMYf6nBiU8KE6a6LDJTZobfJ58o8A3vtGrAkzfHDd815kac7F17iPhK9zYGG\",\"total_vote_count\":\"0\",\"IP_address\":\"officialdelegate.xcash.foundation\",\"delegate_name\":\"officialdelegate_xcash_foundation\",\"about\":\"Official X-Network Shared Delegate\",\"website\":\"officialdelegate.xcash.foundation\",\"team\":\"X-Network Team\",\"pool_mode\":\"true\",\"fee_structure\":\"1\",\"server_settings\":\"Operating System: Ubuntu 18.04, CPU: 8 threads (Intel E3-1246 v3 - 3.50GHz, RAM: 32GB DDR3, Hard drive: 2x HDD SATA 2 TB, Bandwidth Transfer: Unlimited, Bandwidth Speed: 1 Gbps upload and 1 Gbps download\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"delegates_xcash_foundation\",\"most_total_rounds\":\"0\",\"best_block_verifier_online_percentage_delegate_name\":\"delegates_xcash_foundation\",\"best_block_verifier_online_percentage\":\"0\",\"most_block_producer_total_rounds_delegate_name\":\"delegates_xcash_foundation\",\"most_block_producer_total_rounds\":\"0\"}"

#define INITIALIZE_DATABASE_DATA \
color_print("Initializing database data","yellow"); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_2,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_3,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_4,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_5,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_6,0); \
insert_document_into_collection_json(DATABASE_NAME,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0); \
sleep(10);

#define INITIALIZE_NETWORK_DATA_NODES \
memcpy(network_data_nodes_list.network_data_nodes_public_address[0],NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[0],NETWORK_DATA_NODE_1_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_1_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[1],NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[1],NETWORK_DATA_NODE_2_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_2_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[2],NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[2],NETWORK_DATA_NODE_3_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_3_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[3],NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[3],NETWORK_DATA_NODE_4_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_4_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[4],NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[4],NETWORK_DATA_NODE_5_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_5_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \

#define CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE \
if (memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0) \
{ \
  network_data_node_settings = 1; \
}

/*// Network data nodes test
#define NETWORK_DATA_NODES_AMOUNT 2 // The amount of network data nodes
#define NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP"
#define NETWORK_DATA_NODE_1_IP_ADDRESS "192.168.1.201"
#define NETWORK_DATA_NODE_2_PUBLIC_ADDRESS "XCA1VSDHKCc4Qhvqb3fquebSYxfMeyGteQeAYtDSpaTcgquBY1bkKWtQ42tZG2w7Ak7GyqnaiTgWL4bMHE9Lwd2A3g2Recxz7B"
#define NETWORK_DATA_NODE_2_IP_ADDRESS "192.168.1.202"

#define DATABASE_COLLECTION_DELEGATES_DATA_1 "{\"public_address\":\"XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP\",\"total_vote_count\":\"0\",\"IP_address\":\"192.168.1.201\",\"delegate_name\":\"delegate_name_1\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_DELEGATES_DATA_2 "{\"public_address\":\"XCA1VSDHKCc4Qhvqb3fquebSYxfMeyGteQeAYtDSpaTcgquBY1bkKWtQ42tZG2w7Ak7GyqnaiTgWL4bMHE9Lwd2A3g2Recxz7B\",\"total_vote_count\":\"0\",\"IP_address\":\"192.168.1.202\",\"delegate_name\":\"delegate_name_2\",\"about\":\"\",\"website\":\"\",\"team\":\"\",\"pool_mode\":\"false\",\"fee_structure\":\"\",\"server_settings\":\"\",\"block_verifier_score\":\"0\",\"online_status\":\"true\",\"block_verifier_total_rounds\":\"0\",\"block_verifier_online_total_rounds\":\"0\",\"block_verifier_online_percentage\":\"0\",\"block_producer_total_rounds\":\"0\",\"block_producer_block_heights\":\"\"}"
#define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"delegate_name_1\",\"most_total_rounds\":\"0\",\"best_block_verifier_online_percentage_delegate_name\":\"delegate_name_1\",\"best_block_verifier_online_percentage\":\"0\",\"most_block_producer_total_rounds_delegate_name\":\"delegate_name_1\",\"most_block_producer_total_rounds\":\"0\"}"

#define INITIALIZE_DATABASE_DATA \
color_print("Initializing database data","yellow"); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_1,0); \
insert_document_into_collection_json(DATABASE_NAME,"delegates",DATABASE_COLLECTION_DELEGATES_DATA_2,0); \
insert_document_into_collection_json(DATABASE_NAME,"statistics",DATABASE_COLLECTION_STATISTICS_DATA,0); \
sleep(10)

#define INITIALIZE_NETWORK_DATA_NODES \
memcpy(network_data_nodes_list.network_data_nodes_public_address[0],NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[0],NETWORK_DATA_NODE_1_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_1_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); \
memcpy(network_data_nodes_list.network_data_nodes_public_address[1],NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH); \
memcpy(network_data_nodes_list.network_data_nodes_IP_address[1],NETWORK_DATA_NODE_2_IP_ADDRESS,strnlen(NETWORK_DATA_NODE_2_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));

#define CHECK_IF_BLOCK_VERIFIERS_IS_NETWORK_DATA_NODE \
if (memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0 || memcmp(xcash_wallet_public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,XCASH_WALLET_LENGTH) == 0) \
{ \
  network_data_node_settings = 1; \
}*/

// Test
#define TEST_OUTLINE "-----------------------------------------------------------------------------------------------"
#define INVALID_PARAMETERS_ERROR_MESSAGE \
"Parameters\n" \
"--parameters - Show a list of all valid parameters\n\n" \
"--test - Run the test to make sure the program is compatible with your system\n\n" \
"--total_threads \"total_threads\" - The total threads to use, the maximum is 100 total threads.\nIf this parameter is not specified, the default is the number of threads the CPU has.\n\n" \
"--delegates_website - Run the delegates website\n\n" \
"--shared_delegates_website --fee \"fee\" --minimum_amount \"minimum_amount\" - Run the shared delegates website, with a fee of \"fee\" and a minimum amount of \"minimum_amount\"\n" \
"The fee in a percentage (1 would equal 1 percent. You can use up to 6 decimal places.)\n" \
"The minimum for a public_address to receive a payment (10000 etc. The minimum amount should be in regular units, not atomic units.)\n\n" \
"--synchronize_database_from_network_data_node - Synchronize the database from a network data node.\n\n" \
"--disable_synchronizing_databases_and_starting_timers - Disables synchronizing the databases and starting the timers. Used for testing.\n\n" \
"--test_data_add - Add test data to the databases\n\n" \
"--test_data_remove - Remove test data from the databases\n\n"
#endif