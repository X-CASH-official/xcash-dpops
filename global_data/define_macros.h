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
#define XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT 240501 // the start block height for X-CASH proof of stake
#define BLOCK_TIME 5 // the block time in minutes
#define BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME 288 // The blocks per day with a 5 minute block time
#define UNLOCK_BLOCK_AMOUNT 60 // The default unlock block amount for a block reward transaction
#define MAXIMUM_TRANSACATIONS_PER_BLOCK 1000000 // The maximum amount of transaction per block
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a xcash_proof_of_stake_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a xcash_proof_of_stake_signature for the signed datas

// network block string 
#define BLOCK_PRODUCER_NETWORK_BLOCK_NONCE "00000000" // the network block nonce used when the block producer creates the block
#define CONSENSUS_NODE_NETWORK_BLOCK_NONCE "11111111" // the network block nonce used when the consensus node creates the block
#define NETWORK_VERSION "0d0d" // the network version
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

// proof of stake
#define BLOCK_VERIFIERS_AMOUNT 4 // The amount of block verifiers in a round
#define BLOCK_VERIFIERS_SELECTED_AMOUNT 4 // The amount of block verifiers selected. The top 100 that are online are the block verifiers
#define BLOCK_VERIFIERS_VALID_AMOUNT 4 // The amount of block verifiers that need to vote true for the part of the round to be valid.
#define BLOCK_PRODUCERS_BACKUP_AMOUNT 5 // The amount of backup block producers for a network

// Network
#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define SEND_DATA_PORT 8000 // The port that is used by all nodes to send and receive data
#define MAXIMUM_CONNECTIONS 1000 // The maximum connections a node can have at one time
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define SOCKET_CONNECTION_TIMEOUT_SETTINGS 10000 // The time in milliseconds, to wait before a connection is cancelled
#define SOCKET_DATA_TIMEOUT_SETTINGS 10 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define SOCKET_END_STRING "|END|" // End string when sending data between nodes, to signal the end of sending data

// lengths
#define BUFFER_SIZE 164000
#define BUFFER_SIZE_RESERVE_PROOF 10000
#define BUFFER_SIZE_NETWORK_BLOCK_DATA 500
#define BUFFER_SIZE_NETWORK_BLOCK_TRANSACTIONS_DATA 100
#define MAXIMUM_BUFFER_SIZE 52428800 // 50 MB

#define RANDOM_STRING_LENGTH 100 // The length of the random string
#define DATA_HASH_LENGTH 128 // The length of the SHA2-512 hash
#define BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH 100 // The maximum length of the block verifiers IP address
#define BLOCK_VERIFIERS_NAME_TOTAL_LENGTH 100 // The maximum length of the block verifiers name
#define MAXIMUM_INVALID_RESERERVE_PROOFS 50000 // The maximum invalid reserve proofs for the delegates

// VRF
#define VRF_PUBLIC_KEY_LENGTH 64
#define VRF_SECRET_KEY_LENGTH 128
#define VRF_PROOF_LENGTH 160
#define VRF_BETA_LENGTH 128
#define VRF_DATA "74727565" // true when the VRF data is verified

// Database
#define DATABASE_CONNECTION "mongodb://localhost:27017" // The database connection string
#define DATABASE_NAME "XCASH_PROOF_OF_STAKE" // The name of the database
#define DATABASE_ARRAY_COUNT 150 // The maximum amount of parsed item and value from the database
#define TOTAL_RESERVE_PROOFS_DATABASES 50 // The total reserve proofs databases

// network data nodes
#define NETWORK_DATA_NODES_AMOUNT 2 // The amount of network data nodes
#define NETWORK_DATA_NODE_1_PUBLIC_ADDRESS "XCA1pEWxj2q7gn7TJjae7JfsDhtnhydxsHhtADhDm4LbdE11rHVZqbX5MPGZ9tM7jQbDF4VKK89jSAqgL9Nxxjdh8RM5JEpZZP" // the first network data nodes public address
#define NETWORK_DATA_NODE_1_IP_ADDRESS "10.10.10.1" // the first network data nodes IP address
#define NETWORK_DATA_NODE_2_PUBLIC_ADDRESS "XCA1VSDHKCc4Qhvqb3fquebSYxfMeyGteQeAYtDSpaTcgquBY1bkKWtQ42tZG2w7Ak7GyqnaiTgWL4bMHE9Lwd2A3g2Recxz7B" // the second network data nodes public address
#define NETWORK_DATA_NODE_2_IP_ADDRESS "10.10.10.2" // the second network data nodes IP address

// Test
#define TEST_OUTLINE "-----------------------------------------------------------------------------------------------"
#define INVALID_PARAMETERS_ERROR_MESSAGE \
"\n\nParameters\n" \
"--test - Run the test to make sure the program is compatible with your system\n"
#endif