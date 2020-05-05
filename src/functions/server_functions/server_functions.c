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
#include "read_database_functions.h"
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
  // Variables
  char buffer[MAXIMUM_NUMBER_SIZE];
  char data[SMALL_BUFFER_SIZE];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct sockaddr_in addr; 
  int count;
  int settings;

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  #define SERVER_ERROR(message) \
  memcpy(error_message.function[error_message.total],"create_server",13); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  exit(0); 
  
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
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    SERVER_ERROR("Error creating the socket");
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errors
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errors
  */
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1},sizeof(int)) != 0)
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
  addr.sin_addr.s_addr = memcmp(XCASH_DPOPS_delegates_IP_address,"127.0.0.1",9) == 0 ? INADDR_ANY : inet_addr(XCASH_DPOPS_delegates_IP_address);
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
    memcpy(data,"Connected to port ",18);
    memcpy(data+18,buffer,strnlen(buffer,sizeof(data)));
    memcpy(data+strlen(data),"\nWaiting for a connection...\n",29);
    color_print(data,"green");
    memset(data,0,sizeof(data));
  }

  // create the epoll file descriptor
  if ((epoll_fd = epoll_create1(0)) < 0)
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

  for (count = 0; count < total_threads-1; count++)
  {
    if (pthread_create(&server_threads[count], NULL, socket_receive_data_thread, NULL) < 0 || pthread_detach(server_threads[count]) != 0)
    {
      SERVER_ERROR("Error creating the server");
    }
    break;
  }

  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef SERVER_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: new_socket_thread
Description: new socket thread
-----------------------------------------------------------------------------------------------------------
*/

void new_socket_thread(void)
{
  // Variables
  int client_socket;
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(struct sockaddr_in);
  struct epoll_event epevent;

  while ((client_socket = accept(server_socket, (struct sockaddr *) &addr, &addrlen)) != -1)
  {
    /* create the epoll_event struct
    EPOLLIN = signal when the file descriptor is ready to read
    EPOLLET = use edge triggered mode, this will only signal that a file descriptor is ready when that file descriptor changes states
    EPOLLONESHOT = set the socket to only signal its ready once, since were using multiple threads
    */
    epevent.events = EPOLLIN | EPOLLET;
    epevent.data.fd = client_socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &epevent) < 0)
    {    
      close(client_socket);
      return;
    }
  }
  return;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_limit_public_addresses
Description: limits connections based on registered public addresses to the server
Parameters:
  SETTINGS - 1 to run the check before running the server code, 0 to remove the IP address so it can make another connection, 2 to not verify any of the previous block hash or current round part
  MESSAGE - The message
  Return: 0 if there is multiple connections, 1 if there is a single connection
-----------------------------------------------------------------------------------------------------------
*/

int server_limit_public_addresses(const int SETTINGS, const char* MESSAGE)
{
  if (test_settings == 1)
  {
    return 1;
  }

  // Variables
  char data[SMALL_BUFFER_SIZE];
  char data2[SMALL_BUFFER_SIZE];
  char data3[SMALL_BUFFER_SIZE];
  int count;
  int count2;
  long long int number;
  size_t data_size;

  // define macros
  #define DATABASE_COLLECTION "delegates"

  memset(data,0,sizeof(data));
  memset(data2,0,sizeof(data2));
  memset(data3,0,sizeof(data3));

  if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK") != NULL)
  {
    color_print("data,"green");
  }

  // parse the data
  if (SETTINGS == 1 || SETTINGS == 3)
  {
    if (parse_json_data(MESSAGE,"public_address",data2,sizeof(data2)) == 0)
    { 
      return 0;
    }    
  }
  else if (SETTINGS == 2 || SETTINGS == 4)
  {
    for (count = 0, count2 = 0; count < GET_RESERVE_BYTES_DATABASE_HASH_PARAMETER_AMOUNT; count++)
    {
      if (count == 2)
      {
        if ((data_size = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) - count2) != XCASH_WALLET_LENGTH)
        {
          return 0;
        }
        memcpy(data2,&MESSAGE[count2],data_size);
        if (strncmp(data2,XCASH_WALLET_PREFIX,sizeof(XCASH_WALLET_PREFIX)-1) != 0)
        {
          return 0;
        }
        break;
      }
      count2 = strlen(MESSAGE) - strlen(strstr(MESSAGE+count2,"|")) + 1;
    }
  }

  if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK") != NULL)
  {
    color_print(MESSAGE,"yellow");
    color_print(data2,"yellow");
  }

  memcpy(data,"|",1);
  memcpy(data+1,data2,XCASH_WALLET_LENGTH); 

  // start data
  if (SETTINGS == 1 || SETTINGS == 2)
  {
    // verify the message
    if ((SETTINGS == 1 && verify_data(MESSAGE,0) == 0) || (SETTINGS == 2 && verify_data(MESSAGE,0) == 0))
    { 
      return 0;
    }

    // create the message
    memcpy(data3,"{\"public_address\":\"",19);
    memcpy(data3+19,data2,strnlen(data2,XCASH_WALLET_LENGTH));
    memcpy(data3+strlen(data3),"\"}",2);

    // dont check any amount if its coming from a seed node
    if (strncmp(OFFICIAL_SHARED_DELEGATE_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0 || strncmp("XCA1gxReAqF6BHQ8Hs2ne9WYZ6U7Nkq5GK1iRKgzypdn9FdT8JBoKmdD2oAcPgFxA6Ww4WHAJJWqdEfZDwqhFkUQ1foW6fTYgv",data2,BUFFER_SIZE) == 0 || strncmp("XCA1Xb8YQxS728H3oVfeDogpfijhRYTzpMSn7hTWL8NA1oVSoaCb1VMVBbdw9QEq4rHeVVCsnqy7gS2tBLjGZyRe6hcwcicfFC",data2,BUFFER_SIZE) == 0 || strncmp("XCA1Sy6f8UyZ1KBQm56PtvemNhSJJPdb5gywzpcj8TkJTa6AcuZ8kiniKdTUYXfH1zQgWpHDPrXgLdGN6wp5JYcwAUXiTFYj6o",data2,BUFFER_SIZE) == 0 || strncmp("XCA1aMFTo5B5qV8EC8Q9BuTXe6A4pcSphWjPcSZiDEGYToNDhHfN1FvAY5NXSdpoJg3ZFaMjB492DfHLgnFvYnwS5uyyMESMg8",data2,BUFFER_SIZE) == 0 || strncmp("XCA1nbYJfF4b8WvQs6PJiqBVTUN9Y6n4M1z7CmqX4g1CMjrWSea4DYBdSRmUHJm1SUakaZfpqYyb8WX5pPqGpmrr9qByNSCutz",data2,BUFFER_SIZE) == 0 || strncmp("XCA1q3eAJpMcV7u9cx2cnGQv9cT7DGWHFWAPAZpxseAoYQy9HTNXc7XNvniCRQpQnEKMLGvcTFutbZQib5THCty93rquAbjJZW",data2,BUFFER_SIZE) == 0 || strncmp("XCA1XUjHdwdTvoE17Sz5WkNkMJcesfuZoWx8zS3Q6DvtDtz8djGDJeFWGAf4DaeCYR8eY4UmUnrLW2voYfW7zUQK8B2DgwAEFu",data2,BUFFER_SIZE) == 0 || strncmp("XCA1hFcupxhVkTTH22anrz1UQpa41dYTHVDHkgctQtDT1YZUAcbrwQaYpPBV9HshcA49xi19AGfTzKttLaYaxeeu9HTDA6VAG5",data2,BUFFER_SIZE) == 0 || strncmp("XCA1rnKveKL4RCoc3Rby9vXApXXxqmVZRFHVxhvmrmcNBbDxe9ksVHnGdPm5TrS667JJ9mjaCa3VhHX98N4bZHkn1y4tjyDEfR",data2,BUFFER_SIZE) == 0 || strncmp("XCA1pBLeqme3FqrsgMdzgiZrEtshjYYYhU6LLvphhaqqZ5jTe2uy3zPJDGEQe9PiksPXCK8YmRTHJBBJUsmBHH5i5nc73QowtH",data2,BUFFER_SIZE) == 0 || strncmp("XCA1m9Zn8vsGq41omr7tYNXBVhZGpMnWLK8wbZGKVRj6V1UZ9DzUeuPTmuoWPB2jVU3r7EQmKfWV4LKW92GWJh3pA1iGEqomZg",data2,BUFFER_SIZE) == 0 || strncmp("XCA1WdNDNrDHPZgZUJPFF49WyHovKmpJDRMD1ui3aeD2Pu8LtUY3vUfBZHknK1x3JZ78RNpmEVsvjgeJrzGu7vks2sZjW536Sa",data2,BUFFER_SIZE) == 0 || strncmp("",data2,BUFFER_SIZE) == 0 || strncmp("XCA1t6mbscMeao1Eacy3Hg6mE8aq8cjrraT4bB9eGxQbFo4MAJjBAPf1Jhrbi829nTBgAYtJAUgoXbQYocpcBbWu84Gs1GpRkz",data2,BUFFER_SIZE) == 0 || strncmp("XCA1t4AjSMaQo9EvvxhjsgRvz32ii4QweCDx6s32WuGYEFJVnS9vXWmKGfUJpR8Ng5PNh8kUA9UfeCm2kPu8WQ8n5nZ3V15tNn",data2,BUFFER_SIZE) == 0 || strncmp("XCA1pKHPMmF1KSYGdBJ2SNfrzANHtZ6aF7eDFyHSTRi653rbBnnFm7uMijexoG4djPC4T9X83q4Jbgjwxya6UUaE4MkxmkD5kJ",data2,BUFFER_SIZE) == 0 || strncmp("XCA1UArNZKkEYz6qArKgtyJimMBw5xkjE5ec33ymywWU1XvXGzZKWcqWKMjtqwfRdiNeQaeZyJJ7aZfXk3C5TMos4RfueH7ER8",data2,BUFFER_SIZE) == 0 || strncmp("XCA1cC6xQza9AUkL9Cvby49PWSr2aDMfVQi2aaNa5UsJTVddWdn7yJvBcqG2ewnDh3Ee38qWJkEdW52J96zT1C6f7awCAMYxpG",data2,BUFFER_SIZE) == 0 || strncmp("XCA1dJ2TSfwfAeaSTMSzLL83YDnNs6e1mHxkVdx8tdigMSCqqAGDjpi3bV1LCrjHQKh4N1s7kPE3bSoeh4ypwgsj1SRtfhP14K",data2,BUFFER_SIZE) == 0 || strncmp("XCA1sCezaPUYxBAiUXS6xd9kvyNxCcqA65vspMCPAB78Gjjef9E6S7FQ47yFkNA2RdCwybAk2XWhPDT9F1t6c4Jg5JkLy2Wb1p",data2,BUFFER_SIZE) == 0 || strncmp("XCA1gFTg7iiZGJa8RmvT81H66iyLcffBtTYKCRMmnnfLd4yAWcKG4khYD4ESmWGvy6MZZSaTYcvRYC6Exkgd475g3WaoU1tWcY",data2,BUFFER_SIZE) == 0)
    {
      
    }
    else if (strncmp(TEST_WALLET_1,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_2,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_3,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_4,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_5,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_6,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_7,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_8,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_9,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_10,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_11,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_12,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_13,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_14,data2,BUFFER_SIZE) == 0 || strncmp(TEST_WALLET_15,data2,BUFFER_SIZE) == 0 || strncmp("XCA1pKHPMmF1KSYGdBJ2SNfrzANHtZ6aF7eDFyHSTRi653rbBnnFm7uMijexoG4djPC4T9X83q4Jbgjwxya6UUaE4MkxmkD5kJ",data2,BUFFER_SIZE) == 0 || strncmp("XCA1UArNZKkEYz6qArKgtyJimMBw5xkjE5ec33ymywWU1XvXGzZKWcqWKMjtqwfRdiNeQaeZyJJ7aZfXk3C5TMos4RfueH7ER8",data2,BUFFER_SIZE) == 0 || strncmp("XCA1cC6xQza9AUkL9Cvby49PWSr2aDMfVQi2aaNa5UsJTVddWdn7yJvBcqG2ewnDh3Ee38qWJkEdW52J96zT1C6f7awCAMYxpG",data2,BUFFER_SIZE) == 0 || strncmp("XCA1dJ2TSfwfAeaSTMSzLL83YDnNs6e1mHxkVdx8tdigMSCqqAGDjpi3bV1LCrjHQKh4N1s7kPE3bSoeh4ypwgsj1SRtfhP14K",data2,BUFFER_SIZE) == 0 || strncmp("XCA1sCezaPUYxBAiUXS6xd9kvyNxCcqA65vspMCPAB78Gjjef9E6S7FQ47yFkNA2RdCwybAk2XWhPDT9F1t6c4Jg5JkLy2Wb1p",data2,BUFFER_SIZE) == 0 || strncmp("XCA1gFTg7iiZGJa8RmvT81H66iyLcffBtTYKCRMmnnfLd4yAWcKG4khYD4ESmWGvy6MZZSaTYcvRYC6Exkgd475g3WaoU1tWcY",data2,BUFFER_SIZE) == 0)
    {
      
    }
    else if ((production_settings == 0 && strncmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS,data2,BUFFER_SIZE) == 0) || (production_settings == 0 && strncmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS,data2,BUFFER_SIZE) == 0) || (production_settings == 0 && strncmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS,data2,BUFFER_SIZE) == 0) || (production_settings == 0 && strncmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS,data2,BUFFER_SIZE) == 0) || (production_settings == 0 && strncmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS,data2,BUFFER_SIZE) == 0) || (production_settings == 1 && strncmp(NETWORK_DATA_NODE_1_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0) || (production_settings == 1 && strncmp(NETWORK_DATA_NODE_2_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0) || (production_settings == 1 && strncmp(NETWORK_DATA_NODE_3_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0) || (production_settings == 1 && strncmp(NETWORK_DATA_NODE_4_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0) || (production_settings == 1 && strncmp(NETWORK_DATA_NODE_5_PUBLIC_ADDRESS_PRODUCTION,data2,BUFFER_SIZE) == 0))
    {
      if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK") != NULL)
      {
        color_print(MESSAGE,"yellow");
        color_print(data2,"yellow");
      }
    }
    else
    {
      if (strstr(MESSAGE,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK") != NULL)
      {
        color_print(MESSAGE,"yellow");
        color_print(data2,"yellow");
      }
      // check to make sure that the public address is registered to a delegate and the delegate has the DATABASE_DATA_SYNC_DELEGATE_MINIMUM_AMOUNT
      memset(data2,0,sizeof(data2));
      if (read_document_field_from_collection(database_name,DATABASE_COLLECTION,data3,"total_vote_count",data2) == 1)
      {
        sscanf(data2, "%lld", &number);
        if ((production_settings == 1 && number < DATABASE_DATA_SYNC_DELEGATE_MINIMUM_AMOUNT) || (production_settings == 0 && number < 0))
        {
          return 0;
        }
      }
      else
      {
        return 0;
      }
    }
    
    pthread_mutex_lock(&database_data_IP_address_lock);
    if (string_count(server_limit_public_address_list,data) > MAXIMUM_CONNECTIONS_IP_ADDRESS_OR_PUBLIC_ADDRESS)
    {
      pthread_mutex_unlock(&database_data_IP_address_lock);
      return 0;
    }
    else
    {
      memcpy(server_limit_public_address_list+strlen(server_limit_public_address_list),data,strnlen(data,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    }  
    pthread_mutex_unlock(&database_data_IP_address_lock);
    return 1;
  }
  else if (SETTINGS == 3 || SETTINGS == 4)
  {
    pthread_mutex_lock(&database_data_IP_address_lock);
    string_replace_limit(server_limit_public_address_list,15728640,data,"",1);
    pthread_mutex_unlock(&database_data_IP_address_lock);
    return 1;
  }
  return 0;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_limit_IP_addresses
Description: limits connections based on IP addresses to the server
Parameters:
  SETTINGS - 1 to run the check before running the server code, 0 to remove the IP address so it can make another connection
  IP_ADDRESS - The IP address
  Return: 0 if there is multiple connections, 1 if there is a single connection
-----------------------------------------------------------------------------------------------------------
*/

int server_limit_IP_addresses(const int SETTINGS, const char* IP_ADDRESS)
{
  if (test_settings == 1)
  {
    return 1;
  }

  // Variables
  char data[SMALL_BUFFER_SIZE];

  memset(data,0,sizeof(data));

  memcpy(data,"|",1);
  memcpy(data+1,IP_ADDRESS,strnlen(IP_ADDRESS,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH)); 

  // start data
  if (SETTINGS == 1)
  {
    pthread_mutex_lock(&database_data_IP_address_lock);
    if (string_count(server_limit_IP_address_list,data) >= MAXIMUM_CONNECTIONS_IP_ADDRESS_OR_PUBLIC_ADDRESS)
    {
      pthread_mutex_unlock(&database_data_IP_address_lock);
      return 0;
    }
    else
    {
      memcpy(server_limit_IP_address_list+strlen(server_limit_IP_address_list),data,strnlen(data,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    }  
    pthread_mutex_unlock(&database_data_IP_address_lock);
    return 1;
  }
  else if (SETTINGS == 0)
  {
    pthread_mutex_lock(&database_data_IP_address_lock);
    string_replace_limit(server_limit_IP_address_list,15728640,data,"",1);
    pthread_mutex_unlock(&database_data_IP_address_lock);
    return 1;
  }
  return 0;

  #undef DATABASE_COLLECTION
}



/*
-----------------------------------------------------------------------------------------------------------
Name: socket_thread
Description: socket thread
Parameters:
  client_socket - The client socket
-----------------------------------------------------------------------------------------------------------
*/

void socket_thread(int client_socket)
{
  // Constants
  const size_t MAXIMUM_AMOUNT = database_data_socket_settings == 1 ? MAXIMUM_BUFFER_SIZE : BUFFER_SIZE;

  // Variables
  char* buffer = (char*)calloc(MAXIMUM_AMOUNT+1,sizeof(char));
  char buffer2[BUFFER_SIZE];
  char data2[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  char client_address[BUFFER_SIZE]; 
  char client_IP_address[BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH];
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct sockaddr_in addr;
  socklen_t addrlength = sizeof(addr);
  
  memset(buffer2,0,sizeof(buffer2));
  memset(data2,0,sizeof(data2));
  memset(message,0,sizeof(message));
  memset(client_address,0,sizeof(client_address));
  memset(client_IP_address,0,sizeof(client_IP_address));

  // convert the port to a string
  snprintf(buffer2,sizeof(buffer2)-1,"%d",SEND_DATA_PORT); 

  // receive the data
  if (receive_data(client_socket,buffer,MAXIMUM_AMOUNT,1,SEND_OR_RECEIVE_SOCKET_DATA_TIMEOUT_SETTINGS) < 2)
  {
    pointer_reset(buffer);
    return;
  }

  // check if the message length is correct for the type of message
  if (strstr(buffer,"POST /") != NULL || strstr(buffer,"PUT /") != NULL || strstr(buffer,"PATCH /") != NULL || strstr(buffer,"DELETE /") != NULL)
  {
    pointer_reset(buffer);
    return;
  }

  // validate the message
  if (validate_data(buffer) == 0)
  {
    pointer_reset(buffer);
    return;
  }

  if (strstr(buffer,"GET /") != NULL)
  {
    memcpy(data2,"HTTP ",5);
    memcpy(data2+5,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer," HTTP/"),sizeof(data2)));
  }
  else if (strlen(buffer) > 25 && strstr(buffer,"}") != NULL)
  {
    memcpy(data2,&buffer[25],strlen(buffer) - strlen(strstr(buffer,"\",\r\n")) - 25);
    if ((strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA",BUFFER_SIZE) == 0 || strncmp(data2,"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE",BUFFER_SIZE) == 0 || strncmp(data2,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) == 0) && (strlen(buffer) >= MAXIMUM_BUFFER_SIZE))
    {
      pointer_reset(buffer);
      return;
    }
  }
  else if (strstr(buffer,"|") != NULL)
  {
    memcpy(data2,buffer,strnlen(buffer,sizeof(data2)) - strnlen(strstr(buffer,"|"),sizeof(data2)));
    if (strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE",BUFFER_SIZE) != 0 && strncmp(data2,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) != 0 && strncmp(data2,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_REMOVE_DELEGATE",BUFFER_SIZE) != 0 && strncmp(data2,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE",BUFFER_SIZE) != 0)
    {
      pointer_reset(buffer);
      return;
    }
  }
  else
  {
    pointer_reset(buffer);
    return;
  }
    
  // get the IP address
  pthread_mutex_lock(&lock);
  if (getpeername(client_socket, (struct sockaddr *) &addr, &addrlength) != 0 || getnameinfo((struct sockaddr *)&addr, addrlength, client_IP_address, sizeof(client_IP_address), NULL, 0, NI_NUMERICHOST) != 0)
  {
    pointer_reset(buffer);
    return;
  }
  pthread_mutex_unlock(&lock); 
  
  // get the current time
  get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

  // dont display the message if the message came from the test wallet, or your own wallet
  if (debug_settings == 1 && test_settings == 0)
  {  
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
  }
 


 // check if a certain type of message has been received 
 if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_received_data_XCASH_DPOPS_test_data(client_socket,buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 }
 else if (strstr(buffer,"GET /delegateswebsitegetstatistics HTTP/") != NULL && delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_delegates_website_get_statistics(client_socket);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getdelegates HTTP/") != NULL && delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_delegates(client_socket);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getdelegatesstatistics?parameter1=") != NULL && delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_delegates_statistics(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getdelegatesinformation?parameter1=") != NULL && delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_delegates_information(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getdelegatesvoterslist?parameter1=") != NULL && (delegates_website == 1 || shared_delegates_website == 1))
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_delegates_voters_list(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getroundstatistics?parameter1=") != NULL && delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_round_statistics(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /shareddelegateswebsitegetstatistics HTTP/") != NULL && shared_delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_shared_delegates_website_get_statistics(client_socket);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getblocksfound HTTP/") != NULL && shared_delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_blocks_found(client_socket);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getpublicaddressinformation?public_address=") != NULL && shared_delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_public_address_information(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"GET /getpublicaddresspaymentinformation?public_address=") != NULL && shared_delegates_website == 1)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_public_address_payment_information(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\"") != NULL && (network_data_node_settings == 1 || test_settings == 1))
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_node_to_network_data_nodes_get_previous_current_next_block_verifiers_list(client_socket);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\"") != NULL && (network_data_node_settings == 1 || test_settings == 1))
 {
   if ((strstr(buffer,"\"public_address\"") != NULL && server_limit_public_addresses(1,(const char*)buffer) == 1) || (strstr(buffer,"\"public_address\"") == NULL && server_limit_IP_addresses(1,(const char*)client_IP_address) == 1))
   { 
     server_receive_data_socket_node_to_network_data_nodes_get_current_block_verifiers_list(client_socket);
     strstr(buffer,"\"public_address\"") != NULL ? server_limit_public_addresses(3,(const char*)buffer) : server_limit_IP_addresses(0,(const char*)client_IP_address);
   }   
 } 
 else if (strstr(buffer,"\"message_settings\": \"NETWORK_DATA_NODES_TO_NETWORK_DATA_NODES_DATABASE_SYNC_CHECK\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_network_data_nodes_to_network_data_nodes_database_sync_check((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_nodes_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH\"") != NULL || strstr(buffer,"NODE_TO_BLOCK_VERIFIERS_GET_RESERVE_BYTES_DATABASE_HASH|") != NULL)
 {
   if ((strstr(buffer,"|") != NULL && server_limit_public_addresses(2,(const char*)buffer) == 1) || (strstr(buffer,"|") == NULL && server_limit_IP_addresses(1,(const char*)client_IP_address) == 1))
   {
     server_receive_data_socket_node_to_block_verifiers_get_reserve_bytes_database_hash(client_socket,(const char*)buffer);
     strstr(buffer,"|") != NULL ? server_limit_public_addresses(4,(const char*)buffer) : server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"NODE_TO_BLOCK_VERIFIERS_CHECK_IF_CURRENT_BLOCK_VERIFIER\"") != NULL)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_node_to_block_verifiers_check_if_current_block_verifier(client_socket);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_sync_check_all_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_proofs_database_download_file_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_sync_check_all_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_reserve_bytes_database_download_file_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_sync_check_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
    server_receive_data_socket_block_verifiers_to_block_verifiers_delegates_database_download_file_update(client_socket);
    server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_sync_check_update(client_socket,(const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_statistics_database_download_file_update(client_socket);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"NODE_TO_BLOCK_VERIFIERS_ADD_RESERVE_PROOF") != NULL)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     // since this function will modify the delegates total vote count, only add one reserve proof at a time
     pthread_mutex_lock(&add_reserve_proof_lock);
     server_receive_data_socket_node_to_block_verifiers_add_reserve_proof(client_socket,(const char*)buffer);
     pthread_mutex_unlock(&add_reserve_proof_lock);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_INVALID_RESERVE_PROOFS\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     pthread_mutex_lock(&invalid_reserve_proof_lock);
     server_receive_data_socket_block_verifiers_to_block_verifiers_invalid_reserve_proofs((const char*)buffer);
     pthread_mutex_unlock(&invalid_reserve_proof_lock);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }  
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_REGISTER_DELEGATE") != NULL)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_nodes_to_block_verifiers_register_delegates(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 }
 else if (strstr(buffer,"NODES_TO_BLOCK_VERIFIERS_UPDATE_DELEGATE") != NULL)
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_nodes_to_block_verifiers_update_delegates(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_NETWORK_DATA_NODE_BLOCK_VERIFIERS_CURRENT_TIME\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_network_data_nodes_block_verifiers_current_time(client_socket);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_ONLINE_STATUS\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_online_status((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_START_BLOCK\"") != NULL && main_network_data_node_create_block == 1)
 {
   color_print("anti ddos start","yellow");
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     color_print("anti ddos start","yellow");
     server_receive_data_socket_main_network_data_node_to_block_verifier_start_block((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NETWORK_DATA_NODE_TO_BLOCK_VERIFIERS_CREATE_NEW_BLOCK\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_main_network_data_node_to_block_verifier_create_new_block((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 } 
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_MAIN_NETWORK_DATA_NODE_CREATE_NEW_BLOCK\"") != NULL)
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifier_to_main_network_data_node_create_new_block((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"\"message_settings\": \"MAIN_NODES_TO_NODES_PART_4_OF_ROUND_CREATE_NEW_BLOCK\"") != NULL && ((network_functions_test_error_settings != 2 && current_UTC_date_and_time.tm_sec >= START_TIME_SECONDS_NETWORK_BLOCK_PART_2 && current_UTC_date_and_time.tm_sec < START_TIME_SECONDS_NETWORK_BLOCK_PART_3) || (network_functions_test_error_settings == 2)))
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_main_node_to_node_message_part_4((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }         
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_VRF_DATA\"") != NULL && ((network_functions_test_error_settings != 2 && current_UTC_date_and_time.tm_sec < START_TIME_SECONDS_NETWORK_BLOCK_PART_2) || (network_functions_test_error_settings == 2)))
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_vrf_data((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }  
 else if (strstr(buffer,"\"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_BLOCK_BLOB_SIGNATURE\"") != NULL && ((network_functions_test_error_settings != 2 && current_UTC_date_and_time.tm_sec >= START_TIME_SECONDS_NETWORK_BLOCK_PART_3 && current_UTC_date_and_time.tm_sec < START_TIME_SECONDS_NETWORK_BLOCK_PART_4) || (network_functions_test_error_settings == 2)))
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_block_verifiers_to_block_verifiers_block_blob_signature((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }  
 else if (strstr(buffer,"\"message_settings\": \"NODES_TO_NODES_VOTE_RESULTS\"") != NULL && ((current_UTC_date_and_time.tm_sec >= SEND_DATA_TIME_SECONDS_NETWORK_BLOCK_PART_4 && current_UTC_date_and_time.tm_sec < START_TIME_SECONDS_NETWORK_BLOCK_PART_5) || (current_UTC_date_and_time.tm_min % BLOCK_TIME == 4 && current_UTC_date_and_time.tm_sec >= START_TIME_SECONDS_INVALID_RESERVE_PROOFS_PART_2 && current_UTC_date_and_time.tm_sec < START_TIME_SECONDS_INVALID_RESERVE_PROOFS_PART_3) || (network_functions_test_error_settings == 2)))
 {
   if (server_limit_public_addresses(1,(const char*)buffer) == 1)
   {
     server_receive_data_socket_node_to_node((const char*)buffer);
     server_limit_public_addresses(3,(const char*)buffer);
   }
 }
 else if (strstr(buffer,"GET /") != NULL && (delegates_website == 1 || shared_delegates_website == 1))
 {
   if (server_limit_IP_addresses(1,(const char*)client_IP_address) == 1)
   {
     server_receive_data_socket_get_files(client_socket,(const char*)buffer);
     server_limit_IP_addresses(0,(const char*)client_IP_address);
   }
 }


 // print or reset the error messages
 if (error_message.total != 0 && network_functions_test_error_settings <= 1)
 {
   if (network_functions_test_error_settings == 0)
   {
     // reset the error messages
     for (error_message_count = 0; error_message_count < TOTAL_DELEGATES_DATABASE_FIELDS; error_message_count++)
     {
       memset(error_message.function[error_message_count],0,strlen(error_message.function[error_message_count]));
       memset(error_message.data[error_message_count],0,strlen(error_message.data[error_message_count]));
     }
     error_message.total = 0;     
   }
   else if (network_functions_test_error_settings == 1)
   {
     print_error_message(current_date_and_time,current_UTC_date_and_time,data2);
   }
}
pointer_reset(buffer);
return;
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
  const int CONNECTIONS_PER_THREAD = MAXIMUM_CONNECTIONS;
  
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
      //epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[count2].data.fd, &events_copy);
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
  char data2[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];
  long file_size;

  // define macros
  #define SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR \
  memset(data2,0,strlen(data2)); \
  memcpy(data2,"{\"Error\":\"Could not get the file\"}",34); \
  send_data(CLIENT_SOCKET,(unsigned char*)data2,strlen((const char*)data2),400,"application/json"); \
  return 0;

  memset(data2,0,sizeof(data2));
  memset(buffer,0,sizeof(buffer));

  // get the file
  memcpy(buffer,&MESSAGE[(strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5],(strlen(MESSAGE) - strlen(strstr(MESSAGE," HTTP/"))) - ((strlen(MESSAGE) - strlen(strstr(MESSAGE,"GET /")))+5));
 
  // remove any invalid request from the url
  string_replace(buffer,sizeof(buffer),"../","");
  string_replace(buffer,sizeof(buffer),"%",""); 
  memcpy(data2,website_path,strnlen(website_path,sizeof(data2)));
  memcpy(data2+strlen(data2),buffer,strnlen(buffer,sizeof(data2)));

  if (strstr(data2,".") == NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,website_path,strnlen(website_path,sizeof(data2)));
    memcpy(data2+strlen(data2),"index.html",10);
  }

  // get the file size
  if ((file_size = get_file_size(data2)) == 0)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,website_path,strnlen(website_path,sizeof(data2)));
    memcpy(data2+strlen(data2),"index.html",10);
    if ((file_size = get_file_size(data2)) == 0)
    {
      SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
    }
  }
  const size_t MAXIMUM_AMOUNT = file_size >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : file_size+SMALL_BUFFER_SIZE;
  unsigned char* data = (unsigned char*)calloc(MAXIMUM_AMOUNT,sizeof(unsigned char));
  if ((file_size = read_file(data,data2)) == 0)
  {
    pointer_reset(data);
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  }

  if (strstr(data2,".html") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"text/html; charset=utf-8",24);
  }
  else if (strstr(data2,".js") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"application/javascript; charset=utf-8",37);
  }
  else if (strstr(data2,".css") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"text/css; charset=utf-8",23);
  }
  else if (strstr(data2,".png") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"image/png",9);
  }
  else if (strstr(data2,".jpg") != NULL || strstr(data2,".jpeg") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"image/jpeg",10);
  }
  else if (strstr(data2,".ico") != NULL)
  {
    memset(data2,0,sizeof(data2));
    memcpy(data2,"image/x-icon",12);
  }
  else
  {
    pointer_reset(data);
    SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR;
  }

  send_data(CLIENT_SOCKET,data,(const long)file_size,200,data2);

  pointer_reset(data);
  return 1;

  #undef SERVER_RECEIVE_DATA_SOCKET_GET_FILES_ERROR
}