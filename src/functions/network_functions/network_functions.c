#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"

#include "structures.h"
#include "variables.h"

#include "block_verifiers_functions.h"
#include "database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "organize_functions.h"
#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: check_if_IP_address_or_hostname
Description: Checks if the data is an IP address or a hostname
Parameters:
  HOST - The hostname or IP address
Return: 1 if an IP address, 2 if a hostname
-----------------------------------------------------------------------------------------------------------
*/

int check_if_IP_address_or_hostname(const char* HOST)
{
  // Constants
  const size_t HOST_LENGTH = strlen(HOST);

  // Variables
  size_t count;

  // quickly check if its a hostname
  if (string_count(HOST,".") != 3)
  {
    return 2;
  }

  for (count = 0; count < HOST_LENGTH; count++)
  {
    if (strncmp(&HOST[count],"0",1) != 0 && strncmp(&HOST[count],"1",1) != 0 && strncmp(&HOST[count],"2",1) != 0 && strncmp(&HOST[count],"3",1) != 0 && strncmp(&HOST[count],"4",1) != 0 && strncmp(&HOST[count],"5",1) != 0 && strncmp(&HOST[count],"6",1) != 0 && strncmp(&HOST[count],"7",1) != 0 && strncmp(&HOST[count],"8",1) != 0 && strncmp(&HOST[count],"9",1) != 0 && strncmp(&HOST[count],".",1) != 0)
    {
      return 2;
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_http_request
Description: Sends a HTTP request
Parameters:
  result - Where the result is stored
  HOST - The hostname or IP address
  URL - The URL
  PORT - The port
  HTTP_SETTINGS - The HTTP method
  HTTP_HEADERS - The HTTP headers
  HTTP_HEADERS_LENGTH - The length of the HTTP headers
  DATA - The request data. If sending a GET request, the data is appended to the url. If sending a POST request, the data is sent in the request body
  DATA_TIMEOUT_SETTINGS - The timeout settings for reading the data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS)
{
  // Constants
  const struct timeval SOCKET_TIMEOUT = {DATA_TIMEOUT_SETTINGS, 0}; 
  const size_t HTTP_SETTINGS_LENGTH = strnlen(HTTP_SETTINGS,BUFFER_SIZE);
  const size_t URL_LENGTH = strnlen(URL,BUFFER_SIZE);
  const size_t DATA_LENGTH = strlen(DATA);
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);

  // Variables
  char buffer2[BUFFER_SIZE];
  char* post_request_data;
  char str[BUFFER_SIZE]; 
  char* message = (char*)calloc(TX_VERIFICATION_MAXIMUM_SIZE,sizeof(char));
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  int count;  
  struct pollfd socket_file_descriptors;
  int socket_settings;
  struct addrinfo serv_addr;
  struct addrinfo* settings = NULL;
  socklen_t socket_option_settings = sizeof(int);
  int network_socket;

  // define macros
  #define SEND_HTTP_REQUEST_ERROR(data_settings,socket_settings) \
  if (test_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"send_http_request",17); \
    memcpy(error_message.data[error_message.total],data_settings,sizeof(data_settings)-1); \
    error_message.total++; \
  } \
  freeaddrinfo(settings); \
  pointer_reset(message); \
  if ((socket_settings) == 1) \
  { \
    close(network_socket); \
  } \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    memcpy(error_message.function[error_message.total],"send_http_request",17);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,str);  
    exit(0);
  }
  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));

  // create the HTTP request
  memcpy(message,HTTP_SETTINGS,HTTP_SETTINGS_LENGTH);
  memcpy(message+strlen(message)," ",sizeof(char));
  memcpy(message+strlen(message),URL,URL_LENGTH);
  if (strncmp(HTTP_SETTINGS,"GET",BUFFER_SIZE) == 0)
  {
    memcpy(message+strlen(message),"?",sizeof(char));
    memcpy(message+strlen(message),DATA,DATA_LENGTH);
  }
  memcpy(message+strlen(message)," HTTP/1.1\r\nHost: ",17);
  memcpy(message+strlen(message),HOST,HOST_LENGTH);
  memcpy(message+strlen(message),"\r\n",2);
  for (count = 0; count < (int)HTTP_HEADERS_LENGTH; count++)
  {
    memcpy(message+strlen(message),HTTP_HEADERS[count],strnlen(HTTP_HEADERS[count],BUFFER_SIZE));
    memcpy(message+strlen(message),"\r\n",2);
  }
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    memcpy(message+strlen(message),"Content-Length: ",16);
    snprintf(str, sizeof(str)-1, "%zu", strlen(DATA));
    memcpy(message+strlen(message),str,strnlen(str,BUFFER_SIZE));
  } 
  memcpy(message+strlen(message),"\r\n\r\n",4);  
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    memcpy(message+strlen(message),DATA,DATA_LENGTH);
  }

  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT);  

  // set up the addrinfo
  memset(&serv_addr, 0, sizeof(serv_addr));
  if (check_if_IP_address_or_hostname(HOST) == 1)
  {
    /* the host is an IP address
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AI_NUMERICHOST = The host is already an IP address, and this will have getaddrinfo not lookup the hostname
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  else
  {
    /* the host is a domain name
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  
  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  if (getaddrinfo(str, buffer2, &serv_addr, &settings) != 0)
  {
    SEND_HTTP_REQUEST_ERROR("Error invalid hostname",0);
  }

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  if ((network_socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol)) == -1)
  { 
    SEND_HTTP_REQUEST_ERROR("Error creating socket",0);
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_SNDTIMEO = allow the socket on sending data, to use the timeout settings
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(network_socket, SOL_SOCKET, SO_SNDTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0 || setsockopt(network_socket, SOL_SOCKET, SO_RCVTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    SEND_HTTP_REQUEST_ERROR("Error setting socket timeout",1);
  }  

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = network_socket;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(network_socket,settings->ai_addr, settings->ai_addrlen) != 0)
  {    
    count = poll(&socket_file_descriptors,1,CONNECTION_TIMEOUT_SETTINGS * 1000);  
    if ((count != 1) || (count == 1 && getsockopt(network_socket,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0 && socket_settings != 0))
    { 
      SEND_HTTP_REQUEST_ERROR("Error connecting to host",1);
    } 
  }

  if (send_data(network_socket,(unsigned char*)message,0,0,"") == 0)
  {
    SEND_HTTP_REQUEST_ERROR("Error sending data to host",1);
  }
   
  // get the result
  if (receive_data(network_socket,message,TX_VERIFICATION_MAXIMUM_SIZE,1,DATA_TIMEOUT_SETTINGS) < 2)
  {
    SEND_HTTP_REQUEST_ERROR("Error receiving data from host",1);
  }

  // check if the data recived is correct
  if (strstr(message,"{") == NULL && strstr(message,"error") == NULL)
  {
    SEND_HTTP_REQUEST_ERROR("Error receiving data from host",1);
  }
  
  // parse the HTTP request header from the result
  // check if the result includes a header
  if (strstr(message,"HTTP/") != NULL)
  {
    // the HTTP result does contain a header
    post_request_data = strstr(message,"{");
    memcpy(result+strlen(result),post_request_data,strlen(post_request_data));
  }
  else
  {
    // the HTTP result does not contain a header
    memcpy(result+strlen(result),message,strlen(message));
  }
  freeaddrinfo(settings);
  pointer_reset(message);
  close(network_socket);
  return 1;

  #undef SEND_HTTP_REQUEST_ERROR
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_and_receive_data_socket
Description: Send a message through a socket and receives data
Parameters:
  result - The result from the host
  RESULT_LENGTH - The maximum size of result
  HOST - The host to send the message to
  PORT - The port to send the message through
  DATA - The message
  DATA_TIMEOUT_SETTINGS - the timeout settings for reading the data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_and_receive_data_socket(char *result, const size_t RESULT_LENGTH, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS)
{ 
  // Constants
  const struct timeval SOCKET_TIMEOUT = {DATA_TIMEOUT_SETTINGS, 0}; 
  const size_t MAXIMUM_AMOUNT = strlen(DATA) >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : strlen(DATA)+BUFFER_SIZE;

  // Variables 
  char buffer2[MAXIMUM_NUMBER_SIZE];
  char str[SMALL_BUFFER_SIZE];
  char* message = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char)); 
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  int count;
  struct addrinfo serv_addr;
  struct addrinfo* settings = NULL;
  socklen_t socket_option_settings = sizeof(int);
  int network_socket;

  // define macros
  #define SEND_AND_RECEIVE_DATA_SOCKET_ERROR(data_settings,socket_settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28); \
    memcpy(error_message.data[error_message.total],data_settings,sizeof(data_settings)-1); \
    error_message.total++; \
  } \
  freeaddrinfo(settings); \
  pointer_reset(message); \
  if ((socket_settings) == 1) \
  { \
    close(network_socket); \
  } \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    memcpy(error_message.function[error_message.total],"send_http_request",17);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message(current_date_and_time,current_UTC_date_and_time,str);  
    exit(0);
  }

  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));

  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT);  

  // set up the addrinfo
  memset(&serv_addr, 0, sizeof(serv_addr));
  if (check_if_IP_address_or_hostname(HOST) == 1)
  {
    /* the host is an IP address
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AI_NUMERICHOST = The host is already an IP address, and this will have getaddrinfo not lookup the hostname
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  else
  {
    /* the host is a domain name
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  
  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  if (getaddrinfo(str, buffer2, &serv_addr, &settings) != 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error invalid hostname",0);
  }

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  if ((network_socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol)) == -1)
  { 
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error creating socket",0);
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_SNDTIMEO = allow the socket on sending data, to use the timeout settings
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(network_socket, SOL_SOCKET, SO_SNDTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0 || setsockopt(network_socket, SOL_SOCKET, SO_RCVTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error setting socket timeout",1);
  }

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = network_socket;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(network_socket,settings->ai_addr, settings->ai_addrlen) != 0)
  {    
    count = poll(&socket_file_descriptors,1,CONNECTION_TIMEOUT_SETTINGS * 1000);  
    if ((count != 1) || (count == 1 && getsockopt(network_socket,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0 && socket_settings != 0))
    { 
      SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error connecting to host",1);
    } 
  }

  memset(message,0,strlen(message));
  memcpy(message,DATA,strnlen(DATA,MAXIMUM_AMOUNT));
  if (send_data(network_socket,(unsigned char*)message,0,1,"") == 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error sending data to host",1);
  }
    
  // get the result
  memset(result,0,strlen(result));
  if (receive_data(network_socket,result,RESULT_LENGTH,1,DATA_TIMEOUT_SETTINGS) < 2 || strncmp(result,ERROR_DATA,BUFFER_SIZE) == 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Error receiving data from host",1);
  }
  
  // validate the data, dont run this is running the test, since it uses send_and_receive_data_socket to check all messages
  if (test_settings == 0 && validate_data(result) == 0)
  {
    SEND_AND_RECEIVE_DATA_SOCKET_ERROR("Could not validate the message",1);
  }
  
  freeaddrinfo(settings);
  pointer_reset(message);
  close(network_socket);
  return 1;

  #undef SEND_AND_RECEIVE_DATA_SOCKET_ERROR
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_data_socket
Description: Send a message through a socket
Parameters:
  HOST - The host to send the message to
  PORT - The port to send the message through
  DATA - The message
  DATA_TIMEOUT_SETTINGS - the timeout settings for sending the data
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data_socket(const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS)
{ 
  // Constants
  const struct timeval SOCKET_TIMEOUT = {DATA_TIMEOUT_SETTINGS, 0};
  const size_t MAXIMUM_AMOUNT = strlen(DATA) >= BUFFER_SIZE ? BUFFER_SIZE : strlen(DATA)+BUFFER_SIZE;
  
  // Variables  
  char buffer2[MAXIMUM_NUMBER_SIZE];
  char str[SMALL_BUFFER_SIZE];
  char message[MAXIMUM_AMOUNT];
  struct pollfd socket_file_descriptors;
  int socket_settings;
  int count;
  struct addrinfo serv_addr;
  struct addrinfo* settings = NULL;
  socklen_t socket_option_settings = sizeof(int);
  int network_socket;

  // define macros
  // define macros
  #define SEND_DATA_SOCKET_ERROR(data_settings,socket_settings) \
  if (debug_settings == 1) \
  { \
    memcpy(error_message.function[error_message.total],"send_data_socket",16); \
    memcpy(error_message.data[error_message.total],data_settings,sizeof(data_settings)-1); \
    error_message.total++; \
  } \
  freeaddrinfo(settings); \
  if ((socket_settings) == 1) \
  { \
    close(network_socket); \
  } \
  return 0;

  memset(buffer2,0,sizeof(buffer2));
  memset(str,0,sizeof(str));
  memset(message,0,sizeof(message));
  
  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT);

  // set up the addrinfo
  memset(&serv_addr, 0, sizeof(serv_addr));
  if (check_if_IP_address_or_hostname(HOST) == 1)
  {
    /* the host is an IP address
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AI_NUMERICHOST = The host is already an IP address, and this will have getaddrinfo not lookup the hostname
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  else
  {
    /* the host is a domain name
    AI_NUMERICSERV = Specifies that getaddrinfo is provided a numerical port
    AF_INET = IPV4 support
    SOCK_STREAM = TCP protocol
    */
    serv_addr.ai_flags = AI_NUMERICSERV;
    serv_addr.ai_family = AF_INET;
    serv_addr.ai_socktype = SOCK_STREAM;
  }
  
  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));

  if (getaddrinfo(str, buffer2, &serv_addr, &settings) != 0)
  {     
    SEND_DATA_SOCKET_ERROR("Error invalid hostname",0);
  }

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  if ((network_socket = socket(settings->ai_family, settings->ai_socktype | SOCK_NONBLOCK, settings->ai_protocol)) == -1)
  { 
    SEND_DATA_SOCKET_ERROR("Error creating socket",0);
  }

   /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_SNDTIMEO = allow the socket on sending data, to use the timeout settings
  */
  if (setsockopt(network_socket, SOL_SOCKET, SO_SNDTIMEO,&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    SEND_DATA_SOCKET_ERROR("Error setting socket timeout",1);
  }

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = network_socket;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(network_socket,settings->ai_addr, settings->ai_addrlen) != 0)
  {    
    count = poll(&socket_file_descriptors,1,CONNECTION_TIMEOUT_SETTINGS * 1000);  
    if ((count != 1) || (count == 1 && getsockopt(network_socket,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0 && socket_settings != 0))
    { 
      SEND_DATA_SOCKET_ERROR("Error connecting to host",1);
    }
  }

  // send the message 
  memset(message,0,sizeof(message));
  memcpy(message,DATA,strnlen(DATA,sizeof(message)));
  if (send_data(network_socket,(unsigned char*)message,0,1,"") == 0)
  {  
    SEND_DATA_SOCKET_ERROR("Error sending data to host",1);
  }

  freeaddrinfo(settings);    
  close(network_socket);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_data
Description: Sends data to a socket
Parameters:
  SOCKET - The socket
  data - The data to send
  DATA_LENGTH - The length of data, only used when MESSAGE_SETTINGS is not either 0 or 1
  MESSAGE_SETTINGS - 0 to not change the message, 1 to append the SOCKET_END_STRING, any other HTTP status code to prepend the HTTP headers
  MESSAGE_DATA_SETTINGS - The message data settings
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data(const int SOCKET, unsigned char* data, const long DATA_LENGTH, const int MESSAGE_SETTINGS, const char* MESSAGE_DATA_SETTINGS)
{
  // Constants
  const size_t MAXIMUM_AMOUNT = DATA_LENGTH >= MAXIMUM_BUFFER_SIZE ? MAXIMUM_BUFFER_SIZE : DATA_LENGTH+BUFFER_SIZE;

  // Variables
  size_t count;
  time_t current_date_and_time;
  struct tm current_UTC_date_and_time;
  size_t total;
  size_t sent;
  long long int bytes = 1;
  struct pollfd socket_file_descriptors;

  if (MESSAGE_SETTINGS == 1)
  {
    // append the SOCKET_END_STRING to the message since this is a socket message
    memcpy(data+strlen((const char*)data),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
    total = strlen((const char*)data);
  }    
  else if (MESSAGE_SETTINGS != 0)
  {
    // prepend the HTTP headers to the message

    // Variables
    char* message = (char*)calloc(MAXIMUM_AMOUNT,sizeof(char));
    char current_date_and_time_data[BUFFER_SIZE];

    memset(current_date_and_time_data,0,sizeof(current_date_and_time_data));

    // get the current time
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

    strftime(current_date_and_time_data,sizeof(current_date_and_time_data),"%a, %d %b %Y %H:%M:%S GMT",&current_UTC_date_and_time);

    memcpy(message,"HTTP/1.1 ",9);
    snprintf(message+9,MAXIMUM_AMOUNT-9,"%d",MESSAGE_SETTINGS);
    if (MESSAGE_SETTINGS == 200)
    {
      memcpy(message+strlen(message)," OK",3);
    }
    else if (MESSAGE_SETTINGS == 400)
    {
      memcpy(message+strlen(message)," Bad Request",12);
    }
    else if (MESSAGE_SETTINGS == 404)
    {
      memcpy(message+strlen(message)," Not Found",10);
    }
    memcpy(message+strlen(message),"\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept\r\nContent-Length: ",147);
    snprintf(message+strlen(message),MAXIMUM_AMOUNT,"%ld",DATA_LENGTH);
    memcpy(message+strlen(message),"\r\nContent-Language: en\r\nContent-Type: ",38);
    memcpy(message+strlen(message),MESSAGE_DATA_SETTINGS,strnlen(MESSAGE_DATA_SETTINGS,MAXIMUM_AMOUNT));
    memcpy(message+strlen(message),"\r\nServer: xcash-dpops version 1.0.0\r\nDate: ",43);
    memcpy(message+strlen(message),current_date_and_time_data,strnlen(current_date_and_time_data,MAXIMUM_AMOUNT));
    memcpy(message+strlen(message),HTTP_SOCKET_END_STRING,sizeof(HTTP_SOCKET_END_STRING)-1);
    count = strlen(message);
    memcpy(message+count,data,DATA_LENGTH);
    memset(data,0,DATA_LENGTH);
    memcpy(data,message,count+DATA_LENGTH);
    total = count+DATA_LENGTH;
    pointer_reset(message);
  } 
  else
  {
    total = strlen((const char*)data);
  }
  
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  for (sent = 0; sent < total; sent += bytes == -1 ? 0 : bytes)
  {
      if ((bytes = send(SOCKET,data+sent,total-sent,MSG_NOSIGNAL)) == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        if (poll(&socket_file_descriptors, 1, 1000) == -1) {
          return 0; // Socket Failure
        }
      }
      else
      {
        return 0;
      }
    }
  }
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: receive_data
Description: Receives data from a socket
Parameters:
  SOCKET - The socket
  message - Where the data is stored
  LENGTH - The maximum size of message
  SOCKET_TIMEOUT_SETTINGS - 1 if a socket_timeout is needed, otherwise 0
  SOCKET_TIMEOUT - The length of the socket_timeout
Return: 0 if an error would have occured from a buffer overflow, 1 if a timeout has occured, 2 if successful
-----------------------------------------------------------------------------------------------------------
*/

int receive_data(const int SOCKET, char *message, const size_t LENGTH, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT)
{
  // Variables
  char* buffer = (char*)calloc(LENGTH,sizeof(char)); 
  time_t start = time(NULL);
  int needsPoll = 0;
  struct pollfd socket_file_descriptors;

  memset(message,0,strlen(message));

  for (;;)
  { 
    memset(buffer,0,strlen(buffer));

    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data, but check errno to see if we should exit or try again
    if (recvfrom(SOCKET, buffer, LENGTH, MSG_DONTWAIT, NULL, NULL) == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        needsPoll = 1;
      }
      else {
        pointer_reset(buffer);
        return 1;
      }
    }

    if (buffer[0] != '\0' && (strstr(buffer,SOCKET_END_STRING) == NULL && strstr(buffer,HTTP_SOCKET_END_STRING) == NULL && strstr(buffer,XCASH_DAEMON_AND_WALLET_SOCKET_END_STRING) == NULL && strstr(buffer,XCASH_DAEMON_AND_WALLET_ERROR_SOCKET_END_STRING) == NULL))
    {
      // there is data, but this is not the final data
      append_string(message,buffer,LENGTH);
    }
    if (buffer[0] != '\0' && (strstr(buffer,SOCKET_END_STRING) != NULL || (strstr(buffer,HTTP_SOCKET_END_STRING) != NULL && (strstr(message,"Server: xcash-dpops") != NULL || strstr(buffer,"Server: xcash-dpops") != NULL || strstr(message,"User-Agent:") != NULL || strstr(buffer,"User-Agent:") != NULL)) || (strstr(buffer,XCASH_DAEMON_AND_WALLET_SOCKET_END_STRING) != NULL) || (strstr(buffer,"\"untrusted\": false\r\n}") != NULL) || (strstr(buffer,XCASH_DAEMON_AND_WALLET_ERROR_SOCKET_END_STRING) != NULL)))
    {
      // there is data, and this is the final data
      append_string(message,buffer,LENGTH);

      // if the final message has the SOCKET_END_STRING in the message, remove it
      if (strstr(buffer,SOCKET_END_STRING) != NULL)
      {
        string_replace(message,LENGTH,SOCKET_END_STRING,"");
        //message[strlen(message)-(sizeof(SOCKET_END_STRING)-1)] = 0;
      }
      pointer_reset(buffer);
      return 2;
    }

    // check for a timeout in receiving data
    if (RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS == 1 && time(NULL) - start > RECEIVE_DATA_SOCKET_TIMEOUT)
    {
      pointer_reset(buffer);
      return 1;
    }

    if (needsPoll == 1) {
      socket_file_descriptors.fd = SOCKET;
      socket_file_descriptors.events = POLLIN;
      time_t timeout = RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS == 1 ? (RECEIVE_DATA_SOCKET_TIMEOUT - (time(NULL) - start)) * 1000 : 1000;

      if (poll(&socket_file_descriptors, 1, timeout) == -1) { // Socket Failure
        pointer_reset(buffer);
        return 1;
      }
      needsPoll = 0;
    }
  }
}
