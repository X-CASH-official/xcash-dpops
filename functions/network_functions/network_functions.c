#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"

#include "structures.h"
#include "variables.h"
#include "database_functions.h"
#include "network_functions.h"
#include "network_security_functions.h"
#include "string_functions.h"

/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

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
  TITLE - A summary of the data sent in the POST request. This text gets printed to the console
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS)
{
  // Constants
  const struct timeval SOCKET_TIMEOUT = {SOCKET_DATA_TIMEOUT_SETTINGS, 0}; 
  const size_t HTTP_SETTINGS_LENGTH = strnlen(HTTP_SETTINGS,BUFFER_SIZE);
  const size_t URL_LENGTH = strnlen(URL,BUFFER_SIZE);
  const size_t DATA_LENGTH = strnlen(DATA,BUFFER_SIZE);
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE); 

  // Variables
  char response[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char* post_request_data;
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count; 
  size_t counter = 0; 
  size_t receive_data_result; 
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(message); \
  message = NULL; 

  // check if the memory needed was allocated on the heap successfully
  if (str == NULL || message == NULL)
  {
    if (str != NULL)
    {
      pointer_reset(str);
    }
    if (message != NULL)
    {
      pointer_reset(message);
    }
    memcpy(error_message.function[error_message.total],"send_http_request",17);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  // create the HTTP request
  memcpy(message,HTTP_SETTINGS,HTTP_SETTINGS_LENGTH);
  counter += HTTP_SETTINGS_LENGTH;
  memcpy(message+counter," ",1);
  counter++;
  memcpy(message+counter,URL,URL_LENGTH);
  counter += URL_LENGTH;
  if (strncmp(HTTP_SETTINGS,"GET",BUFFER_SIZE) == 0)
  {
    memcpy(message+counter,"?",1);
    counter++;
    memcpy(message+counter,DATA,DATA_LENGTH);
    counter += DATA_LENGTH;
  }
  memcpy(message+counter," HTTP/1.1\r\nHost: ",17);
  counter += 17;
  memcpy(message+counter,HOST,HOST_LENGTH);
  counter += HOST_LENGTH;
  memcpy(message+counter,"\r\n",2);
  counter += 2;
  for (count = 0; count < HTTP_HEADERS_LENGTH; count++)
  {
    memcpy(message+counter,HTTP_HEADERS[count],strnlen(HTTP_HEADERS[count],BUFFER_SIZE));
    counter += strnlen(HTTP_HEADERS[count],BUFFER_SIZE);
    memcpy(message+counter,"\r\n",2);
    counter += 2;
  }
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    memcpy(message+counter,"Content-Length: ",16);
    counter += 16;
    sprintf(str, "%d", (int)strnlen(DATA,BUFFER_SIZE));
    memcpy(message+counter,str,strnlen(str,BUFFER_SIZE));
    counter += strnlen(str,BUFFER_SIZE);
  } 
  memcpy(message+counter,"\r\n\r\n",4);   
  counter += 4; 
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    memcpy(message+counter,DATA,DATA_LENGTH);
    counter += DATA_LENGTH;
  }  
  memset(&response, 0, sizeof(response));

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],"Error creating socket for sending a post request",48);
      error_message.total++;
    }
    pointer_reset_all;
    return 0;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],"Error setting socket timeout for sending a post request",55);
      error_message.total++;       
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }  

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(HOST); 
  if (HOST_NAME == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {       
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,HOST,strnlen(HOST,BUFFER_SIZE));
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  // convert the port to a string  
  sprintf(buffer2,"%d",PORT);  

  // get the length of buffer2 and host, since they will not change at this point and we need them for faster string copying
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  memset(&serv_addr,0,sizeof(serv_addr));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {        
        if (MESSAGE_SETTINGS == 1)
        {
          memset(str,0,strnlen(str,BUFFER_SIZE));
          memcpy(str,"Error connecting to ",20);
          memcpy(str+20,HOST,HOST_LENGTH);
          memcpy(str+20+HOST_LENGTH," on port ",9);
          memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
          memcpy(error_message.function[error_message.total],"send_http_request",17);
          memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
          error_message.total++; 
        }
        close(SOCKET);
        pointer_reset_all;
        return 0;
      } 
    }
  }

  // get the current socket settings
  socket_settings = fcntl(SOCKET, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Connected to ",13);
    memcpy(str+13,HOST,HOST_LENGTH);
    memcpy(str+13+HOST_LENGTH," on port ",9);
    memcpy(str+22+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green"); 
  }
    
  // send the message
  if (MESSAGE_SETTINGS == 1)
  {
    printf("Sending %s to %s on port %s\r\n",TITLE,HOST,buffer2);
  }
  if (send_data(SOCKET,message,0) == 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,HOST,HOST_LENGTH);
      memcpy(str+22+HOST_LENGTH," on port ",9);
      memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
   
  // get the result
  receive_data_result = receive_data(SOCKET,message,"{",1,DATA_TIMEOUT_SETTINGS);
  if (receive_data_result < 2)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(str+26+HOST_LENGTH," on port ",9);
      memcpy(str+35+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      if (receive_data_result == 1)
      {
        memcpy(str+35+HOST_LENGTH+BUFFER2_LENGTH,", because of a timeout issue",28);
      }
      else if (receive_data_result == 0)
      { 
        memcpy(str+35+HOST_LENGTH+BUFFER2_LENGTH,", because of a potential buffer overflow issue",46);
      }
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  // check if the data recived is correct
  if (strstr(message,"{") == NULL && strstr(message,"error") == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(str+26+HOST_LENGTH," on port ",9);
      memcpy(str+35+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;  
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Received data from ",19);
    memcpy(str+19,HOST,HOST_LENGTH);
    memcpy(str+19+HOST_LENGTH," on port ",9);
    memcpy(str+28+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green"); 
  }
  
  // parse the HTTP request header from the result
  // check if the result includes a header
  if (strstr(message,"HTTP/") != NULL)
  {
    // the HTTP result does contain a header
    post_request_data = strstr(message,"{");
    memcpy(result+strnlen(result,BUFFER_SIZE),post_request_data,strnlen(post_request_data,BUFFER_SIZE));
  }
  else
  {
    // the HTTP result does not contain a header
    memcpy(result+strnlen(result,BUFFER_SIZE),message,strnlen(message,BUFFER_SIZE));
  }
    
  close(SOCKET);
  pointer_reset_all;
  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef pointer_reset_all
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_and_receive_data_socket
Description: Send a message through a socket and receives data
Parameters:
  result - The result from the host
  HOST - The host to send the message to
  PORT - The port to send the message through
  DATA - The message
  DATA_TIMEOUT_SETTINGS - the timeout settings for reading the data
  TITLE - A summary of the data sent to the host. This text gets printed to the console
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_and_receive_data_socket(char *result, const char* HOST, const int PORT, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS)
{ 
  // Constants
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);
  const struct timeval SOCKET_TIMEOUT = {SOCKET_DATA_TIMEOUT_SETTINGS, 0};   

  // Variables 
  char buffer2[BUFFER_SIZE];
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int receive_data_result;
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(message); \
  message = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (str == NULL || message == NULL)
  {
    if (str != NULL)
    {
      pointer_reset(str);
    }
    if (message != NULL)
    {
      pointer_reset(message);
    }
    memcpy(error_message.function[error_message.total],"get_block_template",18);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error creating socket for sending data to ",42);
      memcpy(str+42,HOST,HOST_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++; 
    }
    pointer_reset_all;
    return 0;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error setting socket timeout for sending data to ",49);
      memcpy(str+49,HOST,HOST_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  } 

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(HOST); 
  if (HOST_NAME == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {        
        if (MESSAGE_SETTINGS == 1)
        {
          memset(str,0,strnlen(str,BUFFER_SIZE));
          memcpy(str,"Error connecting to ",20);
          memcpy(str+20,HOST,HOST_LENGTH);
          memcpy(str+20+HOST_LENGTH," on port ",9);
          memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
          memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
          memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
          error_message.total++;
        }
        close(SOCKET);
        pointer_reset_all;
        return 0;
      } 
    }
  }

  // get the current socket settings
  socket_settings = fcntl(SOCKET, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++; 
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }

  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Connected to ",13);
    memcpy(str+13,HOST,HOST_LENGTH);
    memcpy(str+13+HOST_LENGTH," on port ",9);
    memcpy(str+22+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green"); 
  }

  // send the message 
  memcpy(message,DATA,strnlen(DATA,BUFFER_SIZE));
  if (MESSAGE_SETTINGS == 1)
  {
    printf("Sending %s to %s on port %s\r\n",TITLE,HOST,buffer2);
  }
  if (send_data(SOCKET,message,1) == 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,HOST,HOST_LENGTH);
      memcpy(str+22+HOST_LENGTH," on port ",9);
      memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
    
  // get the result
  receive_data_result = receive_data(SOCKET,message,SOCKET_END_STRING,1,DATA_TIMEOUT_SETTINGS);
  if (receive_data_result < 2)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(str+26+HOST_LENGTH," on port ",9);
      memcpy(str+35+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      if (receive_data_result == 1)
      {
        memcpy(str+35+HOST_LENGTH+BUFFER2_LENGTH,", because of a timeout issue",28);
      }
      else if (receive_data_result == 0)
      {   
        memcpy(str+35+HOST_LENGTH+BUFFER2_LENGTH,", because of a potential buffer overflow issue",46);
      }
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
      error_message.total++;
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
     
  memcpy(result,message,strnlen(message,BUFFER_SIZE));
  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Received data from ",19);
    memcpy(str+19,HOST,HOST_LENGTH);
    memcpy(str+19+HOST_LENGTH," on port ",9);
    memcpy(str+28+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green");
  }
  
  close(SOCKET);
  pointer_reset_all;
  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_data_socket
Description: Send a message through a socket
Parameters:
  HOST - The host to send the message to
  PORT - The port to send the message through
  DATA - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data_socket(const char* HOST, const int PORT, const char* DATA)
{ 
  // Constants
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);
  const struct timeval SOCKET_TIMEOUT = {SOCKET_DATA_TIMEOUT_SETTINGS, 0};   
  
  // Variables  
  char buffer2[BUFFER_SIZE];
  char str[BUFFER_SIZE];
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SEND_DATA_SOCKET_ERROR(message) \
  memcpy(error_message.function[error_message.total],"send_data_socket",16); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
  error_message.total++; \
  close(SOCKET); \
  return 0;
  

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings when connecting
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    memcpy(str,"Error creating socket for sending data to ",42);
    memcpy(str+42,HOST,HOST_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {   
    memcpy(str,"Error setting socket timeout for sending data to ",49);
    memcpy(str+49,HOST,HOST_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  } 

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(HOST); 
  if (HOST_NAME == NULL)
  {    
    memcpy(str,"Error invalid hostname of ",26);
    memcpy(str+26,HOST,HOST_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,1,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {       
        memset(str,0,strnlen(str,BUFFER_SIZE));
        memcpy(str,"Error connecting to ",20);
        memcpy(str+20,HOST,HOST_LENGTH);
        memcpy(str+20+HOST_LENGTH," on port ",9);
        memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
        SEND_DATA_SOCKET_ERROR(str);
      } 
    }
  }

  // get the current socket settings
  socket_settings = fcntl(SOCKET, F_GETFL, NULL);
  if (socket_settings == -1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Error connecting to ",20);
    memcpy(str+20,HOST,HOST_LENGTH);
    memcpy(str+20+HOST_LENGTH," on port ",9);
    memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Error connecting to ",20);
    memcpy(str+20,HOST,HOST_LENGTH);
    memcpy(str+20+HOST_LENGTH," on port ",9);
    memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }

  // send the message 
  memcpy(message,DATA,strnlen(DATA,BUFFER_SIZE));
  if (send_data(SOCKET,message,1) == 0)
  {    
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Error sending data to ",22);
    memcpy(str+22,HOST,HOST_LENGTH);
    memcpy(str+22+HOST_LENGTH," on port ",9);
    memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }
    
  close(SOCKET);
  pointer_reset(message);
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: send_data
Description: Sends data to a socket
Parameters:
  SOCKET - The socket
  data - The data to send
  APPEND_STRING_SETTINGS - 1 to append an end string to the data, else 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data(const int SOCKET, char* data, const int APPEND_STRING_SETTINGS)
{
  if (APPEND_STRING_SETTINGS == 1)
  {
    memcpy(data+strnlen(data,BUFFER_SIZE),SOCKET_END_STRING,sizeof(SOCKET_END_STRING)-1);
  }    
  const int TOTAL = strnlen(data,BUFFER_SIZE);
  int sent = 0;
  int bytes = 0;
  do {
    bytes = write(SOCKET,data+sent,TOTAL-sent);
    if (bytes < 0)
    {             
      return 0;
    }
    else if (bytes == 0)  
    {
      break;
    }
    sent+=bytes;
    } while (sent < TOTAL);
    return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: receive_data
Description: Receives data from a socket
Parameters:
  SOCKET - The socket
  message - Where the data is stored
  STRING - The end string to know the socket is done receving data
  SOCKET_TIMEOUT_SETTINGS - 1 if a socket_timeout is needed, otherwise 0
  SOCKET_TIMEOUT - The length of the socket_timeout
Return: 0 if an error would have occured from a buffer overflow, 1 if a timeout has occured, 2 if successful
-----------------------------------------------------------------------------------------------------------
*/

int receive_data(const int SOCKET, char *message, const char* STRING, const int RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS, const int RECEIVE_DATA_SOCKET_TIMEOUT)
{
  // Variables
  int count = 0;
  char buffer [BUFFER_SIZE];
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL)
  {
    memcpy(error_message.function[error_message.total],"get_block_template",18);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  memset(message, 0, strnlen(message,BUFFER_SIZE));  
  for (;;)
  { 
    memset(&buffer, 0, sizeof(buffer));
    // check the size of the data that were about to receive. If it is over BUFFER_SIZE then dont accept it, since it will cause a buffer overflow
    if (recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= BUFFER_SIZE)
    {
      pointer_reset(data);
      return 0;
    }    
    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data
    recvfrom(SOCKET, buffer, BUFFER_SIZE, MSG_DONTWAIT, NULL, NULL);  
    if (buffer[0] != '\0' && strstr(buffer,STRING) == NULL)
    {
      // there is data, but this is not the final data
      memcpy(message,buffer,strnlen(buffer,BUFFER_SIZE));
    }
    if (buffer[0] != '\0' && strstr(buffer,STRING) != NULL)
    {
      // there is data, and this is the final data
      memcpy(message,buffer,strnlen(buffer,BUFFER_SIZE));
      // if the final message has the SOCKET_END_STRING in the message, remove it
      if (strstr(message,SOCKET_END_STRING) != NULL)
      {
        // remove SOCKET_END_STRING from the message
        memcpy(data,message,strnlen(message,BUFFER_SIZE) - (sizeof(SOCKET_END_STRING)-1));
        memset(message, 0, strnlen(message,BUFFER_SIZE));
        memcpy(message,data,strnlen(data,BUFFER_SIZE));
      }
      break;
    }

    // check for a timeout in receiving data
    if (RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS == 1)
    {
      count++;
      if (count > (RECEIVE_DATA_SOCKET_TIMEOUT * 5))
      {
        pointer_reset(data);
        return 1;
      }
    } 

    usleep(200000);   
  }
  pointer_reset(data);
  return 2;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_all_block_verifiers_list
Description: Sync the previous, current and next block verifiers from a network data node. This function is only run at startup, since after that the database is used to get the block verifiers list
-----------------------------------------------------------------------------------------------------------
*/

int sync_all_block_verifiers_list()
{
  // Variables
  struct database_multiple_documents_fields database_multiple_documents_fields;
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  size_t count;
  size_t count2;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_ALL_BLOCK_VERIFIERS_LIST(settings) \
  memcpy(error_message.function[error_message.total],"sync_all_block_verifiers_list",29); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data2 == NULL || data3 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"get_block_template",18);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // reset the previous current and next block verifiers list
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(previous_block_verifiers_list.block_verifiers_name[count],0,strlen(previous_block_verifiers_list.block_verifiers_name[count]));
    memset(previous_block_verifiers_list.block_verifiers_public_address[count],0,strlen(previous_block_verifiers_list.block_verifiers_public_address[count]));
    memset(previous_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(previous_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(current_block_verifiers_list.block_verifiers_name[count],0,strlen(current_block_verifiers_list.block_verifiers_name[count]));
    memset(current_block_verifiers_list.block_verifiers_public_address[count],0,strlen(current_block_verifiers_list.block_verifiers_public_address[count]));
    memset(current_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(current_block_verifiers_list.block_verifiers_IP_address[count]));
    memset(next_block_verifiers_list.block_verifiers_name[count],0,strlen(next_block_verifiers_list.block_verifiers_name[count]));
    memset(next_block_verifiers_list.block_verifiers_public_address[count],0,strlen(next_block_verifiers_list.block_verifiers_public_address[count]));
    memset(next_block_verifiers_list.block_verifiers_IP_address[count],0,strlen(next_block_verifiers_list.block_verifiers_IP_address[count]));
  }

  if (network_data_node_settings == 0)
  {
    print_start_message("Syncing the previous, current and next block verifiers directly from a network data node, to be able to update the database");

    // create the message
    memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\",\r\n}",103);

    start:
    printf("Connecting to a random network data node to get a list of previous, current and next block verifiers\n");

    // send the message to a random network data node
    do
    {
      count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
    } while (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0);
  
    // get the current time
    get_current_UTC_time;

    memset(data3,0,strlen(data3));
    memcpy(data3,"Connecting to network data node ",32);
    memcpy(data3+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data3+strlen(data3)," and sending NODE_TO_NETWORK_DATA_NODES_GET_PREVIOUS_CURRENT_NEXT_BLOCK_VERIFIERS_LIST\n",87);
    memcpy(data3+strlen(data3),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
    printf("%s\n",data3);
    memset(data3,0,strlen(data3));

    if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      memcpy(data2,"Could not receive data from network data node ",46);
      memcpy(data2+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
      color_print(data2,"red");
      memset(data2,0,strlen(data2));
      printf("Connecting to a different network data node\n\n");
      goto start;
    }

    if (verify_data(data3,0,0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not verify data");
    }
 
    // parse the message
    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"previous_block_verifiers_name_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the previous_block_verifiers_name_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_name[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"previous_block_verifiers_public_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the previous_block_verifiers_public_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"previous_block_verifiers_IP_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the previous_block_verifiers_IP_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    // parse the message
    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"current_block_verifiers_name_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the current_block_verifiers_name_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_name[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"current_block_verifiers_public_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the current_block_verifiers_public_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_public_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"current_block_verifiers_IP_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the current_block_verifiers_IP_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    // parse the message
    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"next_block_verifiers_name_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the next_block_verifiers_name_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_name[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"next_block_verifiers_public_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the next_block_verifiers_public_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_public_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    memset(data2,0,strlen(data2));
    if (parse_json_data(data3,"next_block_verifiers_IP_address_list",data2) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not parse the message");
    } 

    // parse the next_block_verifiers_IP_address_list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],data2,strnlen(data2,BUFFER_SIZE) - strnlen(strstr(data2,"|"),BUFFER_SIZE));
      data2 = strstr(data2,"|") + 1;
    }

    color_print("The previous, current and next block verifiers have been synced from a network data node","green");
  }
  else
  {
    print_start_message("Loading the previous, current and next block verifiers from the database");

    // initialize the database_multiple_documents_fields struct 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        database_multiple_documents_fields.item[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));
        database_multiple_documents_fields.value[count][count2] = (char*)calloc(BUFFER_SIZE,sizeof(char));

        if (database_multiple_documents_fields.item[count][count2] == NULL || database_multiple_documents_fields.value[count][count2] == NULL)
        {
          memcpy(error_message.function[error_message.total],"get_block_template",18);
          memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
          error_message.total++;
          print_error_message;  
          exit(0);
        }
      }       
    } 
    database_multiple_documents_fields.document_count = 0;
    database_multiple_documents_fields.database_fields_count = 0;

    // get the top 100 delegates by total votes
    if (read_multiple_documents_all_fields_from_collection(DATABASE_NAME,"delegates","",&database_multiple_documents_fields,1,BLOCK_VERIFIERS_AMOUNT,1,"total_vote_count",0) == 0)
    {
      SYNC_ALL_BLOCK_VERIFIERS_LIST("Could not get the top 100 delegates for the next round. This means that you will not be able to particpate in the next round");
    }

    // copy the database_multiple_documents_fields to the next, current and previous block verifiers list
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        if (memcmp(database_multiple_documents_fields.item[count][count2],"delegate_name",13) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(current_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(next_block_verifiers_list.block_verifiers_name[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
        }
        if (memcmp(database_multiple_documents_fields.item[count][count2],"public_address",14) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(current_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(next_block_verifiers_list.block_verifiers_public_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
        }
        if (memcmp(database_multiple_documents_fields.item[count][count2],"IP_address",10) == 0)
        {
          memcpy(previous_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(current_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
          memcpy(next_block_verifiers_list.block_verifiers_IP_address[count],database_multiple_documents_fields.value[count][count2],strnlen(database_multiple_documents_fields.value[count][count2],BLOCK_VERIFIERS_NAME_TOTAL_LENGTH+1));
        }
      }
    }

    // reset the database_multiple_documents_fields
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      for (count2 = 0; count2 < 19; count2++)
      {
        pointer_reset(database_multiple_documents_fields.item[count][count2]);
        pointer_reset(database_multiple_documents_fields.value[count][count2]);
      }
    }

    color_print("The previous, current and next block verifiers have been loaded from the database","green");
  }
  
  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_ALL_BLOCK_VERIFIERS_LIST  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_synced_block_verifiers
Description: Gets the synced block verifiers for syncing the database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_synced_block_verifiers()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data is referencing, we need to create a copy to pointer_reset
  char* datacopy = data; 
  size_t count;

  // define macros
  #define GET_SYNCED_BLOCK_VERIFIERS_DATA "{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}"
  
  #define GET_SYNCED_BLOCK_VERIFIERS_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"get_synced_block_verifiers",26); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  #define pointer_reset_all \
  free(datacopy); \
  datacopy = NULL; \
  free(data2); \
  data2 = NULL;

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {    
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"get_synced_block_verifiers",26);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers.synced_block_verifiers_IP_address[count],0,strlen(synced_block_verifiers.synced_block_verifiers_IP_address[count]));
    memset(synced_block_verifiers.vote_settings[count],0,strlen(synced_block_verifiers.vote_settings[count]));
  }
  synced_block_verifiers.vote_settings_true = 0;
  synced_block_verifiers.vote_settings_false = 0;

  start:
  printf("Connecting to a random network data node to get a list of current block verifiers\n");

  // send the message to a random network data node
  do
  {
    count = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);
  } while (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0);
  
  // get the current time
  time(&current_date_and_time);
  current_UTC_date_and_time = gmtime(&current_date_and_time);

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to network data node ",32);
  memcpy(data+32,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
  memcpy(data+strlen(data)," and sending NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\n",73);
  memcpy(data+strlen(data),asctime(current_UTC_date_and_time),strlen(asctime(current_UTC_date_and_time)));
  printf("%s\n",data);
  memset(data,0,strlen(data));

  if (send_and_receive_data_socket(data2,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,GET_SYNCED_BLOCK_VERIFIERS_DATA,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data,"Could not receive data from network data node ",46);
    memcpy(data+46,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    color_print(data,"red");
    memset(data,0,strlen(data));
    printf("Connecting to a different network data node\n\n");
    goto start;
  }

  if (verify_data(data2,0,0) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not verify data");
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(data2,"block_verifiers_public_address_list",data) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not parse the message");
  } 

  // parse the block verifiers public addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers.synced_block_verifiers_public_address[count],data,strnlen(data,BUFFER_SIZE) - strnlen(strstr(data,"|"),BUFFER_SIZE));
    data = strstr(data,"|") + 1;
  }

  // parse the message
  memset(data,0,strlen(data));
  if (parse_json_data(data2,"block_verifiers_IP_address_list",data) == 0)
  {
    GET_SYNCED_BLOCK_VERIFIERS_ERROR("Could not parse the message");
  } 

  // parse the block verifiers IP addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers.synced_block_verifiers_IP_address[count],data,strnlen(data,BUFFER_SIZE) - strnlen(strstr(data,"|"),BUFFER_SIZE));
    data = strstr(data,"|") + 1;
  }

  pointer_reset_all;
  return 1;

  #undef GET_SYNCED_BLOCK_VERIFIERS_DATA
  #undef GET_SYNCED_BLOCK_VERIFIERS_ERROR
  #undef pointer_reset_all
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_proofs_database
Description: Checks if the block verifier needs to sync the reserve proofs database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_proofs_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* reserve_proofs_database = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  size_t count;
  size_t counter;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(reserve_proofs_database); \
  reserve_proofs_database = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;
  
  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || reserve_proofs_database == NULL || data == NULL || data2 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (reserve_proofs_database != NULL)
    {
      pointer_reset(reserve_proofs_database);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  print_start_message("Checking if the reserve proofs database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_proofs",0) == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"reserve_proofs_data_hash\": \"",139);
  memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
  memcpy(message+strlen(message),"\",\r\n ",5);

  for (count = 1; count <= TOTAL_RESERVE_PROOFS_DATABASES; count++)
  {
    memcpy(message+strlen(message),"\"reserve_proofs_data_hash_",26);
    sprintf(message+strlen(message),"%zu",count);
    memcpy(message+strlen(message),"\": \"",4);
    // get the database data hash for the reserve proofs database
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));  
    memcpy(data2,"reserve_proofs_",15);  
    sprintf(data2+15,"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database");
    }
    memcpy(message+strlen(message),data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n ",5);
  }
  memcpy(message+strlen(message),"}",1);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data");
  }

  // select a random network data node to copy the reserve proofs database hashes from
  counter = (int)(rand() % NETWORK_DATA_NODES_AMOUNT);

  printf("Sending all block verifiers a message to check if the reserve proofs database is synced\n"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
    {
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
      synced_block_verifiers.vote_settings_connection_timeout++;
    }
    else
    {
      if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count],BUFFER_SIZE) == 0)
      {
        memcpy(reserve_proofs_database,data,strnlen(data,BUFFER_SIZE));
      } 
      parse_json_data(data,"reserve_proofs_database",data2);
      memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
      if (memcmp(data2,"true",4) == 0)
      {
        synced_block_verifiers.vote_settings_true++;
      }
      else if (memcmp(data2,"false",5) == 0)
      {
        synced_block_verifiers.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The reserve proofs database is not synced","red");
    if (sync_reserve_proofs_database(reserve_proofs_database) == 0)
    {
      memcpy(error_message.function[error_message.total],"sync_check_reserve_proofs_database",34);
      memcpy(error_message.data[error_message.total],"Could not sync the reserve proofs database",41);
      error_message.total++;
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The reserve proofs database is synced","green");

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_proofs_database
Description: Syncs the reserve proofs database
Paramters:
  RESERVE_PROOFS_DATABASE - The RESERVE_PROOFS_DATABASE data from a random network data node
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_proofs_database(const char* RESERVE_PROOFS_DATABASE)
{
  // Variables
  char* data = (char*)calloc(52428800,sizeof(char));  // 50 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  size_t counter;
  int settings;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_RESERVE_PROOFS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  } 

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL || data3 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"sync_reserve_proofs_database",28);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  start:

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       if their was a lot of connection_timeouts, to where a majority vote could not be calculated, then select a random network data node instead
    */
    if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }
    else
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      for (counter = 0, settings = 0; counter < NETWORK_DATA_NODES_AMOUNT; counter++)
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[counter],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
        {
          settings = 1;
        }
      }
      if (settings != 1)    
      {
        goto start;
      }
    }

  for (count2 = 1; count2 <= TOTAL_RESERVE_PROOFS_DATABASES; count2++)
  {
    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_proofs_",24);
    sprintf(data+strlen(data),"%zu",count2);
    printf("%s\n",data);

    // parse the RESERVE_PROOFS_DATABASE
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    memcpy(data2,"reserve_proofs_database_",24);
    sprintf(data2+24,"%zu",count2);

    if (parse_json_data(RESERVE_PROOFS_DATABASE,data2,data) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
    }
   
    if (memcmp(data,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,strlen(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_proofs_",133);
      sprintf(data2+133,"%zu",count2);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data",0);
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_proofs_database",data3) == 0 || memcmp(data3,"",1) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);
      sprintf(data2+15,"%zu",count2);

      // delete the collection from the database
      delete_collection_from_database(DATABASE_NAME,data2,0);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data,0);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
      color_print(data,"green");
    }
    else
    {
      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," is already synced\n",19);
      color_print(data,"green");
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_RESERVE_PROOFS_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_reserve_bytes_database
Description: Checks if the block verifier needs to sync the reserve bytes database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;
  
  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL || data2 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  print_start_message("Checking if the reserve bytes database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve bytes database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",123);
  memcpy(message+123,data,DATA_HASH_LENGTH);
  memcpy(message+251,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data");
  }

  printf("Sending all block verifiers a message to check if the reserve bytes database is synced\n"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
    {
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
      synced_block_verifiers.vote_settings_connection_timeout++;
    }
    else
    {
      parse_json_data(data,"reserve_bytes_database",data2);
      memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
      if (memcmp(data2,"true",4) == 0)
      {
        synced_block_verifiers.vote_settings_true++;
      }
      else if (memcmp(data2,"false",5) == 0)
      {
        synced_block_verifiers.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The reserve bytes database is not synced","red");
    if (sync_reserve_bytes_database() == 0)
    {
      memcpy(error_message.function[error_message.total],"sync_check_reserve_bytes_database",33);
      memcpy(error_message.data[error_message.total],"Could not sync the reserve bytes database",41);
      error_message.total++;
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The reserve bytes database is synced","green");

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_bytes_database
Description: Syncs the reserve bytes database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_bytes_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(52428800,sizeof(char));  // 50 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  int counter;
  size_t number;
  int settings;
  int settings2 = 0;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_RESERVE_BYTES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL || data2 == NULL || data3 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    if (data3 != NULL)
    {
      pointer_reset(data3);
    }
    memcpy(error_message.function[error_message.total],"sync_reserve_bytes_database",27);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  // get the current reserve bytes database
  sscanf(current_block_height,"%zu", &number);
  number--;
  if (number < XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT)
  {
    SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the current block height",0);
  }
  number = ((number - XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME) + 1;

  // get what reserve bytes database count it has and start at that count to sync
  for (count2 = 1; count2 <= number; count2++)
  {
    memset(data2,0,strlen(data2));
    memcpy(data2+strlen(data2),"reserve_bytes_",14);
    sprintf(data2+strlen(data2),"%zu",count2);    
    if (count_all_documents_in_collection(DATABASE_NAME,data2,0) <= 0)
    {
      break;
    }
  }

  for (; count2 <= number; count2++)
  {
    start:

    /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
       if their was a lot of connection_timeouts, to where a majority vote could not be calculated, then select a random network data node instead
    */
    if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
      {
        goto start;
      }
    }
    else
    {
      count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
      for (counter = 0, settings = 0; counter < NETWORK_DATA_NODES_AMOUNT; counter++)
      {
        if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[counter],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
        {
          settings = 1;
        }
      }
      if (settings != 1)    
      {
        goto start;
      }
    }

    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_bytes_",23);
    sprintf(data+strlen(data),"%zu",count2);
    color_print(data,"green");

    // get the database data hash
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    memcpy(data2+strlen(data2),"reserve_bytes_",14);
    sprintf(data2+strlen(data2),"%zu",count2);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database",0);
    }

    // create the message
    memset(data2,0,strlen(data2));
    memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"reserve_bytes_",128);
    sprintf(data2+128,"%zu",count2);
    memcpy(data2+strlen(data2),"\",\r\n \"data_hash\": \"",19);
    memcpy(data2+strlen(data2),data,DATA_HASH_LENGTH);
    memcpy(data2+strlen(data2),"\",\r\n}",5);

    // sign_data
    if (sign_data(data2,0) == 0)
    { 
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
    }
     
    memset(data,0,strlen(data));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (verify_data(data,0,0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
    }

    // parse the message
    memset(data3,0,strlen(data3));
    if (parse_json_data(data,"reserve_bytes_database",data3) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (memcmp(data3,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,strlen(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"reserve_bytes_",131);
      sprintf(data2+131,"%zu",count2);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
      }

      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_bytes_database",data3) == 0 || memcmp(data3,"",1) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);
      sprintf(data2+14,"%zu",count2);

      // delete the collection from the database
      delete_collection_from_database(DATABASE_NAME,data2,0);

      // add the data to the database
      memset(data,0,strlen(data));
      memcpy(data,data3,strlen(data3)-2);
      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data,0);

      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," has been synced successfully\n",31);
      color_print(data,"green");
    }
    else
    {
      memset(data,0,strlen(data));
      memcpy(data,"reserve_bytes_",14);
      sprintf(data+strlen(data),"%zu",count2);
      memcpy(data+strlen(data)," is already synced\n",19);
      color_print(data,"green");
    }
  }



  // check that the reserve bytes database is synced and if not sync all of the reserve bytes databases
  if (settings2 == 0)
  {
    if (get_synced_block_verifiers() == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the synced block verifiers",0);
    }

    // get the database data hash for the reserve bytes database
    memset(data,0,strlen(data));
    if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes",0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database",0);
    }

    // create the message
    memset(message,0,strlen(message));
    memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",123);
    memcpy(message+123,data,DATA_HASH_LENGTH);
    memcpy(message+strlen(message),"\",\r\n}",5);

    // sign_data
    if (sign_data(message,0) == 0)
    { 
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data",0);
    }

    printf("Sending all block verifiers a message to check if the reserve bytes database is synced\n"); 
 
    for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
    {
      memset(data,0,strlen(data));
      memset(data2,0,strlen(data2));
      if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
      {
        memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
        synced_block_verifiers.vote_settings_connection_timeout++;
      }
      else
      {
        parse_json_data(data,"reserve_bytes_database",data2);
        memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
        if (memcmp(data2,"true",4) == 0)
        {
          synced_block_verifiers.vote_settings_true++;
        }
        else if (memcmp(data2,"false",5) == 0)
        {
          synced_block_verifiers.vote_settings_false++;
        }
      }   
    }

    if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
    {
      color_print("The reserve bytes database is not synced\nSyncing all of the reserve bytes databases","red");
      count2 = 1;
      settings2 = 1;
      goto start;
    }
  }

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_RESERVE_bytes_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_delegates_database
Description: Checks if the block verifier needs to sync the delegates database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_delegates_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;
  
  #define SYNC_CHECK_DELEGATES_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL || data2 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  print_start_message("Checking if the delegates database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",115);
  memcpy(message+115,data,DATA_HASH_LENGTH);
  memcpy(message+243,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sign_data");
  }

  printf("Sending all block verifiers a message to check if the delegates database is synced\n"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
    {
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
      synced_block_verifiers.vote_settings_connection_timeout++;
    }
    else
    {
      parse_json_data(data,"delegates_database",data2);
      memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
      if (memcmp(data2,"true",4) == 0)
      {
        synced_block_verifiers.vote_settings_true++;
      }
      else if (memcmp(data2,"false",5) == 0)
      {
        synced_block_verifiers.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The delegates database is not synced","red");
    if (sync_delegates_database() == 0)
    {
      memcpy(error_message.function[error_message.total],"sync_check_delegates_database",29);
      memcpy(error_message.data[error_message.total],"Could not sync the delegates database",37);
      error_message.total++;
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The delegates database is synced","green");

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_CHECK_DELEGATES_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_delegates_database
Description: Syncs the reserve proofs database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_delegates_database()
{
  // Variables
  char* data = (char*)calloc(10485760,sizeof(char));  // 10 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  int settings;

  // define macros
  #define DATABASE_COLLECTION "delegates"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SYNC_DELEGATES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_delegates_database",23);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }
  
  start:

  /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
     if their was a lot of connection_timeouts, to where a majority vote could not be calculated, then select a random network data node instead
  */
  if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }
  else
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    for (count2 = 0, settings = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count2],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
      {
        settings = 1;
      }
    }
    if (settings != 1)    
    {
      goto start;
    }
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the delegates database",31);
  sprintf(data2+strlen(data),"%zu",count);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database",0);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  sprintf(data2+118,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_DELEGATES_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"delegates_database",data2) == 0 || memcmp(data2,"",1) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,0);

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_DELEGATES_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_check_statistics_database
Description: Checks if the block verifier needs to sync the statistics database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_statistics_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;
  
  #define SYNC_CHECK_STATISTICS_DATABASE_ERROR(settings) \
  memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30); \
  memcpy(error_message.data[error_message.total],settings,strnlen(settings,BUFFER_SIZE_NETWORK_BLOCK_DATA)); \
  error_message.total++; \
  pointer_reset_all; \
  return 0;

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL || data == NULL || data2 == NULL)
  {
    if (message != NULL)
    {
      pointer_reset(message);
    }
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  print_start_message("Checking if the statistics database is synced");

  if (get_synced_block_verifiers() == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the synced block verifiers");
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the delegates database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",116);
  memcpy(message+116,data,DATA_HASH_LENGTH);
  memcpy(message+244,"\",\r\n}",5);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sign_data");
  }

  printf("Sending all block verifiers a message to check if the statistics database is synced\n"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data2,0,strlen(data2));
    if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0 || verify_data(data,0,0) == 0)
    {
      memcpy(synced_block_verifiers.vote_settings[count],"connection_timeout",18);
      synced_block_verifiers.vote_settings_connection_timeout++;
    }
    else
    {
      parse_json_data(data,"statistics_database",data2);
      memcpy(synced_block_verifiers.vote_settings[count],data2,strnlen(data2,BUFFER_SIZE));
      if (memcmp(data2,"true",4) == 0)
      {
        synced_block_verifiers.vote_settings_true++;
      }
      else if (memcmp(data2,"false",5) == 0)
      {
        synced_block_verifiers.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT || synced_block_verifiers.vote_settings_connection_timeout >= BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The statistics database is not synced","red");
    if (sync_statistics_database() == 0)
    {
      memcpy(error_message.function[error_message.total],"sync_check_statistics_database",30);
      memcpy(error_message.data[error_message.total],"Could not sync the statistics database",38);
      error_message.total++;
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The statistics database is synced","green");

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_CHECK_STATISTICS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_statistics_database
Description: Syncs the reserve proofs database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_statistics_database()
{
  // Variables
  char* data = (char*)calloc(10485760,sizeof(char));  // 10 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  int settings;

  // define macros
  #define DATABASE_COLLECTION "statistics"
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SYNC_STATISTICS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24); \
    memcpy(error_message.data[error_message.total],message,strnlen(message,BUFFER_SIZE)); \
    error_message.total++; \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",37); \
    color_print(data,"red"); \
    goto start; \
  }  

  // check if the memory needed was allocated on the heap successfully
  if (data == NULL || data2 == NULL)
  {
    if (data != NULL)
    {
      pointer_reset(data);
    }
    if (data2 != NULL)
    {
      pointer_reset(data2);
    }
    memcpy(error_message.function[error_message.total],"sync_statistics_database",24);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }
  
  start:

  /* select a random block verifier from the majority vote settings to sync the database from, making sure not to select your own block verifier node
     if their was a lot of connection_timeouts, to where a majority vote could not be calculated, then select a random network data node instead
  */
  if (synced_block_verifiers.vote_settings_connection_timeout < BLOCK_VERIFIERS_AMOUNT - BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    if (memcmp(synced_block_verifiers.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }
  }
  else
  {
    count = (int)(rand() % BLOCK_VERIFIERS_AMOUNT-1);
    for (count2 = 0, settings = 0; count2 < NETWORK_DATA_NODES_AMOUNT; count2++)
    {
      if (strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],network_data_nodes_list.network_data_nodes_IP_address[count2],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0 && strncmp(synced_block_verifiers.synced_block_verifiers_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) != 0)
      {
        settings = 1;
      }
    }
    if (settings != 1)    
    {
      goto start;
    }
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers.synced_block_verifiers_IP_address[count],strnlen(synced_block_verifiers.synced_block_verifiers_IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the statistics database",32);
  sprintf(data2+strlen(data),"%zu",count);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,DATABASE_COLLECTION,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database",0);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  sprintf(data2+118,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_STATISTICS_DATABASE_ERROR("Could not sign_data",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers.synced_block_verifiers_IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"statistics_database",data2) == 0 || memcmp(data2,"",1) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  // delete the collection from the database
  delete_collection_from_database(DATABASE_NAME,DATABASE_COLLECTION,0);

  // add the data to the database
  memset(data,0,strlen(data));
  memcpy(data,data2,strlen(data2)-2);
  insert_multiple_documents_into_collection_json(DATABASE_NAME,DATABASE_COLLECTION,data,0);

  pointer_reset_all;
  return 1;

  #undef DATABASE_COLLECTION
  #undef pointer_reset_all
  #undef SYNC_STATISTICS_DATABASE_ERROR   
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_delegate_online_status
Description: Sends data to a socket
Parameters:
  HOST - The IP address to connect to
Return: 1 if the IP address is online, 0 if the IP address is offline
-----------------------------------------------------------------------------------------------------------
*/

int get_delegate_online_status(const char* HOST)
{
  // Constants
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);

  // Variables
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  // check if the memory needed was allocated on the heap successfully
  if (str == NULL)
  {    
    memcpy(error_message.function[error_message.total],"get_delegate_online_status",26);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  } 

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    memcpy(str,"Error creating socket for sending data to ",42);
    memcpy(str+42,HOST,HOST_LENGTH);
    memcpy(error_message.function[error_message.total],"get_delegate_online_status",26);
    memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    error_message.total++; 
    pointer_reset(str);
    return 0;
  }

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(HOST); 
  if (HOST_NAME == NULL)
  {
    close(SOCKET);
    memcpy(str,"Error invalid hostname of ",26);
    memcpy(str+26,HOST,HOST_LENGTH);
    memcpy(error_message.function[error_message.total],"get_delegate_online_status",26);
    memcpy(error_message.data[error_message.total],str,strnlen(str,BUFFER_SIZE_NETWORK_BLOCK_DATA));
    error_message.total++;
    pointer_reset(str);
    return 0;
  }  
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(SEND_DATA_PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings == 0)
      {        
        close(SOCKET);
        return 1;
      } 
    }
  }

  close(SOCKET);
  return 0;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
}