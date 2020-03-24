#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"
#include "initialize_and_reset_structs_define_macros.h"

#include "blockchain_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "block_verifiers_thread_server_functions.h"
#include "block_verifiers_update_functions.h"
#include "database_functions.h"
#include "insert_database_functions.h"
#include "delete_database_functions.h"
#include "count_database_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "organize_functions.h"
#include "string_functions.h"
#include "thread_functions.h"
#include "convert.h"
#include "vrf.h"
#include "crypto_vrf.h"
#include "VRF_functions.h"
#include "sha512EL.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: sync_network_data_nodes_database
Description: Sync all of the network data nodes to the same database data
-----------------------------------------------------------------------------------------------------------
*/

void sync_network_data_nodes_database(void)
{
  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char database_data_hash_majority[DATA_HASH_LENGTH+1];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  size_t count2;
  double network_data_nodes_valid_count;

  // define macros
  #define SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE); \
  color_print("Syncing the reserve proofs database","yellow"); \
  sync_reserve_proofs_database(0,NETWORK_DATA_NODE); \
  color_print("Syncing the reserve bytes database","yellow"); \
  sync_reserve_bytes_database(0,1,NETWORK_DATA_NODE); \
  color_print("Syncing the delegates database","yellow"); \
  sync_delegates_database(0,NETWORK_DATA_NODE); \
  color_print("Syncing the statistics database","yellow"); \
  sync_statistics_database(0,NETWORK_DATA_NODE); \
  color_print("Successfully synced all databases","yellow"); \
  return;

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(database_data_hash_majority,0,sizeof(database_data_hash_majority));

  // set the database to not accept any data
  database_settings = 0;

  // reset the synced_network_data_nodes
  for (count = 0; count < NETWORK_DATA_NODES_AMOUNT; count++)
  {
    synced_network_data_nodes[count] = -1;
  }

  // reset the struct network_data_nodes_sync_database_list
  memset(network_data_nodes_sync_database_list.network_data_node_1_public_address,0,sizeof(network_data_nodes_sync_database_list.network_data_node_1_public_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_1_IP_address,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_1_IP_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash));
  memset(network_data_nodes_sync_database_list.network_data_node_2_public_address,0,sizeof(network_data_nodes_sync_database_list.network_data_node_2_public_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_2_IP_address,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_2_IP_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash));
  memset(network_data_nodes_sync_database_list.network_data_node_3_public_address,0,sizeof(network_data_nodes_sync_database_list.network_data_node_3_public_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_3_IP_address,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_3_IP_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash));
  memset(network_data_nodes_sync_database_list.network_data_node_4_public_address,0,sizeof(network_data_nodes_sync_database_list.network_data_node_4_public_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_4_IP_address,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_4_IP_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash));
  memset(network_data_nodes_sync_database_list.network_data_node_5_public_address,0,sizeof(network_data_nodes_sync_database_list.network_data_node_5_public_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_5_IP_address,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_5_IP_address));
  memset(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,0,sizeof(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash));
  if (production_settings == 0)
  {
    memcpy(network_data_nodes_sync_database_list.network_data_node_1_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,sizeof(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_1_IP_address,NETWORK_DATA_NODE_1_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_1_IP_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_2_public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,sizeof(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_2_IP_address,NETWORK_DATA_NODE_2_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_2_IP_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_3_public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,sizeof(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_3_IP_address,NETWORK_DATA_NODE_3_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_3_IP_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_4_public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,sizeof(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_4_IP_address,NETWORK_DATA_NODE_4_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_4_IP_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_5_public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,sizeof(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_5_IP_address,NETWORK_DATA_NODE_5_IP_ADDRESS,sizeof(NETWORK_DATA_NODE_5_IP_ADDRESS)-1);
  }
  else
  {
    memcpy(network_data_nodes_sync_database_list.network_data_node_1_public_address,NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_1_IP_address,NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_2_public_address,NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_2_IP_address,NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_3_public_address,NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_3_IP_address,NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_4_public_address,NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_4_IP_address,NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_node_5_public_address,NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION)-1);
    memcpy(network_data_nodes_sync_database_list.network_data_nodes_5_IP_address,NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,sizeof(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION)-1);
  }
  
  
  print_start_message(current_date_and_time,current_UTC_date_and_time,"Network data nodes are now checking if all network data nodes databases are synced",data);

  // wait so all network data nodes start at the same time, this way one is not reseting the variables as another one is sending them data
  sync_block_verifiers_seconds(current_date_and_time,current_UTC_date_and_time,20);

  // get if the network data node has the previous blocks reserve bytes, because if not this network data node should not be synced from
  memcpy(data2,"{\"block_height\":\"",17);
  sscanf(current_block_height, "%zu", &count2);
  count2--;
  snprintf(data2+strlen(data2),sizeof(data2)-18,"%zu",count2);
  memcpy(data2+strlen(data2),"\"}",2);
  get_reserve_bytes_database(count2,1);
  memcpy(data,"reserve_bytes_",14);
  snprintf(data+strlen(data),sizeof(data)-15,"%zu",count2);
  if (count_documents_in_collection(database_name,data,data2,1) == 1)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"true",4);
  }
  else
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"false",5);
  }
  memset(data,0,sizeof(data));
  
  // get the database data hash and send it to all other network data nodes
  if ((production_settings == 0 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (production_settings == 1 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    // set your own network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings
    network_data_nodes_sync_database_list.network_data_nodes_1_previous_block_settings = strncmp(data2,"true",BUFFER_SIZE) == 0 ? 1 : 0;

    if (get_database_data_hash(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,database_name,"ALL") == 0)
    {
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\",\r\n \"data_hash\": \"",104);
    memcpy(data+strlen(data),network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH);
    memcpy(data+strlen(data),"\",\r\n \"previous_blocks_reserve_bytes\": \"",39);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    memcpy(data+strlen(data),"\",\r\n}",5);

    // sign_data
    if (sign_data(data) == 0)
    { 
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }

    if (production_settings == 0)
    {
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }
    else
    {
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }    
  }
  else if ((production_settings == 0 && memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (production_settings == 1 && memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    // set your own network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings
    network_data_nodes_sync_database_list.network_data_nodes_2_previous_block_settings = strncmp(data2,"true",BUFFER_SIZE) == 0 ? 1 : 0;

    if (get_database_data_hash(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,database_name,"ALL") == 0)
    {
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\",\r\n \"data_hash\": \"",104);
    memcpy(data+strlen(data),network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH);
    memcpy(data+strlen(data),"\",\r\n \"previous_blocks_reserve_bytes\": \"",39);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    memcpy(data+strlen(data),"\",\r\n}",5);

    // sign_data
    if (sign_data(data) == 0)
    { 
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
     
    if (production_settings == 0)
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }
    else
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }    
  }
  else if ((production_settings == 0 && memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (production_settings == 1 && memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    // set your own network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings
    network_data_nodes_sync_database_list.network_data_nodes_3_previous_block_settings = strncmp(data2,"true",BUFFER_SIZE) == 0 ? 1 : 0;

    if (get_database_data_hash(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,database_name,"ALL") == 0)
    {
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\",\r\n \"data_hash\": \"",104);
    memcpy(data+strlen(data),network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH);
    memcpy(data+strlen(data),"\",\r\n \"previous_blocks_reserve_bytes\": \"",39);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    memcpy(data+strlen(data),"\",\r\n}",5);

    // sign_data
    if (sign_data(data) == 0)
    { 
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }

    if (production_settings == 0)
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }
    else
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }    
  }
  else if ((production_settings == 0 && memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (production_settings == 1 && memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    // set your own network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings
    network_data_nodes_sync_database_list.network_data_nodes_4_previous_block_settings = strncmp(data2,"true",BUFFER_SIZE) == 0 ? 1 : 0;

    if (get_database_data_hash(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,database_name,"ALL") == 0)
    {
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\",\r\n \"data_hash\": \"",104);
    memcpy(data+strlen(data),network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH);
    memcpy(data+strlen(data),"\",\r\n \"previous_blocks_reserve_bytes\": \"",39);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    memcpy(data+strlen(data),"\",\r\n}",5);

    // sign_data
    if (sign_data(data) == 0)
    { 
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }

    if (production_settings == 0)
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }
    else
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }    
  }
  else if ((production_settings == 0 && memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0) || (production_settings == 1 && memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0))
  {
    // set your own network_data_nodes_sync_database_list.network_data_nodes_previous_block_settings
    network_data_nodes_sync_database_list.network_data_nodes_5_previous_block_settings = strncmp(data2,"true",BUFFER_SIZE) == 0 ? 1 : 0;

    if (get_database_data_hash(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,database_name,"ALL") == 0)
    {
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }
    // create the message
    memcpy(data,"{\r\n \"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\",\r\n \"data_hash\": \"",104);
    memcpy(data+strlen(data),network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH);
    memcpy(data+strlen(data),"\",\r\n \"previous_blocks_reserve_bytes\": \"",39);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    memcpy(data+strlen(data),"\",\r\n}",5);

    // sign_data
    if (sign_data(data) == 0)
    { 
      database_settings = 1;
      pthread_cond_broadcast(&thread_settings_lock);
      return;
    }

    if (production_settings == 0)
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }
    else
    {
      send_data_socket(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
      send_data_socket(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION,SEND_DATA_PORT,data,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS);
    }    
  }

  // wait for the network data nodes to process the data
  sleep(NETWORK_DATA_NODES_SYNCHRONIZE_DATABASE_SETTINGS);

  // set the network data nodes sync database settings so that if the network data node takes longer than the amount of time, a block verifier will not sync from them
  network_data_nodes_sync_databases_settings = 0;

  // check if a consensus was reached and get the majority database data hash
  network_data_nodes_valid_count = 0;
  count = 0;
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (count > NETWORK_DATA_NODES_VALID_AMOUNT-1) {memcpy(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH); synced_network_data_nodes[0] = 0;}
  
  count = 0;
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (count > NETWORK_DATA_NODES_VALID_AMOUNT-1) {memcpy(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH); synced_network_data_nodes[1] = 1;}

  count = 0;
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (count > NETWORK_DATA_NODES_VALID_AMOUNT-1) {memcpy(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH); synced_network_data_nodes[2] = 2;}

  count = 0;
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (count > NETWORK_DATA_NODES_VALID_AMOUNT-1) {memcpy(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH); synced_network_data_nodes[3] = 3;}

  count = 0;
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (memcmp(network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) {network_data_nodes_valid_count++; count++;}
  if (count > NETWORK_DATA_NODES_VALID_AMOUNT-1) {memcpy(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH); synced_network_data_nodes[4] = 4;}

  if (network_data_nodes_valid_count / (NETWORK_DATA_NODES_AMOUNT*(NETWORK_DATA_NODES_AMOUNT-1)) < NETWORK_DATA_NODES_VALID_AMOUNT_PERCENTAGE)
  {
    // a consensus could not be reached, sync from the main network data node
    color_print("A majority could not be reached between network data nodes for the database sync. Syncing the database from the main network data node","yellow");
    
    sleep(BLOCK_VERIFIERS_SETTINGS);
    database_settings = 1;
    pthread_cond_broadcast(&thread_settings_lock);

    // if all previous block settings are false then sync from the main network data node. This should not happen unless its the first block of the network   
    if (network_data_nodes_sync_database_list.network_data_nodes_1_previous_block_settings == 1)
    {
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_1_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }      
    }
    else if (network_data_nodes_sync_database_list.network_data_nodes_2_previous_block_settings == 1)
    { 
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_2_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_2_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }
    }
    else if (network_data_nodes_sync_database_list.network_data_nodes_3_previous_block_settings == 1)
    { 
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_3_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_3_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }
    }
    else if (network_data_nodes_sync_database_list.network_data_nodes_4_previous_block_settings == 1)
    { 
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_4_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_4_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }
    }
    else if (network_data_nodes_sync_database_list.network_data_nodes_5_previous_block_settings == 1)
    { 
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_5_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_5_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }
    }
    else
    { 
      if (production_settings == 0 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {      
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_1_IP_ADDRESS);
      }
      else if (production_settings == 1 && memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) != 0)
      {
        SYNC_NETWORK_DATA_NODES(NETWORK_DATA_NODE_1_IP_ADDRESS_PRODUCTION);
      } 
      else
      {
        color_print("Successfully synced all databases","yellow");
        return;
      }
    }
  }



  // check if the network data node is in the majority
  if ((production_settings == 0 && ((memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) ||(memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0))) || (production_settings == 1 && ((memcmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_1_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_2_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_3_database_data_hash,DATA_HASH_LENGTH) == 0) ||(memcmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_4_database_data_hash,DATA_HASH_LENGTH) == 0) || (memcmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 && memcmp(database_data_hash_majority,network_data_nodes_sync_database_list.network_data_nodes_5_database_data_hash,DATA_HASH_LENGTH) == 0))))
  {
    // the network data node is in the majority
    color_print("A majority has been reached and the block verifier is already synced with the majority","yellow");
    network_data_nodes_sync_databases_settings = 1;
    database_settings = 1;
    pthread_cond_broadcast(&thread_settings_lock);
    return;
  }

  // the network data node is not in the majority, sync the database from a network data node that is in the majority
  color_print("The database is not synced with the majority of network data nodes, syncing the database from a random network data node that is in the majority","yellow");
  do
  {
    count = ((int)(rand() % NETWORK_DATA_NODES_AMOUNT));
  } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 || synced_network_data_nodes[count] == -1);
  
  sleep(BLOCK_VERIFIERS_SETTINGS);
  database_settings = 1;
  pthread_cond_broadcast(&thread_settings_lock);

  // sync the databses from one of the synced network data nodes
  color_print("Syncing the reserve proofs database","yellow");
  sync_reserve_proofs_database(0,"synced_network_data_nodes");
  color_print("Syncing the reserve bytes database","yellow");
  sync_reserve_bytes_database(0,1,"synced_network_data_nodes");
  color_print("Syncing the delegates database","yellow");
  sync_delegates_database(0,"synced_network_data_nodes");
  color_print("Syncing the statistics database","yellow");
  sync_statistics_database(0,"synced_network_data_nodes");
  color_print("Successfully synced all databases","yellow");

  network_data_nodes_sync_databases_settings = 1;
  return;

  #undef SYNC_NETWORK_DATA_NODES
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_all_block_verifiers_list
Description: Sync the previous, current and next block verifiers from a network data node. This function is only run at startup, since after that the database is used to get the block verifiers list
-----------------------------------------------------------------------------------------------------------
*/

int sync_all_block_verifiers_list(void)
{
  // Variables
  struct delegates delegates[MAXIMUM_AMOUNT_OF_DELEGATES];
  char message[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  char data3[BUFFER_SIZE];
  int count;
  int count2;
  int total_delegates = 0;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define MESSAGE "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n}"
  #define PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA(settings,block_verifiers_data) \
  memset(data2,0,sizeof(data2)); \
  if (parse_json_data(data3,settings,data2,sizeof(data2)) == 0) \
  { \
    SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not parse the message"); \
  } \
  for (count = 0, count2 = 0; count < total_delegates; count++) \
  { \
    memcpy((block_verifiers_data)[count],&data2[count2],strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) - count2); \
    count2 = strnlen(data2,sizeof(data2)) - strnlen(strstr(data2+count2,"|"),sizeof(data2)) + 1; \
  }

  #define SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_all_block_verifiers_list",29); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  database_settings = 1; \
  pthread_cond_broadcast(&thread_settings_lock); \
  return 0;

  // set the database to not accept any new data
  database_settings = 0;

  memset(message,0,sizeof(message));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  // reset the previous current and next block verifiers list
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,sizeof(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,sizeof(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(current_block_verifiers_list.block_verifiers_public_key[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,sizeof(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_public_key[count],0,sizeof(next_block_verifiers_list.block_verifiers_public_key[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  if (network_data_node_settings == 0)
  {
    if (test_settings == 0)
    {
      print_start_message(current_date_and_time,current_UTC_date_and_time,"Syncing the previous, current and next block verifiers directly from a network data node, to be able to update the database",data2);
    }

    // create the message
    memcpy(message,MESSAGE,sizeof(MESSAGE)-1);

    start:
    if (test_settings == 0)
    {
      color_print("Connecting to a random network data node to get a list of previous, current and next block verifiers","white");
    }
    
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));

    // send the message to a random network data node
    do
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
    } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
  
    // get the current time
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);   
    
    if (test_settings == 0)
    {
      memcpy(data3,"Connecting to network data node ",32);
      memcpy(data3+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
      memcpy(data3+strlen(data3)," and sending NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\n",87);
      strftime(data2,sizeof(data2),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time);
      memcpy(data3+strlen(data3),data2,strnlen(data2,MAXIMUM_BUFFER_SIZE));
     color_print(data3,"white");
    }
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));

    if (send_and_receive_data_socket(data3,sizeof(data3),network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
    {
      memset(data2,0,strlen(data2));
      memcpy(data2,"Could not receive data from network data node ",46);
      memcpy(data2+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],MAXIMUM_BUFFER_SIZE));
      color_print(data2,"red");
      color_print("Connecting to a different network data node\n","white");
      goto start;
    }

    if (verify_data(data3,0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not verify data");
    }

    total_delegates = string_count(data3,"|") / 12;
    
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_name_list",previous_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_public_address_list",previous_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_public_key_list",previous_block_verifiers_list.block_verifiers_public_key);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("previous_block_verifiers_IP_address_list",previous_block_verifiers_list.block_verifiers_IP_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_name_list",current_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_public_address_list",current_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_public_key_list",current_block_verifiers_list.block_verifiers_public_key);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("current_block_verifiers_IP_address_list",current_block_verifiers_list.block_verifiers_IP_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_name_list",next_block_verifiers_list.block_verifiers_name);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_public_address_list",next_block_verifiers_list.block_verifiers_public_address);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_public_key_list",next_block_verifiers_list.block_verifiers_public_key);
    PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA("next_block_verifiers_IP_address_list",next_block_verifiers_list.block_verifiers_IP_address);
    if (test_settings == 0)
    {
      color_print("The previous, current and next block verifiers have been synced from a network data node","green");
    }
  }
  else
  {
    if (test_settings == 0)
    {
      print_start_message(current_date_and_time,current_UTC_date_and_time,"Loading the previous, current and next block verifiers from the database",data2);
    }

    // initialize the delegates struct
    INITIALIZE_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES,"sync_all_block_verifiers_list",data2,current_date_and_time,current_UTC_date_and_time);

    // organize the delegates
    if ((total_delegates = organize_delegates(delegates,DATABASE_COLLECTION)) == 0)
    {
      POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
      SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR("Could not organize the delegates");
    }
    else if (total_delegates > BLOCK_VERIFIERS_TOTAL_AMOUNT)
    {
      total_delegates = BLOCK_VERIFIERS_TOTAL_AMOUNT;
    }

    // copy the delegates to the next, current and previous block verifiers list
    for (count = 0; count < total_delegates; count++)
    {
       memcpy(previous_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(previous_block_verifiers_list.block_verifiers_name[count])));
       memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(previous_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(previous_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(previous_block_verifiers_list.block_verifiers_public_key[count])));
       memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(previous_block_verifiers_list.block_verifiers_IP_address[count])));
       memcpy(current_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(current_block_verifiers_list.block_verifiers_name[count])));
       memcpy(current_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(current_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(current_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(current_block_verifiers_list.block_verifiers_public_key[count])));
       memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(current_block_verifiers_list.block_verifiers_IP_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_name[count],delegates[count].delegate_name,strnlen(delegates[count].delegate_name,sizeof(next_block_verifiers_list.block_verifiers_name[count])));
       memcpy(next_block_verifiers_list.block_verifiers_public_address[count],delegates[count].public_address,strnlen(delegates[count].public_address,sizeof(next_block_verifiers_list.block_verifiers_public_address[count])));
       memcpy(next_block_verifiers_list.block_verifiers_public_key[count],delegates[count].public_key,strnlen(delegates[count].public_key,sizeof(next_block_verifiers_list.block_verifiers_public_key[count])));
       memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],delegates[count].IP_address,strnlen(delegates[count].IP_address,sizeof(next_block_verifiers_list.block_verifiers_IP_address[count])));
    }

    POINTER_RESET_DELEGATES_STRUCT(count,MAXIMUM_AMOUNT_OF_DELEGATES);
    if (test_settings == 0)
    {
      color_print("The previous, current and next block verifiers have been loaded from the database","green");
    }
  }

  // set the database to accept data
  database_settings = 1;

  // reset any thread that was waiting for the database
  pthread_cond_broadcast(&thread_settings_lock);

  return 1;

  #undef DATABASE_COLLECTION  
  #undef MESSAGE
  #undef PARSE_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST_DATA
  #undef SYNC_ALL_BLOCK_VERIFIERS_LIST_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_synced_block_verifiers
Description: Gets the synced block verifiers for syncing the database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_synced_block_verifiers(void)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  int count2;
  int total_delegates = 0;

  // define macros
  #define GET_SYNCED_BLOCK_VERIFIERS_DATA "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}"
  #define PARSE_BLOCK_VERIFIERS_LIST_DATA(settings,block_verifiers_data) \
  memset(data,0,sizeof(data)); \
  if (parse_json_data(data2,settings,data,sizeof(data)) == 0) \
  { \
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not parse the message"); \
  } \
  for (count = 0, count2 = 0; count < total_delegates; count++) \
  { \
    memcpy(block_verifiers_data[count],&data[count2],strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) - count2); \
    count2 = strnlen(data,sizeof(data)) - strnlen(strstr(data+count2,"|"),sizeof(data)) + 1; \
  }

  #define GET_SYNCED_BLOCK_VERIFIERS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_synced_block_verifiers",26); \
  memcpy(error_message.data[error_message.total],settings,sizeof(settings)-1); \
  error_message.total++; \
  return 0;

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_TOTAL_AMOUNT; count++)
  {
    memset(synced_block_verifiers.synced_block_verifiers_IP_address[count],0,sizeof(synced_block_verifiers.synced_block_verifiers_IP_address[count]));
    memset(synced_block_verifiers.vote_settings[count],0,sizeof(synced_block_verifiers.vote_settings[count]));
  }
  synced_block_verifiers.vote_settings_true = 0;
  synced_block_verifiers.vote_settings_false = 0;
  synced_block_verifiers.vote_settings_connection_timeout = 0;

  start:
  if (test_settings == 0)
  {
    color_print("Connecting to a random network data node to get a list of current block verifiers","white");
  }

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));

  // send the message to a random network data node
  do
  {
    count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
  } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
  
  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
  
  if (test_settings == 0)
  {
    memcpy(data,"Connecting to network data node ",32);
    memcpy(data+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data+strlen(data)," and sending NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\n",73);
    strftime(data2,MAXIMUM_BUFFER_SIZE,"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time);
    memcpy(data+strlen(data),data2,strnlen(data2,sizeof(data)));
    color_print(data,"white");
  }
  memset(data,0,strlen(data));
  memset(data2,0,strlen(data2));

  if (send_and_receive_data_socket(data2,sizeof(data2),network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,GET_SYNCED_BLOCK_VERIFIERS_DATA,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
  {
    memcpy(data,"Could not receive data from network data node ",46);
    memcpy(data+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    color_print(data,"red");
    memset(data,0,sizeof(data));
    color_print("Connecting to a different network data node\n","white");
    goto start;
  }

  if (verify_data(data2,0) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not verify data");
  }

  // get the delegate amount
  total_delegates = string_count(data2,"|") / 3;

  PARSE_BLOCK_VERIFIERS_LIST_DATA("block_verifiers_public_address_list",synced_block_verifiers.synced_block_verifiers_public_address);
  PARSE_BLOCK_VERIFIERS_LIST_DATA("block_verifiers_public_key_list",synced_block_verifiers.synced_block_verifiers_public_key);
  PARSE_BLOCK_VERIFIERS_LIST_DATA("block_verifiers_IP_address_list",synced_block_verifiers.synced_block_verifiers_IP_address);
  synced_block_verifiers.last_refresh_time_of_synced_block_verifiers = time(NULL);
  
  return 1;

  #undef GET_SYNCED_BLOCK_VERIFIERS_DATA
  #undef PARSE_BLOCK_VERIFIERS_LIST_DATA
  #undef GET_SYNCED_BLOCK_VERIFIERS_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_verifier_for_syncing_database
Description: Gets a block verifier for syncing the database from
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
  block_verifiers_ip_address - The selected block verifier to sync the database from
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void get_block_verifier_for_syncing_database(int settings, const char* DELEGATES_IP_ADDRESS, char *block_verifiers_ip_address)
{
  // Variables
  int count;
  int count2;
  
  if (memcmp(DELEGATES_IP_ADDRESS,"",1) == 0)
  {
    // check if all block verifiers were not from the majority, and if so sync from a network data node
    for (count = 0, count2 = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0)
      {
        count2++;
      }
    }

    if (count2 == BLOCK_VERIFIERS_AMOUNT)
    {
      settings = 2;
    }

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       select a random network data node to sync from if there was a lot of connection_timeouts, to where a majority vote could not be calculated, there were more than BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT new block verifiers
    */
    
    if (settings == 1)
    {
      do
      {
        count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT);
      } while (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || memcmp(synced_block_verifiers.synced_block_verifiers_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0);
      memcpy(block_verifiers_ip_address,synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
    }
    else if (settings == 2)
    {
      get_random_network_data_node(count);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE));
    }
    else
    {     
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[settings-3],strnlen(network_data_nodes_list.network_data_nodes_IP_address[settings-3],BUFFER_SIZE));
    }
  }
  else
  {
    if (memcmp(DELEGATES_IP_ADDRESS,"synced_network_data_nodes",25) == 0)
    {
      // get a random network data node that is a synced network data node
      do
      {
        count = ((int)(rand() % NETWORK_DATA_NODES_AMOUNT));
      } while (memcmp(network_data_nodes_list.network_data_nodes_public_address[count],xcash_wallet_public_address,XCASH_WALLET_LENGTH) == 0 || synced_network_data_nodes[count] == -1);
      memcpy(block_verifiers_ip_address,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    }
    else
    {
      memcpy(block_verifiers_ip_address,DELEGATES_IP_ADDRESS,strnlen(DELEGATES_IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    }
  }
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_proofs_specific_database
Description: Checks if all of the specific reserve proofs databases need to be synced and syncs them if needed
Paramters:
  DATABASE_DATA - The database data
  block_verifiers_ip_address - The selected block verifier to sync the database from
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_proofs_specific_database(const char* DATABASE_DATA, const char* BLOCK_VERIFIERS_IP_ADDRESS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[SMALL_BUFFER_SIZE];
  char* data3 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;
  
  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data3); \
  data3 = NULL;

  #define SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR(message) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_specific_database",43); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }  
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }  
    memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_specific_database",43);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  memset(data2,0,sizeof(data2));

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    if (test_settings == 0)
    {
      memset(data,0,strlen(data));
      memcpy(data,"Checking if reserve_proofs_",27);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%d",count);
      memcpy(data+strlen(data)," is synced",10);
      color_print(data,"white");
    }

    // parse the database_data
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    memset(data3,0,strlen(data3));
    memcpy(data2,"reserve_proofs_database_",24);
    snprintf(data2+24,sizeof(data2)-25,"%d",count);

    if (parse_json_data(DATABASE_DATA,data2,data,MAXIMUM_BUFFER_SIZE) == 0)
    {
      SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier");
    }
   
    if (memcmp(data,"false",5) == 0)
    {
      // sync the database
      if (test_settings == 0)
      {
        memset(data,0,strlen(data));
        memcpy(data,"reserve_proofs_",15);
        snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%d",count);
        memcpy(data+strlen(data)," is not synced, downloading it from ",36);
        memcpy(data+strlen(data),BLOCK_VERIFIERS_IP_ADDRESS,strnlen(BLOCK_VERIFIERS_IP_ADDRESS,MAXIMUM_BUFFER_SIZE));
        color_print(data,"red");
      }

      // create the message
      memset(data2,0,sizeof(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_proofs_",133);
      snprintf(data2+133,sizeof(data2)-134,"%d",count);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2) == 0)
      { 
        SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR("Could not sign_data");
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,BLOCK_VERIFIERS_IP_ADDRESS,SEND_DATA_PORT,data2,DATABASE_SYNCING_TIMEOUT_SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier");
      }

      if (verify_data(data,0) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR("Could not verify data from the block verifier");
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_proofs_database",data3,MAXIMUM_BUFFER_SIZE) == 0)
      {
        SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier");
      }

      // if the delegate did send data over, but the database is empty then just exit the loop
      if (memcmp(data3,"",1) == 0)
      {
        break;
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);
      snprintf(data2+15,sizeof(data2)-16,"%d",count);

      // delete the collection from the database
      delete_collection_from_database(database_name,data2,1);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(database_name,data2,data,strlen(data),1);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%d",count);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
      color_print(data,"green");
    }
    else
    {
      if (test_settings == 0)
      {
        memset(data,0,strlen(data));
        memcpy(data,"reserve_proofs_",15);
        snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%d",count);
        memcpy(data+strlen(data)," is already synced\n",19);
        color_print(data,"green");
      }
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all  
  #undef SYNC_CHECK_RESERVE_PROOFS_SPECIFIC_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_bytes_specific_database
Description: Checks if all of the specific reserve bytes databases need to be synced and syncs them if needed
Paramters:
  DATABASE_DATA - The database data
  block_verifiers_ip_address - The selected block verifier to sync the database from
  starting_reserve_bytes_database - The starting reserve bytes database
  CURRENT_RESERVE_BYTES_DATABASE - The current reserve bytes database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_specific_database(const char* DATABASE_DATA, const char* BLOCK_VERIFIERS_IP_ADDRESS, size_t starting_reserve_bytes_database, const size_t CURRENT_RESERVE_BYTES_DATABASE)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char* data3 = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  
  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data3); \
  data3 = NULL; \

  #define SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR(message) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_specific_database",42); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }  
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }  
    memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_specific_database",42);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  memset(data2,0,sizeof(data2));

  for (; starting_reserve_bytes_database <= CURRENT_RESERVE_BYTES_DATABASE; starting_reserve_bytes_database++)
  {
    if (test_settings == 0)
    {
      memset(data,0,strlen(data));
      memcpy(data,"Checking if reserve_bytes_",26);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",starting_reserve_bytes_database);
      memcpy(data+strlen(data)," is synced",10);
      color_print(data,"white");
    }

    // parse the database_data
    memset(data,0,strlen(data));
    memset(data2,0,sizeof(data2));
    memset(data3,0,strlen(data3));
    memcpy(data2,"reserve_bytes_database_",23);
    snprintf(data2+23,sizeof(data2)-24,"%zu",starting_reserve_bytes_database);

    if (parse_json_data(DATABASE_DATA,data2,data,MAXIMUM_BUFFER_SIZE) == 0)
    {
      SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier");
    }
   
    if (memcmp(data,"false",5) == 0)
    {
      if (test_settings == 0)
      {
        // sync the database
        memset(data,0,strlen(data));
        memcpy(data,"reserve_bytes_",14);
        snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",starting_reserve_bytes_database);
        memcpy(data+strlen(data)," is not synced, downloading it from ",36);
        memcpy(data+strlen(data),BLOCK_VERIFIERS_IP_ADDRESS,strnlen(BLOCK_VERIFIERS_IP_ADDRESS,MAXIMUM_BUFFER_SIZE));
        color_print(data,"red");
      }

      // create the message
      memset(data2,0,strlen(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_bytes_",131);
      snprintf(data2+131,sizeof(data2)-132,"%zu",starting_reserve_bytes_database);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2) == 0)
      { 
        SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR("Could not sign_data");
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,BLOCK_VERIFIERS_IP_ADDRESS,SEND_DATA_PORT,data2,DATABASE_SYNCING_TIMEOUT_SETTINGS) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier3");
      }

      if (verify_data(data,0) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR("Could not verify data from the block verifier2");
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_bytes_database",data3,MAXIMUM_BUFFER_SIZE) == 0)
      {
        SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR("Could not receive data from the block verifier1");
      }

      // if the delegate did send data over, but the database is empty then just exit the loop
      if (memcmp(data3,"",1) == 0)
      {
        break;
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);
      snprintf(data2+14,sizeof(data2)-15,"%zu",starting_reserve_bytes_database);

      // delete the collection from the database
      delete_collection_from_database(database_name,data2,1);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(database_name,data2,data,strlen(data),1);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",starting_reserve_bytes_database);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
    }
    else
    {
      if (test_settings == 0)
      {
        memset(data,0,strlen(data));
        memcpy(data,"reserve_bytes_",14);
        snprintf(data+strlen(data),MAXIMUM_BUFFER_SIZE-1,"%zu",starting_reserve_bytes_database);
        memcpy(data+strlen(data)," is already synced\n",19);
      }
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all  
  #undef SYNC_CHECK_RESERVE_BYTES_SPECIFIC_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_proofs_database
Description: Syncs the reserve proofs database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, or if it is "synced_network_data_nodes" then it will sync from a random synced network data node, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_proofs_database(int settings, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  int count;
  
  // define macros
  #define SYNC_RESERVE_PROOFS_DATABASE_ERROR(message,data_settings) \
  if ((data_settings) == 0 || (memcmp(DELEGATES_IP_ADDRESS,"",1) != 0 && memcmp(DELEGATES_IP_ADDRESS,"synced_network_data_nodes",1) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    get_random_network_data_node(settings); \
    settings += 3; \
    goto start; \
  } 

  start:

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address);

  // get the database data hash for the reserve proofs database
  if (test_settings == 0)
  {
    memset(data3,0,strlen(data3));
    memcpy(data3,"Getting the database data from ",31);
    memcpy(data3+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,BUFFER_SIZE));
    memcpy(data3+strlen(data3),"\n",1);
    color_print(data3,"white");
  }
  if (get_database_data_hash(data,database_name,"reserve_proofs") == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database from ",1);
  }

  // create the message
  memset(data3,0,strlen(data3));
  memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_proofs_data_hash\": \"",139);
  memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\",\r\n ",5);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memcpy(data3+strlen(data3),"\"reserve_proofs_data_hash_",26);
    snprintf(data3+strlen(data3),MAXIMUM_BUFFER_SIZE-1,"%d",count);
    memcpy(data3+strlen(data3),"\": \"",4);
    // get the database data hash for the reserve proofs database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_proofs_",15);  
    snprintf(data2+15,sizeof(data2)-16,"%d",count);
    if (get_database_data_hash(data,database_name,data2) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database from ",1);
    }
    memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n ",5);
  }
  memcpy(data3+strlen(data3),"}",1);

  // sign_data
  if (sign_data(data3) == 0)
  { 
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data",0);
  }

  // connect to the block verifier and get the database data
  if (send_and_receive_data_socket(database_data,sizeof(database_data),block_verifiers_ip_address,SEND_DATA_PORT,data3,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(database_data,0) == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
  }

  // check if the block verifier needs to sync any of the reserve proof databases and sync them if needed
  if (sync_check_reserve_proofs_specific_database(database_data,(const char*)block_verifiers_ip_address) == 0)
  {
    SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not check if any of the specific databases needed to be synced, from ",1);
  }

  return 1;

  #undef SYNC_RESERVE_PROOFS_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_bytes_database
Description: Syncs the reserve bytes database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  RESERVE_BYTES_START_SETTINGS - 0 to sync all of the reserve bytes databases, 1 to only sync the current reserve bytes database
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_bytes_database(int settings, const int RESERVE_BYTES_START_SETTINGS, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char data[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char data3[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  size_t count;
  size_t count2;
  size_t current_reserve_bytes_database;
  
  // define macros
  #define SYNC_RESERVE_BYTES_DATABASE_ERROR(message,data_settings) \
  if ((data_settings) == 0 || (memcmp(DELEGATES_IP_ADDRESS,"",1) != 0 && memcmp(DELEGATES_IP_ADDRESS,"synced_network_data_nodes",1) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    get_random_network_data_node(settings); \
    settings += 3; \
    goto start; \
  }
  
  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,strlen(data3));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address);

  // get the current reserve bytes database
  get_reserve_bytes_database(current_reserve_bytes_database,0);
  
  // get the database data hash for the reserve bytes database
  if (test_settings == 0)
  {
    memset(data3,0,strlen(data3));
    memcpy(data3,"Getting the database data from ",31);
    memcpy(data3+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,BUFFER_SIZE));
    memcpy(data3+strlen(data3),"\n",1);
    color_print(data3,"white");
  }
  if (get_database_data_hash(data,database_name,"reserve_bytes") == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database from ",1);
  }

  // create the message
  memset(data3,0,strlen(data3));
  memcpy(data3,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_bytes_settings\": \"",136);
  snprintf(data3+strlen(data3),sizeof(data3)-138,"%d",RESERVE_BYTES_START_SETTINGS);
  memcpy(data3+strlen(data3),"\",\r\n \"reserve_bytes_data_hash\": \"",33);
  memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
  memcpy(data3+strlen(data3),"\",\r\n ",5);
  count = RESERVE_BYTES_START_SETTINGS == 0 ? 1 : current_reserve_bytes_database - 1;
  if (count == 0)
  {
    // set it to only check the current reserve bytes database if their is no previous reserve bytes database
    count = 1;
  }

  for (; count <= current_reserve_bytes_database; count++)
  {
    memcpy(data3+strlen(data3),"\"reserve_bytes_data_hash_",25);
    snprintf(data3+strlen(data3),sizeof(data3)-1,"%zu",count);
    memcpy(data3+strlen(data3),"\": \"",4);
    // get the database data hash for the reserve bytes database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_bytes_",14);  
    snprintf(data2+14,sizeof(data2)-15,"%zu",count);
    if (get_database_data_hash(data,database_name,data2) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database from ",1);
    }
    memcpy(data3+strlen(data3),data,DATA_HASH_LENGTH);
    memcpy(data3+strlen(data3),"\",\r\n ",5);
  }
  memcpy(data3+strlen(data3),"}",1);

  // sign_data
  if (sign_data(data3) == 0)
  { 
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
  }

  // connect to the block verifier and get the database data
  if (send_and_receive_data_socket(database_data,sizeof(database_data),block_verifiers_ip_address,SEND_DATA_PORT,data3,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(database_data,0) == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
  }

  count2 = RESERVE_BYTES_START_SETTINGS == 0 ? 1 : current_reserve_bytes_database - 1;
  if (count2 == 0)
  {
    // set it to only check the current reserve bytes database if their is no previous reserve bytes database
    count2 = 1;
  }

  // check if the block verifier needs to sync any of the reserve bytes databases and sync them if needed
  if (sync_check_reserve_bytes_specific_database(database_data,(const char*)block_verifiers_ip_address,count2,(const size_t)current_reserve_bytes_database) == 0)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not check if any of the specific databases needed to be synced, from ",1);
  }
  return 1;
  
  #undef SYNC_RESERVE_BYTES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_delegates_database
Description: Syncs the delegates database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_delegates_database(int settings, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  
  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define MESSAGE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
  #define SYNC_DELEGATES_DATABASE_ERROR(message,data_settings) \
  if ((data_settings) == 0 || (memcmp(DELEGATES_IP_ADDRESS,"",1) != 0 && memcmp(DELEGATES_IP_ADDRESS,"synced_network_data_nodes",1) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset(data); \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    get_random_network_data_node(settings); \
    settings += 3; \
    goto start; \
  } 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);  
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address);

  // get the database data hash for the delegates database
  if (test_settings == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"Getting the database data from ",31);
    memcpy(data2+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,sizeof(data2)));
    memcpy(data2+strlen(data2),"\n",1);
    color_print(data2,"white");
  }
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database from ",1);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,MESSAGE,sizeof(MESSAGE)-1);

  // sign_data
  if (sign_data(data2) == 0)
  { 
    SYNC_DELEGATES_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,block_verifiers_ip_address,SEND_DATA_PORT,data2,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"delegates_database",data2,sizeof(data2)) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(database_name,DATABASE_COLLECTION,1);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_multiple_documents_into_collection_json(database_name,DATABASE_COLLECTION,data,strlen(data),1);

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef SYNC_DELEGATES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_statistics_database
Description: Syncs the statistics database
Paramters:
  settings - 1 to sync from a random block verifier, 2 to sync from a random network data node, otherwise the index of the network data node to sync from + 3
  DELEGATES_IP_ADDRESS - The specific delegates IP address, if you are syncing directly from a delegate, otherwise an empty string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_statistics_database(int settings, const char* DELEGATES_IP_ADDRESS)
{
  // Variables
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char data2[BUFFER_SIZE];
  char database_data[BUFFER_SIZE];
  char block_verifiers_ip_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  
  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define MESSAGE "{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n}"
  #define SYNC_STATISTICS_DATABASE_ERROR(message,data_settings) \
  if ((data_settings) == 0 || (memcmp(DELEGATES_IP_ADDRESS,"",1) != 0 && memcmp(DELEGATES_IP_ADDRESS,"synced_network_data_nodes",1) != 0)) \
  { \
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset(data); \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),block_verifiers_ip_address,strnlen(block_verifiers_ip_address,MAXIMUM_BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    get_random_network_data_node(settings); \
    settings += 3; \
    goto start; \
  } 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    pointer_reset(data);
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data2);
    exit(0);
  }

  start:

  memset(data,0,strlen(data));
  memset(data2,0,sizeof(data2));
  memset(database_data,0,sizeof(database_data));
  memset(block_verifiers_ip_address,0,sizeof(block_verifiers_ip_address));

  // get a block verifier to sync the database from
  get_block_verifier_for_syncing_database(settings,DELEGATES_IP_ADDRESS,block_verifiers_ip_address);

  // get the database data hash for the statistics database
  if (test_settings == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"Getting the database data from ",31);
    memcpy(data2+31,block_verifiers_ip_address,strnlen(block_verifiers_ip_address,sizeof(data2)));
    memcpy(data2+strlen(data2),"\n",1);
    color_print(data2,"white");
  }
  if (get_database_data_hash(data,database_name,DATABASE_COLLECTION) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database from ",1);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,MESSAGE,sizeof(MESSAGE)-1);

  // sign_data
  if (sign_data(data2) == 0)
  { 
    SYNC_STATISTICS_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,MAXIMUM_BUFFER_SIZE,block_verifiers_ip_address,SEND_DATA_PORT,data2,DATABASE_SYNCING_TIMEOUT_SETTINGS) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"statistics_database",data2,sizeof(data2)) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(database_name,DATABASE_COLLECTION,1);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_document_into_collection_json(database_name,DATABASE_COLLECTION,data,1);

  pointer_reset(data);
  return 1;

  #undef DATABASE_COLLECTION
  #undef MESSAGE
  #undef SYNC_STATISTICS_DATABASE_ERROR   
}