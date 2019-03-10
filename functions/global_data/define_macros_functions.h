#ifndef DEFINE_MACROS_FUNCTIONS_H_   /* Include guard */
#define DEFINE_MACROS_FUNCTIONS_H_

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define XCASH_DAEMON_PORT 18281 // The X-CASH Daemon RPC port
#define XCASH_WALLET_PORT 18285 // The X-CASH Wallet RPC port
#define XCASH_WALLET_PREFIX "XCA" // The prefix of a XCA address
#define XCASH_SIGN_DATA_PREFIX "SigV1" // The prefix of a xcash_proof_of_stake_signature for the signed data
#define XCASH_SIGN_DATA_LENGTH 93 // The length of a xcash_proof_of_stake_signature for the signed data
#define MAXIMUM_CONNECTIONS 100 // The maximum connections a node can have at one time
#define SOCKET_TIMEOUT_SETTINGS 1 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define SOCKET_END_STRING "|END|" // End string when sending data between nodes, to signal the end of sending data
#endif