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
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"
#include "structures.h"
#include "variables.h"

#include "blockchain_functions.h"
#include "block_verifiers_server_functions.h"
#include "block_verifiers_synchronize_server_functions.h"
#include "block_verifiers_synchronize_functions.h"
#include "database_functions.h"
#include "delegate_server_functions.h"
#include "delegates_website_functions.h"
#include "file_functions.h"
#include "network_daemon_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "network_wallet_functions.h"
#include "server_functions.h"
#include "shared_delegates_website_functions.h"
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
Name: create_server
Description: Creates the server
Parameters:
   MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_server(const int MESSAGE_SETTINGS)
{
  // Constants
  const int SOCKET_OPTION = 1; 

  // Variables
  char buffer[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct sockaddr_in addr; 
  size_t count;
  int settings;

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  #define SERVER_ERROR(message) \
  memcpy(error_message.function[error_message.total],"create_server",13); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  exit(0);  

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);
  
  memset(buffer,0,sizeof(buffer));
  memset(data,0,sizeof(data));

  if (MESSAGE_SETTINGS == 1)
  {
    print_start_message(current_date_and_time,current_UTC_date_and_time,"Creating the server",data);
  }
  
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    SERVER_ERROR("Error creating the socket");
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errors
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errors
  */
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) != 0)
  {
    SERVER_ERROR("Error setting socket options");
  } 

  if (MESSAGE_SETTINGS == 1)
  {   
    color_print("Socket created","green");
  }
 
  // convert the port to a string
  snprintf(buffer,sizeof(buffer)-1,"%d",SEND_DATA_PORT);  
 
  memset(&addr, 0, sizeof(addr));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(SEND_DATA_PORT);
 
  // connect to 0.0.0.0
  if (bind(server_socket, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) != 0)
  {    
    memcpy(error_message.function[error_message.total],"create_server",13);
    memcpy(error_message.data[error_message.total],"Error connecting to port ",25);
    memcpy(error_message.data[error_message.total]+25,buffer,strnlen(buffer,BUFFER_SIZE));
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,data);
    exit(0);
  } 

  // set the socket to non blocking
  settings = fcntl(server_socket, F_GETFL, 0);
  settings |= O_NONBLOCK;
  fcntl(server_socket, F_SETFL, settings);

  // set the maximum simultaneous connections
  if (listen(server_socket, MAXIMUM_CONNECTIONS) != 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  if (MESSAGE_SETTINGS == 1)
  {
    fprintf(stderr,"\033[1;32mConnected to port %s\033[0m\n",buffer);
    fprintf(stderr,"Waiting for a connection...\n\n");
  }

  // create the epoll file descriptor
  epoll_fd = epoll_create1(0);
  if (epoll_fd < 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  /* create the epoll_event struct
  EPOLLIN = signal when the file descriptor is ready to read
  EPOLLET = use edge triggered mode, this will only signal that a file descriptor is ready when that file descriptor changes states
  */  
  events_copy.events = EPOLLIN | EPOLLET;
  events_copy.data.fd = server_socket;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &events_copy) < 0)
  {
    SERVER_ERROR("Error creating the server");
  }

  for (count = 0; (int)count < total_threads-1; count++)
  {
    if (pthread_create(&server_threads[count], NULL, socket_receive_data_thread, NULL) < 0 || pthread_detach(server_threads[count]) != 0)
    {
      SERVER_ERROR("Error creating the server");
    }
  }

  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef SERVER_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: new_socket_thread
Description: new socket thread
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

int new_socket_thread(void)
{
  // Variables
  int client_socket;
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  // int settings;  

  while ((client_socket = accept(server_socket, (struct sockaddr *) &addr, &addrlen)) != -1)
  {
    /*// set the socket to non blocking
    settings = fcntl(client_socket, F_GETFL, 0);
    settings |= O_NONBLOCK;
    fcntl(client_socket, F_SETFL, settings);*/

    /* create the epoll_event struct
    EPOLLIN = signal when the file descriptor is ready to read
    EPOLLET = use edge triggered mode, this will only signal that a file descriptor is ready when that file descriptor changes states
    EPOLLONESHOT = set the socket to only signal its ready once, since were using multiple threads
    */
    events_copy.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    events_copy.data.fd = client_socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &events_copy) < 0)
    {    
      close(client_socket);
      return 0;
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_thread
Description: socket thread
Parameters:
  client_socket - The client socket
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int socket_thread(int client_socket)
{
  // Variables
  char* buffer = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char buffer2[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char client_address[BUFFER_SIZE]; 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int receive_data_result; 
  struct sockaddr_in addr;
  socklen_t addrlength = sizeof(addr);
  
  memset(buffer2,0,sizeof(buffer2));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(client_address,0,sizeof(client_address));

  // convert the port to a string
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 

  // receive the data
  receive_data_result = receive_data(client_socket,buffer,SOCKET_END_STRING,1,TOTAL_CONNECTION_TIME_SETTINGS);
  if (receive_data_result < 2)
  {
    pointer_reset(buffer);
    return 0;
  } 

  // check if the message length is correct for the type of message
  if (strstr(buffer,"GET /") != NULL)
  {
    memcpy(data2,"HTTP ",5);
    memcpy(data2+5,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer," HTTP/"),sizeof(data2)));
  }
  else if (strnlen(buffer,MAXIMUM_BUFFER_SIZE) > 25 && strstr(buffer,"}") != NULL)
  {
    memcpy(data2,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
    if ((strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) == 0) && (strnlen(buffer,MAXIMUM_BUFFER_SIZE) == MAXIMUM_BUFFER_SIZE))
    {
      pointer_reset(buffer);
      return 0;
    }
  }
  else if (strstr(buffer,"|") != NULL)
  {
    memcpy(data2,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer,"|"),sizeof(data2)));
    if (strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 && strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0)
    {
      pointer_reset(buffer);
      return 0;
    }
  }
  else
  {
    pointer_reset(buffer);
    return 0;
  }

  if (getpeername(client_socket, (struct sockaddr *) &addr, &addrlength) != 0)
  {
    pointer_reset(buffer);
    return 0;
  }
  
  // get the IP address or the reverse DNS name
  pthread_mutex_lock(&lock);
  if (getnameinfo((struct sockaddr *)&addr, addrlength, client_address, sizeof(client_address), NULL, 0, 0) != 0)
  {
    getnameinfo((struct sockaddr *)&addr, addrlength, client_address, sizeof(client_address), NULL, 0, NI_NUMERICHOST);
  }
  pthread_mutex_unlock(&lock); 
  
  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);
  
  memcpy(message,"Received ",9);
  memcpy(message+9,data2,strnlen(data2,sizeof(message)));
  memcpy(message+strlen(message),"\n",1);
  memcpy(message+strlen(message),client_address,strnlen(client_address,sizeof(message)));
  memcpy(message+strlen(message)," on port ",9);
  memcpy(message+strlen(message),buffer2,strnlen(buffer2,sizeof(message)));
  memcpy(message+strlen(message),"\n",1);
  memset(data2,0,sizeof(data2));
  strftime(data2,sizeof(data2),"%a %d %b %Y %H:%M:%S UTC\n",&current_UTC_date_and_time);
  memcpy(message+strlen(message),data2,strnlen(data2,sizeof(message)));
  color_print(message,"green");

 // check if a certain type of message has been received 
 if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL)
 {
   server_received_data_XCASH_DPOPS_test_data(client_socket,buffer);
 }
 else if (strstr(buffer,"GET /delegateswebsitegetstatistics HTTP/") != NULL && delegates_website == 1)
 {
   server_receive_data_socket_delegates_website_get_statistics(client_socket);
 } 
 else if (strstr(buffer,"GET /getdelegates HTTP/") != NULL && delegates_website == 1)
 {
   server_receive_data_socket_get_delegates(client_socket);
 } 
 else if (strstr(buffer,"GET /getdelegatesstatistics?parameter1=") != NULL && delegates_website == 1)
 {
   server_receive_data_socket_get_delegates_statistics(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /getdelegatesinformation?parameter1=") != NULL && delegates_website == 1)
 {
   server_receive_data_socket_get_delegates_information(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /getdelegatesvoterslist?parameter1=") != NULL && (delegates_website == 1 || shared_delegates_website == 1))
 {
   server_receive_data_socket_get_delegates_voters_list(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /getroundstatistics?parameter1=") != NULL && delegates_website == 1)
 {
   server_receive_data_socket_get_round_statistics(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /shareddelegateswebsitegetstatistics HTTP/") != NULL && shared_delegates_website == 1)
 {
   server_receive_data_socket_shared_delegates_website_get_statistics(client_socket);
 } 
 else if (strstr(buffer,"GET /getblocksfound HTTP/") != NULL && shared_delegates_website == 1)
 {
   server_receive_data_socket_get_blocks_found(client_socket);
 } 
 else if (strstr(buffer,"GET /getpublicaddressinformation?public_address=") != NULL && shared_delegates_website == 1)
 {
   server_receive_data_socket_get_public_address_information(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"GET /getpublicaddresspaymentinformation?public_address=") != NULL && shared_delegates_website == 1)
 {
   server_receive_data_socket_get_public_address_payment_information(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
 {
   server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(client_socket);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\"") != NULL && network_data_node_settings == 1)
 {
   server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(client_socket);
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket);
 }
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES\"") != NULL)
 {
   server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_update(client_socket,(const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(client_socket,(const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(client_socket,(const char*)buffer);
 }
 else if (strstr(buffer,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") != NULL)
 {
   server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
 }  
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_register_delegates(client_socket,(const char*)buffer);
 }            
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_remove_delegates(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") != NULL)
 {
   server_receive_data_socket_nodes_to_block_verifiers_update_delegates(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\"") != NULL)
 {
   server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\"") != NULL && main_network_data_node_create_block == 1)
 {  
   server_receive_data_socket_main_network_data_node_to_block_verifier_start_block((const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\"") != NULL)
 {  
   server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block(client_socket,(const char*)buffer);
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && current_UTC_date_and_time.tm_sec >= 10 && current_UTC_date_and_time.tm_sec < 30)
 {
   server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
 }         
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && current_UTC_date_and_time.tm_sec < 10)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && current_UTC_date_and_time.tm_sec >= 30 && current_UTC_date_and_time.tm_sec < 40)
 {
   server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
 }  
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && ((current_UTC_date_and_time.tm_sec >= 45 && current_UTC_date_and_time.tm_sec < 55) || (current_UTC_date_and_time.tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time.tm_sec >= 30 && current_UTC_date_and_time.tm_sec < 40)))
 { 
   server_receive_data_socket_node_to_node((const char*)buffer);
 }
 else if (strstr(buffer,"GET /") != NULL)
 {
   server_receive_data_socket_get_files(client_socket,(const char*)buffer);
 }
 if (error_message.total != 0)
 {
   print_error_message(current_date_and_time,current_UTC_date_and_time,data2);
 }
pointer_reset(buffer);
return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_receive_data_thread
Description: socket receive data thread
-----------------------------------------------------------------------------------------------------------
*/

void* socket_receive_data_thread(void* parameters)
{
  // Constants
  const int CONNECTIONS_PER_THREAD = MAXIMUM_CONNECTIONS / total_threads;
  
  // Variables
  struct epoll_event events[CONNECTIONS_PER_THREAD];
  int count;
  int count2;

  // unused parameters
  (void)parameters;

  // define macros
  #define SOCKET_RECEIVE_DATA_THREAD_ERROR(message) \
  memcpy(error_message.function[error_message.total],"socket_receive_data_thread",26); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  pthread_exit((void *)(intptr_t)1);

  /* get the events that have a ready signal
  set the timeout settings to -1 to wait until any file descriptor is ready
  */ 
 for (;;)
 { 
   count = epoll_wait(epoll_fd, events, CONNECTIONS_PER_THREAD, -1);
   for (count2 = 0; count2 < count; count2++)
   {    
     if (events[count2].events & EPOLLERR || events[count2].events & EPOLLHUP || !(events[count2].events & EPOLLIN))
     {  
       close(events[count2].data.fd);
     }
     else if (events[count2].data.fd == server_socket)
     {
       // a file descriptor is ready for a new socket connection
       new_socket_thread();
     }
     else
     {
       // a file descriptor is ready for a current socket connection
      socket_thread(events[count2].data.fd);
      epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[count2].data.fd, &events_copy);
      close(events[count2].data.fd);
     }
   } 
}
pthread_exit((void *)(intptr_t)1);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_get_files
Description: gets the files
Parameters:
  CLIENT_SOCKET - The socket to send data to
  MESSAGE - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_get_files(const int CLIENT_SOCKET, const char* MESSAGE)
{
  // Variables
  unsigned char* data = (unsigned char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(unsigned char));
  char data2[BUFFER_SIZE];
  long file_size;
  int settings = 0;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR \
  memcpy(data,"{\"Error\":\"Could not get the file\"}",34); \
  send_data(CLIENT_SOCKET,data,strlen((const char*)data),400,"application/json"); \
  pointer_reset(data); \
  return 0;

  memset(data2,0,sizeof(data2));

  // get the file
  if (shared_delegates_website == 1)
  {
    memcpy(data2,"../shared_delegates_website/",28);
  }
  if (delegates_website == 1)
  {
    memcpy(data2,"../delegates_website/",21);
  }
  memcpy(data2+strlen(data2),&MESSAGE[(strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5],(strlen(MESSAGE) - strlen(strstr(MESSAGE," HTTP/"))) - ((strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5));
  file_size = read_file(data,data2);
  if (file_size == 0)
  {
    if (shared_delegates_website == 1)
    {
      file_size = read_file(data,"../shared_delegates_website/index.html");
    }
    if (delegates_website == 1)
    {
      file_size = read_file(data,"../delegates_website/index.html");
    }    
    settings = 1;
    if (file_size == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
    }
  }

  memset(data2,0,sizeof(data2));

  if (strstr(MESSAGE,".html") != NULL || settings == 1)
  {
    memcpy(data2,"text/html; charset=utf-8",24);
  }
  else if (strstr(MESSAGE,".js") != NULL)
  {
    memcpy(data2,"application/javascript; charset=utf-8",37);
  }
  else if (strstr(MESSAGE,".css") != NULL)
  {
    memcpy(data2,"text/css; charset=utf-8",23);
  }
  else if (strstr(MESSAGE,".png") != NULL)
  {
    memcpy(data2,"image/png",9);
  }
  else if (strstr(MESSAGE,".jpg") != NULL || strstr(MESSAGE,".jpeg") != NULL)
  {
    memcpy(data2,"image/jpeg",10);
  }
  else if (strstr(MESSAGE,".ico") != NULL)
  {
    memcpy(data2,"image/x-icon",12);
  }
  else
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  } 

  if (send_data(CLIENT_SOCKET,data,(const long)file_size,200,data2) == 0)
  {
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  }

  pointer_reset(data);
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR
}