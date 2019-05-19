#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "define_macros_functions.h"
#include "database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "blockchain_functions.h"
#include "server_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

#include "define_macros_test.h"
#include "variables_test.h"
#include "network_functions_test.h"
#include "reset_variables_allocated_on_the_heap_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: get_program_memory_usage
Description: Gets the current memory usage of the program
Parameters:
   process_id_file - The file to read the memory usage from
Return: 0 if an error has occured, otherwise the programs memory usage in KB
-----------------------------------------------------------------------------------------------------------
*/

size_t get_program_memory_usage(char* process_id_file)
{
  // Variables
  FILE* file;
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t programs_memory_usage = 0;

  if (data == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // read the current system memory usage
  file = fopen(process_id_file,"r");
  if (file != NULL)
  {
    while(fgets(data,BUFFER_SIZE,file) != NULL)
    {
      if (strstr(data,"VmSize:") != NULL)
      {
        string_replace(data,"VmSize:","");
        string_replace(data,"	",""); // tab
        string_replace(data," ",""); // space
        string_replace(data,"kB","");
        string_replace(data,"\n","");
        programs_memory_usage = atoi(data);
      }
      memset(data,0,strnlen(data,BUFFER_SIZE));
    }
    fclose(file);
  }
  pointer_reset(data);
  return programs_memory_usage;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: reset_variables_allocated_on_the_heap_test
Description: Test each function to make sure that the variables are reset that were allocated on the heap
Return: The number of passed reset_variables_allocated_on_the_heap test
-----------------------------------------------------------------------------------------------------------
*/

int reset_variables_allocated_on_the_heap_test()
{
  // Constants
  const char STR1_TEST [BUFFER_SIZE] = "test string 1";
  const char* STR2_TEST = "test string 2";
  const unsigned char data2[crypto_vrf_SEEDBYTES] = {0x4c,0xcd,0x08,0x9b,0x28,0xff,0x96,0xda,0x9d,0xb6,0xc3,0x46,0xec,0x11,0x4e,0x0f,0x5b,0x8a,0x31,0x9f,0x35,0xab,0xa6,0x24,0xda,0x8c,0xf6,0xed,0x4f,0xb8,0xa6,0xfb};
  const unsigned char public_key[crypto_vrf_PUBLICKEYBYTES] = {0x3d,0x40,0x17,0xc3,0xe8,0x43,0x89,0x5a,0x92,0xb7,0x0a,0xa7,0x4d,0x1b,0x7e,0xbc,0x9c,0x98,0x2c,0xcf,0x2e,0xc4,0x96,0x8c,0xc0,0xcd,0x55,0xf1,0x2a,0xf4,0x66,0x0c};
  const unsigned char proof[crypto_vrf_PROOFBYTES] = {0xae,0x5b,0x66,0xbd,0xf0,0x4b,0x4c,0x01,0x0b,0xfe,0x32,0xb2,0xfc,0x12,0x6e,0xad,0x21,0x07,0xb6,0x97,0x63,0x4f,0x6f,0x73,0x37,0xb9,0xbf,0xf8,0x78,0x5e,0xe1,0x11,0x20,0x00,0x95,0xec,0xe8,0x7d,0xde,0x4d,0xbe,0x87,0x34,0x3f,0x6d,0xf3,0xb1,0x07,0xd9,0x17,0x98,0xc8,0xa7,0xeb,0x12,0x45,0xd3,0xbb,0x9c,0x5a,0xaf,0xb0,0x93,0x35,0x8c,0x13,0xe6,0xae,0x11,0x11,0xa5,0x57,0x17,0xe8,0x95,0xfd,0x15,0xf9,0x9f,0x07};
  const unsigned char alpha_string[1] = {"\x72"};
  const unsigned char beta_string[crypto_vrf_OUTPUTBYTES] = {0x94,0xf4,0x48,0x7e,0x1b,0x2f,0xec,0x95,0x43,0x09,0xef,0x12,0x89,0xec,0xb2,0xe1,0x50,0x43,0xa2,0x46,0x1e,0xcc,0x7b,0x2a,0xe7,0xd4,0x47,0x06,0x07,0xef,0x82,0xeb,0x1c,0xfa,0x97,0xd8,0x49,0x91,0xfe,0x4a,0x7b,0xfd,0xfd,0x71,0x56,0x06,0xbc,0x27,0xe2,0x96,0x7a,0x6c,0x55,0x7c,0xfb,0x58,0x75,0x87,0x9b,0x67,0x17,0x40,0xb7,0xd8};

  // Variables
  char* process_id_file = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char** data = (char**)calloc(5 * sizeof(char*),sizeof(char*));
  char** settings = (char**)calloc(5 * sizeof(char*),sizeof(char*));
  int previous_system_memory_usage = 0;
  int current_system_memory_usage = 0;
  int current_memory_usage = 0;
  size_t count = 0;
  size_t count2 = 0;
  size_t counter = 0;
  struct database_document_fields database_data;
  struct database_multiple_documents_fields database_multiple_documents_fields;
  unsigned char vrf_public_key[crypto_vrf_PUBLICKEYBYTES];
  unsigned char vrf_secret_key[crypto_vrf_SECRETKEYBYTES];
  unsigned char vrf_proof[crypto_vrf_PROOFBYTES];
  unsigned char vrf_beta[crypto_vrf_OUTPUTBYTES];
  pthread_t thread_id;
  int settings2 = 1;
  char* transactions[5];

  // define macros
  #define RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST 40
  #define GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA "{\r\n  \"id\": \"0\",\r\n  \"jsonrpc\": \"2.0\",\r\n  \"result\": {\r\n    \"blockhashing_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"blocktemplate_blob\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"difficulty\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"expected_reward\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"height\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"prev_hash\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"reserved_offset\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA,\r\n    \"status\": \"GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\",\r\n    \"untrusted\": GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA\r\n  }\r\n}"
  #define DATA1 "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
  #define DATA2 "[{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"},{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}]" 
  #define NETWORK_BLOCK "0d0da5d5f1e00500000000000000000000000000000000000000000000000000000000000000050000000002b5d9ab0101fff9d8ab0101b2cce199a30202b1ae08c48f3b3e9ba6e22d9fdaf289eda8565179ebff7787883ecaf49f1ebdfbd81801159a7ed6a1065b708355d900b06e4e1c47238397723f4d379945b3bcdf10f09702b4187c424c4f434b434841494e5f52455345525645445f42595445535f53544152547c64656c65676174656e616d65317c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5843413176313851736635504b4c723847467231346a486b6a6766336d506d314d4156627377427339515037467747544c434534537759693831425270327672635631326d614d744377395445314e5a525679796e51336532633362376d785277337c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c307c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c64656c65676174655f6261636b75705f312c64656c65676174655f6261636b75705f322c64656c65676174655f6261636b75705f332c64656c65676174655f6261636b75705f342c64656c65676174655f6261636b75705f357c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c64656c65676174656e616d65327c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c58434131675463557337443761486f32756f533239474445706a4d576b52357a7a514a43337a78437742315231674d4c594263736d70414267575a394d5776764c57585633516868595a39376d63787356383854665a725032564d416d6152514a347c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c307c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c64656c65676174655f6261636b75705f312c64656c65676174655f6261636b75705f322c64656c65676174655f6261636b75705f332c64656c65676174655f6261636b75705f342c64656c65676174655f6261636b75705f357c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c64656c65676174656e616d65337c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c584341313036674d4a6a6e59706b4c70533768546f6544375443625531796464475263427a7173416b35425447327643776e4d7536504c5a6232385a36644543566853587038374d374865505369624c444b4777363433696a4b6e4d774c303536327c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c307c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c64656c65676174655f6261636b75705f312c64656c65676174655f6261636b75705f322c64656c65676174655f6261636b75705f332c64656c65676174655f6261636b75705f342c64656c65676174655f6261636b75705f357c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf67237c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf67237c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5b951935c5066c51f1beef7295aba6856ef5a1c85a2ca14c98a4f8745626a1313a2efab73f452334f3c36aac626b4c12296b1d84925b61831ebeba96b464da6158455118474e7dbeb0234680fba587097c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c310f13ef35d48748ad7057fe55861912fc153221b10633ebdb79460db8dbaa5a2a82796702b2eeef6b3355ebcbb1b8f65a62837d9bb5f64383722ac20f308e5c7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c747275657c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c05f1c69a16ac265377454395bd133c9072b59fa3e0cd652f6f8e8d55fbff19927c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035363536676635676866696a64696f76644a4450494f53677a663534683477363532343567643573313032316430673635316466383467643531363531663635643467353836343136643566345a36643534673635643473363534673635346673646734357c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c7f78db0d006639548c88fa9685ba3d0fa64e626d426c37e28ad442f74cb80e39a5ed9b4fb397f01bfbc8f54dd8aea7590d0408ae433d4d111eaef0ed97b9074f608bbb1722abaafa8734616ce7e4350b7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c414079410cb5ed29a0f331a79cbf898d62b1f94c4d4752d4b9519668ada04f2201a277d60f09f8e50a5e4b0342aafdbd718a0761da52f3f7d3f0a38cc8cb05737c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c747275657c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf67237c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035363536676635676866696a64696f76644a4450494f53677a663534683477363532343567643573313032316430673635316466383467643531363531663635643467353836343136643566345a36643534673635643473363534673635346673646734357c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477cf7b2033fae04e087bd39478c6e5795bbf4f68a51c79bb6d7c085f86770b57a59ef8e61441ac6f67efa6c5f8286a02922f3fa91fd0d74d036462cc9e9f53ee0f4c8e63375b2b48af034591ba864ee260d7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c0162fc911133879a42732ae5dc6bec484e0a8ad77cba4a8b6af260c84f6c6e42af3d21caee35b78a8a89ea7d88efd877724fc6d493059f4824dd40e866f4941a7c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c747275657c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c747275657c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c00000000000000000000000000000000000000000000000000000000000000057c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5369675631346d3250624c4832597436693672546734574d6a5a4b3842324850624e69347a47464d56534534724362673444634a6e3138486b734e5a58513936557451615272766639757532794a6f376571437a5a57326a6641566e597c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c536967563151615254743652347737365a4c67534a58444d71684561506d4b6b3371414b4e7665524b34486578716e45755557776255626254373241524a683371376a5a7a614345486b54787246593570794d61476942595557426d517c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c5369675631616d716e55546263386448656243535a6b77414362445435434b6e6345555070594743786b4a317861586f393451794b586b685037634258327a616b796434466e7a34617757724834374c3878524e366553376d6f3942727c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c53696756316436315035577774726a54527a4237784850735436676a61476844763539596a6a52477066413863626d38576667687570634b6d637258387171444e43396a4646334b6d46527a7a675845336b36474c314d796d467439627c424c4f434b434841494e5f444154415f5345474d454e545f535452494e477c536967563155483236544230354133446441634659574355706d4850394863456e5a577731635645543973576e7a717130353644313175393773374e51694659524b34644866695774624d53614361394d31504d41705854784834564d7c424c4f434b434841494e5f52455345525645445f42595445535f454e447c000500000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000300000000000000000000000000000000000000000000000000000000000000040000000000000000000000000000000000000000000000000000000000000005"
  #define NETWORK_BLOCK_HEIGHT "2813049"
  #define BLOCK_REWARD_TRANSACTION_VERSION "02"
  #define BLOCK_REWARD_INPUT "01"
  #define VIN_TYPE "ff"
  #define BLOCK_REWARD_OUTPUT "01"
  #define STEALTH_ADDRESS_OUTPUT_TAG "02"
  #define TRANSACTION_PUBLIC_KEY_TAG "01"
  #define EXTRA_NONCE_TAG "02"
  #define RINGCT_VERSION "00"
  #define VARINT_ENCODED_VALUE_1 0xb2f58199a302 // random value
  #define VARINT_DECODED_VALUE_1 78167177906 // random value
  #define DATA_HASH_TEXT "X-CASH Proof Of Stake"
  #define DATABASE_COLLECTION "XCASH_PROOF_OF_STAKE_TEST_DATA"
  #define MESSAGE "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"}"
  #define MESSAGE_SETTINGS "{\"message_settings\": \"XCASH_PROOF_OF_STAKE_DATA\"}"
  #define DATABASE_COLLECTION_STATISTICS_DATA "{\"username\":\"XCASH\",\"most_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_total_rounds\":\"5\",\"best_block_verifier_online_percentage_delegate_name\":\"DELEGATE_NAME\",\"best_block_verifier_online_percentage\":\"10\",\"most_block_producer_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_block_producer_total_rounds\":\"15\",\"most_VRF_node_public_and_private_key_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_public_and_private_key_total_rounds\":\"5\",\"most_VRF_node_random_data_total_rounds_delegate_name\":\"DELEGATE_NAME\",\"most_VRF_node_random_data_total_rounds\":\"10\",\"total_XCASH_proof_of_stake_rounds\":\"15\",\"total_coins_in_proof_of_stake\":\"5\",\"total_circulating_supply_percentage_in_proof_of_stake\":\"10\"}"
  #define DATABASE_COLLECTION_DELEGATES_DATA "{\"public_address\":\"XCA\",\"password\":\"XCA\",\"salt\":\"XCA\",\"session\":\"XCA\",\"total_vote_count\":\"XCA\",\"current_vote_count\":\"XCA\",\"delegate_number\":\"XCA\",\"IP_address\":\"127.0.0.1\",\"delegate_name\":\"XCA\",\"about\":\"XCA\",\"website\":\"XCA\",\"team\":\"XCA\",\"pool_mode\":\"XCA\",\"fee_structure\":\"XCA\",\"server_settings\":\"XCA\",\"block_producer_eligibility\":\"XCA\",\"online_status\":\"XCA\",\"block_verifier_total_rounds\":\"XCA\",\"block_verifier_online_total_rounds\":\"XCA\",\"block_verifier_online_percentage\":\"XCA\",\"block_producer_total_rounds\":\"XCA\",\"VRF_node_public_and_private_key_total_rounds\":\"XCA\",\"VRF_node_random_data_total_rounds\":\"XCA\",\"block_producer_block_heights\":\"XCA\",\"VRF_node_public_and_private_key_block_heights\":\"XCA\",\"VRF_node_random_data_block_heights\":\"XCA\"}"

  if (process_id_file == NULL)
  {
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
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

  for (count = 0; count < 5; count++)
  {
    transactions[count] = (char*)calloc(BUFFER_SIZE,sizeof(char));
  }

  // reset the variables
  memset(&string1_test,0,sizeof(string1_test)); 
  memset(string2_test,0,strnlen(string2_test,BUFFER_SIZE)); 
  count_test = 0;  
  
  // get the process_id_file
  memcpy(process_id_file,"/proc/",6);
  sprintf(process_id_file+6,"%d",getpid());
  memcpy(process_id_file+strnlen(process_id_file,BUFFER_SIZE),"/status",7);

  // initialize the arrays
  for (count = 0; count < 5; count++)
  {
    data[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
    settings[count] = (char*)calloc(BUFFER_SIZE,sizeof(char)); 

    if (data[count] == NULL || settings[count] == NULL)
    {
      color_print("Could not allocate the memory needed on the heap","red");
      exit(0);
    }
  }  

  // create the the arrays
  for (count = 0; count < 5; count++)
  {
    sprintf(data[count],"%ld",count);
    sprintf(settings[count],"%ld",count);
  }

  // write the start test message
  color_print(TEST_OUTLINE,"blue");
  printf("\033[1;34mReset variables allocated on the heap test - Total test: %d\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
  color_print(TEST_OUTLINE,"blue");
  printf("\n");

  // run the test

  // varint_decode  
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for varint_decode: %zu / 1000",count);
      fprintf(stderr,"\r");
      varint_decode(VARINT_ENCODED_VALUE_1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_decode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! varint_decode has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // varint_encode  
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for varint_encode: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      varint_encode(VARINT_DECODED_VALUE_1,result_test);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_encode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_encode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! varint_encode has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! varint_encode has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // network_block_string_to_blockchain_data  
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for network_block_string_to_blockchain_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      // reset the blockchain_data
      memset(blockchain_data.network_version_data,0,strnlen(blockchain_data.network_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.timestamp_data,0,strnlen(blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.previous_block_hash_data,0,strnlen(blockchain_data.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.nonce_data,0,strnlen(blockchain_data.nonce_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.block_reward_transaction_version_data,0,strnlen(blockchain_data.block_reward_transaction_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.unlock_block_data,0,strnlen(blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.block_reward_input_data,0,strnlen(blockchain_data.block_reward_input_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.vin_type_data,0,strnlen(blockchain_data.vin_type_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.block_height_data,0,strnlen(blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.block_reward_output_data,0,strnlen(blockchain_data.block_reward_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.block_reward_data,0,strnlen(blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.stealth_address_output_tag_data,0,strnlen(blockchain_data.stealth_address_output_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.stealth_address_output_data,0,strnlen(blockchain_data.stealth_address_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.extra_bytes_size_data,0,strnlen(blockchain_data.extra_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.transaction_public_key_tag_data,0,strnlen(blockchain_data.transaction_public_key_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.transaction_public_key_data,0,strnlen(blockchain_data.transaction_public_key_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.extra_nonce_tag_data,0,strnlen(blockchain_data.extra_nonce_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.reserve_bytes_size_data,0,strnlen(blockchain_data.reserve_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,BUFFER_SIZE));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));  
      memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      for (count2 = 0; count2 < BLOCK_VALIDATION_NODES_TOTAL_AMOUNT; count2++)
      {
        memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count2],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
        memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count2],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
      }
      memset(blockchain_data.ringct_version_data,0,strnlen(blockchain_data.ringct_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.transaction_amount_data,0,strnlen(blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      for (count2 = 0; count2 < MAXIMUM_TRANSACATIONS_PER_BLOCK; count2++)
      {
        memset(blockchain_data.transactions[count2],0,strnlen(blockchain_data.transactions[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
      }

      network_block_string_to_blockchain_data(NETWORK_BLOCK,NETWORK_BLOCK_HEIGHT);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! network_block_string_to_blockchain_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! network_block_string_to_blockchain_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! network_block_string_to_blockchain_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! network_block_string_to_blockchain_data has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // verify_network_block_data  
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for verify_network_block_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      verify_network_block_data(0,0,0,NETWORK_BLOCK_HEIGHT);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_network_block_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_network_block_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_network_block_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! verify_network_block_data has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // verify_network_block_data  
  // read the current system memory usage
  if (settings2 == 1)
  {
    // reset the blockchain_data
    memset(blockchain_data.network_version_data,0,strnlen(blockchain_data.network_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.timestamp_data,0,strnlen(blockchain_data.timestamp_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.previous_block_hash_data,0,strnlen(blockchain_data.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.nonce_data,0,strnlen(blockchain_data.nonce_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.block_reward_transaction_version_data,0,strnlen(blockchain_data.block_reward_transaction_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.unlock_block_data,0,strnlen(blockchain_data.unlock_block_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.block_reward_input_data,0,strnlen(blockchain_data.block_reward_input_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.vin_type_data,0,strnlen(blockchain_data.vin_type_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.block_height_data,0,strnlen(blockchain_data.block_height_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.block_reward_output_data,0,strnlen(blockchain_data.block_reward_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.block_reward_data,0,strnlen(blockchain_data.block_reward_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.stealth_address_output_tag_data,0,strnlen(blockchain_data.stealth_address_output_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.stealth_address_output_data,0,strnlen(blockchain_data.stealth_address_output_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.extra_bytes_size_data,0,strnlen(blockchain_data.extra_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.transaction_public_key_tag_data,0,strnlen(blockchain_data.transaction_public_key_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.transaction_public_key_data,0,strnlen(blockchain_data.transaction_public_key_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.extra_nonce_tag_data,0,strnlen(blockchain_data.extra_nonce_tag_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.reserve_bytes_size_data,0,strnlen(blockchain_data.reserve_bytes_size_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1,0,strlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_1));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_1,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_1,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_2,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_2,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_public_key_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_round_part_3,BUFFER_SIZE));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_proof_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data_round_part_3,BUFFER_SIZE_NETWORK_BLOCK_DATA));  
    memset(blockchain_data.blockchain_reserve_bytes.vrf_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.vrf_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,0,strnlen(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    for (count2 = 0; count2 < BLOCK_VALIDATION_NODES_TOTAL_AMOUNT; count2++)
    {
      memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count2],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature_data[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
      memset(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count2],0,strnlen(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    }
    memset(blockchain_data.ringct_version_data,0,strnlen(blockchain_data.ringct_version_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    memset(blockchain_data.transaction_amount_data,0,strnlen(blockchain_data.transaction_amount_data,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    for (count2 = 0; count2 < MAXIMUM_TRANSACATIONS_PER_BLOCK; count2++)
    {
      memset(blockchain_data.transactions[count2],0,strnlen(blockchain_data.transactions[count2],BUFFER_SIZE_NETWORK_BLOCK_DATA));
    }

    // create the block data
    memcpy(blockchain_data.network_version_data,NETWORK_VERSION,4);
    blockchain_data.timestamp = 1545366181;
    memcpy(blockchain_data.previous_block_hash_data,"0000000000000000000000000000000000000000000000000000000000000005",64);
    memcpy(blockchain_data.nonce_data,BLOCK_PRODUCER_NETWORK_BLOCK_NONCE,8);
    memcpy(blockchain_data.block_reward_transaction_version_data,BLOCK_REWARD_TRANSACTION_VERSION,2);
    blockchain_data.unlock_block = 2813109;
    memcpy(blockchain_data.block_reward_input_data,BLOCK_REWARD_INPUT,2);
    memcpy(blockchain_data.vin_type_data,VIN_TYPE,2);
    blockchain_data.block_height = 2813049;
    memcpy(blockchain_data.block_reward_output_data,BLOCK_REWARD_OUTPUT,2);
    blockchain_data.block_reward = 78168745522;
    memcpy(blockchain_data.stealth_address_output_tag_data,STEALTH_ADDRESS_OUTPUT_TAG,2);
    memcpy(blockchain_data.stealth_address_output_data,"b1ae08c48f3b3e9ba6e22d9fdaf289eda8565179ebff7787883ecaf49f1ebdfb",64);
    memcpy(blockchain_data.transaction_public_key_tag_data,TRANSACTION_PUBLIC_KEY_TAG,2);
    memcpy(blockchain_data.transaction_public_key_data,"159a7ed6a1065b708355d900b06e4e1c47238397723f4d379945b3bcdf10f097",64);
    memcpy(blockchain_data.extra_nonce_tag_data,EXTRA_NONCE_TAG,2);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_delegates_name,"delegatename1",13);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_public_address,"XCA1v18Qsf5PKLr8GFr14jHkjgf3mPm1MAVbswBs9QP7FwGTLCE4SwYi81BRp2vrcV12maMtCw9TE1NZRVyynQ3e2c3b7mxRw3",XCASH_WALLET_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_node_backup_count,"0",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_producer_backup_nodes_names,"delegate_backup_1,delegate_backup_2,delegate_backup_3,delegate_backup_4,delegate_backup_5",89);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_delegates_name,"delegatename2",13);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_public_address,"XCA1gTcUs7D7aHo2uoS29GDEpjMWkR5zzQJC3zxCwB1R1gMLYBcsmpABgWZ9MWvvLWXV3QhhYZ97mcxsV88TfZrP2VMAmaRQJ4",XCASH_WALLET_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_node_backup_count,"0",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_public_and_secret_key_backup_nodes_names,"delegate_backup_1,delegate_backup_2,delegate_backup_3,delegate_backup_4,delegate_backup_5",89);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_delegates_name,"delegatename3",13);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_public_address,"XCA106gMJjnYpkLpS7hToeD7TCbU1yddGRcBzqsAk5BTG2vCwnMu6PLZb28Z6dECVhSXp87M7HePSibLDKGw643ijKnMwL0562",XCASH_WALLET_LENGTH);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_node_backup_count,"0",1);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_node_random_data_backup_nodes_names,"delegate_backup_1,delegate_backup_2,delegate_backup_3,delegate_backup_4,delegate_backup_5",89);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_1,"7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf6723",64);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_1,"7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf6723",64);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_1,"5b951935c5066c51f1beef7295aba6856ef5a1c85a2ca14c98a4f8745626a1313a2efab73f452334f3c36aac626b4c12296b1d84925b61831ebeba96b464da6158455118474e7dbeb0234680fba58709",160);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_1,"310f13ef35d48748ad7057fe55861912fc153221b10633ebdb79460db8dbaa5a2a82796702b2eeef6b3355ebcbb1b8f65a62837d9bb5f64383722ac20f308e5c",128);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_2,"05f1c69a16ac265377454395bd133c9072b59fa3e0cd652f6f8e8d55fbff1992",64);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_2,"30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035363536676635676866696a64696f76644a4450494f53677a663534683477363532343567643573313032316430673635316466383467643531363531663635643467353836343136643566345a3664353467363564347336353467363534667364673435",328);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_2,"7f78db0d006639548c88fa9685ba3d0fa64e626d426c37e28ad442f74cb80e39a5ed9b4fb397f01bfbc8f54dd8aea7590d0408ae433d4d111eaef0ed97b9074f608bbb1722abaafa8734616ce7e4350b",160);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_2,"414079410cb5ed29a0f331a79cbf898d62b1f94c4d4752d4b9519668ada04f2201a277d60f09f8e50a5e4b0342aafdbd718a0761da52f3f7d3f0a38cc8cb0573",128);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_public_key_data_round_part_3,"7c73a5031d645004add8a0360f3a01302cd7e0a1125842e50b7deca371cf6723",64);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_alpha_string_data_round_part_3,"30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303035363536676635676866696a64696f76644a4450494f53677a663534683477363532343567643573313032316430673635316466383467643531363531663635643467353836343136643566345a3664353467363564347336353467363534667364673435",328);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_proof_data_round_part_3,"f7b2033fae04e087bd39478c6e5795bbf4f68a51c79bb6d7c085f86770b57a59ef8e61441ac6f67efa6c5f8286a02922f3fa91fd0d74d036462cc9e9f53ee0f4c8e63375b2b48af034591ba864ee260d",160);
    memcpy(blockchain_data.blockchain_reserve_bytes.vrf_beta_string_data_round_part_3,"0162fc911133879a42732ae5dc6bec484e0a8ad77cba4a8b6af260c84f6c6e42af3d21caee35b78a8a89ea7d88efd877724fc6d493059f4824dd40e866f4941a",128);
    memcpy(blockchain_data.blockchain_reserve_bytes.previous_block_hash_data,"0000000000000000000000000000000000000000000000000000000000000005",64);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[0],"SigV14m2PbLH2Yt6i6rTg4WMjZK8B2HPbNi4zGFMVSE4rCbg4DcJn18HksNZXQ96UtQaRrvf9uu2yJo7eqCzZW2jfAVnY",93);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[1],"SigV1QaRTt6R4w76ZLgSJXDMqhEaPmKk3qAKNveRK4HexqnEuUWwbUbbT72ARJh3q7jZzaCEHkTxrFY5pyMaGiBYUWBmQ",93);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[2],"SigV1amqnUTbc8dHebCSZkwACbDT5CKncEUPpYGCxkJ1xaXo94QyKXkhP7cBX2zakyd4Fnz4awWrH47L8xRN6eS7mo9Br",93);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[3],"SigV1d61P5WwtrjTRzB7xHPsT6gjaGhDv59YjjRGpfA8cbm8WfghupcKmcrX8qqDNC9jFF3KmFRzzgXE3k6GL1MymFt9b",93);
    memcpy(blockchain_data.blockchain_reserve_bytes.block_validation_node_signature[4],"SigV1UH26TB05A3DdAcFYWCUpmHP9HcEnZWw1cVET9sWnzqq056D11u97s7NQiFYRK4dHfiWtbMSaCa9M1PMApXTxH4VM",93);
    memcpy(blockchain_data.ringct_version_data,RINGCT_VERSION,2);
    blockchain_data.transaction_amount = 5;
    memcpy(blockchain_data.transactions[0],"0000000000000000000000000000000000000000000000000000000000000001",64);
    memcpy(blockchain_data.transactions[1],"0000000000000000000000000000000000000000000000000000000000000002",64);
    memcpy(blockchain_data.transactions[2],"0000000000000000000000000000000000000000000000000000000000000003",64);
    memcpy(blockchain_data.transactions[3],"0000000000000000000000000000000000000000000000000000000000000004",64);
    memcpy(blockchain_data.transactions[4],"0000000000000000000000000000000000000000000000000000000000000005",64);
  
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for blockchain_data_to_network_block_string: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      blockchain_data_to_network_block_string(data_test);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! blockchain_data_to_network_block_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! blockchain_data_to_network_block_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! blockchain_data_to_network_block_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! blockchain_data_to_network_block_string has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // create_database_connection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for create_database_connection: %zu / 1000",count);
      fprintf(stderr,"\r");
      mongoc_client_destroy(database_client);
      create_database_connection();
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! create_database_connection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! create_database_connection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! create_database_connection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! create_database_connection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // insert_document_into_collection_array 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for insert_document_into_collection_array: %zu / 1000",count);
      fprintf(stderr,"\r");
      insert_document_into_collection_array(DATABASE_NAME,DATABASE_COLLECTION,data,settings,5);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_array has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_array has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_array has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! insert_document_into_collection_array has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // insert_document_into_collection_json 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for insert_document_into_collection_json: %zu / 1000",count);
      fprintf(stderr,"\r");
      insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_json has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_json has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! insert_document_into_collection_json has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! insert_document_into_collection_json has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // update_document_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for update_document_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      update_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,MESSAGE_SETTINGS,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! update_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! update_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! update_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! update_document_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // read_document_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for read_document_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      read_document_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! read_document_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }  
      }
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }


  // read_document_field_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for read_document_field_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      read_document_field_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE_SETTINGS,"message_settings",data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_field_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_field_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_field_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! read_document_field_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);  
  
  // read_document_all_fields_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for read_document_all_fields_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      for (count2 = 0; count2 < 14; count2++)
      {
        memset(database_data.item[count2],0,strnlen(database_data.item[count2],BUFFER_SIZE));
        memset(database_data.value[count2],0,strnlen(database_data.value[count2],BUFFER_SIZE));
      }
      read_document_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"{\"username\":\"XCASH\"}",&database_data,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! read_document_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! read_document_all_fields_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }

  for (count2 = 0; count2 < 14; count2++)
  {
    memset(database_data.item[count2],0,strnlen(database_data.item[count2],BUFFER_SIZE));
    memset(database_data.value[count2],0,strnlen(database_data.value[count2],BUFFER_SIZE));
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



  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_STATISTICS_DATA,0);
  
  // read_multiple_documents_all_fields_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for read_multiple_documents_all_fields_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      for (count2 = 0; count2 < 2; count2++)
      {
        for (counter = 0; counter < 14; counter++)
        {
          memset(database_multiple_documents_fields.item[count2][counter],0,strnlen(database_multiple_documents_fields.item[count2][counter],BUFFER_SIZE));
          memset(database_multiple_documents_fields.value[count2][counter],0,strnlen(database_multiple_documents_fields.value[count2][counter],BUFFER_SIZE));
        }
      }
      read_multiple_documents_all_fields_from_collection(DATABASE_NAME,DATABASE_COLLECTION,"",&database_multiple_documents_fields,1,2,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! read_multiple_documents_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! read_multiple_documents_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! read_multiple_documents_all_fields_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! read_multiple_documents_all_fields_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }

  for (count2 = 0; count2 < 2; count2++)
  {
    for (counter = 0; counter < 14; counter++)
    {
      memset(database_multiple_documents_fields.item[count2][counter],0,strnlen(database_multiple_documents_fields.item[count2][counter],BUFFER_SIZE));
      memset(database_multiple_documents_fields.value[count2][counter],0,strnlen(database_multiple_documents_fields.value[count2][counter],BUFFER_SIZE));
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



  // update_all_documents_from_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for update_all_documents_from_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      update_all_documents_from_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! update_all_documents_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! update_all_documents_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! update_all_documents_from_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! update_all_documents_from_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // count_documents_in_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for count_documents_in_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      count_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,MESSAGE,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! count_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! count_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! count_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! count_documents_in_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // count_all_documents_in_collection 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for count_all_documents_in_collection: %zu / 1000",count);
      fprintf(stderr,"\r");
      count_all_documents_in_collection(DATABASE_NAME,DATABASE_COLLECTION,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! count_all_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! count_all_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! count_all_documents_in_collection has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! count_all_documents_in_collection has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }


  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);
  insert_document_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,DATABASE_COLLECTION_DELEGATES_DATA,0);

  

  // get_block_template 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_block_template: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_block_template(data_test,"0",0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_template has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_block_template has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // verify_blockchain_network_transactions
  // read the current system memory usage
  if (settings2 == 1)
  {
    memcpy(transactions[0],"f6458824e54ea5cddd80a6bb0105ecdd6d2248629482df2c0f989db3d46f6ebd",64);
    memcpy(transactions[1],"871eb7b29c72582572041c597ff092143031bfcef5fa1fa92808dacab2ba226f",64);
    memcpy(transactions[2],"72f97600db9d7522a2a39fc690d25e1cc9a17535064b08f81bd7424a51bba931",64);
    memcpy(transactions[3],"b180489867776c7e39c07ddb0d88609dac6f748dec554e2d96b6a168785bdb44",64);
    memcpy(transactions[4],"",64);
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for verify_blockchain_network_transactions: %zu / 1000",count);
      fprintf(stderr,"\r");
      verify_blockchain_network_transactions(transactions,5,0,0);  
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_blockchain_network_transactions has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_blockchain_network_transactions has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_blockchain_network_transactions has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! verify_blockchain_network_transactions has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_current_block_height 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_current_block_height: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_current_block_height(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_current_block_height has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_current_block_height has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_block_settings 
  sscanf(data_test, "%zu", &count);
  count--;
  memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
  sprintf(data_test,"%zu",count);
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_block_settings: %zu / 1000",count);
      fprintf(stderr,"\r");
      get_block_settings(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_settings has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_settings has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_block_settings has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_block_settings has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_previous_block_hash 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_previous_block_hash: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      get_previous_block_hash(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! get_previous_block_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! get_previous_block_hash has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // set the current_round_part and current_round_part_backup_node
  memset(current_round_part,0,strnlen(current_round_part,BUFFER_SIZE));
  memset(current_round_part_backup_node,0,strnlen(current_round_part_backup_node,BUFFER_SIZE));
  memcpy(current_round_part,"1",1);
  memcpy(current_round_part_backup_node,"0",1); 

  // sign_data 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for sign_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! sign_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! sign_data has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // verify_data 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for verify_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      verify_data(data_test,0,1,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! verify_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! verify_data has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // set the server_message
  memset(server_message,0,strnlen(server_message,BUFFER_SIZE));
  memcpy(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",30);
  // create the server
  pthread_create(&thread_id, NULL, &create_server_on_separate_thread,NULL);
  pthread_detach(thread_id);
  sleep(1);

  // send_and_receive_data_socket 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for send_and_receive_data_socket: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      memcpy(data_test,"{\r\n \"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\",\r\n}",60);
      sign_data(data_test,0);
      send_and_receive_data_socket(result_test,"127.0.0.1",SEND_DATA_PORT,data_test,TOTAL_CONNECTION_TIME_SETTINGS,"XCASH_PROOF_OF_STAKE_TEST_DATA",0);
      verify_data(result_test,0,1,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      } 
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! send_and_receive_data_socket has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
        else
        {
          color_print("PASSED! send_and_receive_data_socket has reset all variables allocated on the heap","green");
          count_test++;
        }      
      } 
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // get_public_address   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for get_public_address: %zu / 1000",count);
      fprintf(stderr,"\r");
      get_public_address(0);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! get_public_address has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! get_public_address has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! get_public_address has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! get_public_address has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // data_verify   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for data_verify: %zu / 1000",count);
      fprintf(stderr,"\r");
      data_verify(0,"XCA1rWqiYtmHPZssWpCpnsPuQd1uXP91zEVDYkEYdqzb2tDKZCZUaDahWignGvx38ugboroBRRMBS6M3NxDZJEdM5W2KU1BNDK","SigV1AFPX9VxwFgbNmLn2AmZbvG6n342EGfJYbDDZMtFRDkPUZc7bPYriHMYGQ4E996AU64RCsKKqG95X9UXRS1pDETyF","X-CASH");
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! data_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! data_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! data_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! data_verify has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // parse_json_data   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for parse_json_data: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memset(data_test,0,strnlen(data_test,BUFFER_SIZE));
      append_string(result_test,GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA);
      parse_json_data(result_test,"blocktemplate_blob",data_test);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! parse_json_data has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! parse_json_data has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // random_string   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for random_string: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      random_string(result_test,RANDOM_STRING_LENGTH);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! random_string has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! random_string has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }




  // string_count   
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for string_count: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      memcpy(result_test,TEST_OUTLINE,strnlen(TEST_OUTLINE,BUFFER_SIZE));
      string_count(result_test,"-");
      string_count(result_test,"--");
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! string_count has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! string_count has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // string_replace 
  // read the current system memory usage
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for string_replace: %zu / 1000",count);
      fprintf(stderr,"\r");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"string_replace_test","string_replace");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"\"","\\\"");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace_test\",\r\n}");
      string_replace(result_test,"_test","");
      memset(result_test,0,strnlen(result_test,BUFFER_SIZE));
      append_string(result_test,"{\r\n \"message_settings\": \"string_replace\",\r\n}");
      string_replace(result_test,"string_replace_test","");
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 250 && current_memory_usage > 0)
        {
          color_print("FAILED! string_replace has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! string_replace has reset all variables allocated on the heap","green");
          count_test++;
        } 
      }    
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // create_random_VRF_keys 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for create_random_VRF_keys: %zu / 1000",count);
      fprintf(stderr,"\r");
      create_random_VRF_keys((unsigned char*)vrf_public_key,(unsigned char*)vrf_secret_key);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! create_random_VRF_keys has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! create_random_VRF_keys has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! create_random_VRF_keys has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! create_random_VRF_keys has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_vrf_is_valid_key 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_vrf_is_valid_key: %zu / 1000",count);
      fprintf(stderr,"\r");
      count2 = crypto_vrf_is_valid_key((const unsigned char*)vrf_public_key);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_is_valid_key has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_is_valid_key has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_is_valid_key has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_vrf_is_valid_key has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_vrf_keypair_from_seed 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_vrf_keypair_from_seed: %zu / 1000",count);
      fprintf(stderr,"\r");
      crypto_vrf_keypair_from_seed((unsigned char*)vrf_public_key, (unsigned char*)vrf_secret_key, (const unsigned char*)data2);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_keypair_from_seed has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_keypair_from_seed has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_keypair_from_seed has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_vrf_keypair_from_seed has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_vrf_prove 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_vrf_prove: %zu / 1000",count);
      fprintf(stderr,"\r");
      crypto_vrf_prove((unsigned char*)vrf_proof,(const unsigned char*)vrf_secret_key,alpha_string,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_prove has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_prove has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_prove has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_vrf_prove has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_vrf_proof_to_hash 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_vrf_proof_to_hash: %zu / 1000",count);
      fprintf(stderr,"\r");
      crypto_vrf_proof_to_hash((unsigned char*)vrf_beta,(const unsigned char*)vrf_proof);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_proof_to_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_proof_to_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_proof_to_hash has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_vrf_proof_to_hash has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_vrf_verify 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_vrf_verify: %zu / 1000",count);
      fprintf(stderr,"\r");
      count2 = crypto_vrf_verify((unsigned char*)vrf_beta,(const unsigned char*)vrf_public_key,(const unsigned char*)vrf_proof,alpha_string,1);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_vrf_verify has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_vrf_verify has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  // crypto_hash_sha512 
  if (settings2 == 1)
  {
    previous_system_memory_usage = get_program_memory_usage(process_id_file);
    for (count = 0; count <= 1000; count++)
    {
      fprintf(stderr,"Current progress for crypto_hash_sha512: %zu / 1000",count);
      fprintf(stderr,"\r");
      crypto_hash_sha512((unsigned char*)data2,(const unsigned char*)DATA_HASH_TEXT,21);
      if (count == 0)
      {    
        current_memory_usage = get_program_memory_usage(process_id_file) - previous_system_memory_usage;
      }
      if (count == 10)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 9 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_hash_sha512 has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }      
      }
      if (count == 100)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 50 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_hash_sha512 has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }  
      }
      if (count == 1000)
      {
        current_system_memory_usage = get_program_memory_usage(process_id_file);
        if ((current_system_memory_usage - previous_system_memory_usage) > current_memory_usage * 100 && current_memory_usage > 0)
        {
          color_print("FAILED! crypto_hash_sha512 has not reset all variables allocated on the heap","red");
          settings2 = 0;
          break;
        }
        else
        {
          color_print("PASSED! crypto_hash_sha512 has reset all variables allocated on the heap","green");
          count_test++;
        }   
      }  
    }
  }
  else
  {
    color_print("All other test will not be run","red");
  }



  for (count = 0; count < 5; count++)
  {
    pointer_reset(transactions[count]);
  }  

  // write the end test message
  if (count_test == RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST)
  {
    printf("\n");
    color_print(TEST_OUTLINE,"green");
    printf("\033[1;32mReset variables allocated on the heap test - Passed test: %d, Failed test: 0\033[0m\n",RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST);
    color_print(TEST_OUTLINE,"green");
    printf("\n\n");
  }
  else
  {
    printf("\n");
    color_print(TEST_OUTLINE,"red");
    printf("\033[1;31mReset variables allocated on the heap test - Passed test: %d, Failed test: 1\033[0m\n",count_test);
    color_print(TEST_OUTLINE,"red");
    printf("\n\n");
    exit(0);
  } 
  pointer_reset(process_id_file);
  return count_test;

  #undef RESET_VARAIBLES_ALLOCATED_ON_THE_HEAP_TEST
  #undef NETWORK_BLOCK
  #undef NETWORK_BLOCK_HEIGHT
  #undef BLOCK_REWARD_TRANSACTION_VERSION
  #undef BLOCK_REWARD_INPUT
  #undef VIN_TYPE
  #undef BLOCK_REWARD_OUTPUT
  #undef STEALTH_ADDRESS_OUTPUT_TAG
  #undef TRANSACTION_PUBLIC_KEY_TAG
  #undef EXTRA_NONCE_TAG
  #undef RINGCT_VERSION
  #undef VARINT_ENCODED_VALUE_1
  #undef VARINT_DECODED_VALUE_1
  #undef GET_BLOCK_TEMPLATE_RPC_CALL_TEST_DATA
}