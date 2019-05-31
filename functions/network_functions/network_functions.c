#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>

#include "define_macro_functions.h"
#include "define_macros.h"

#include "define_macros_functions.h"
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
    color_print("Could not allocate the memory needed on the heap","red");
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
      color_print("Error creating socket for sending a post request","red");
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
      color_print("Error setting socket timeout for sending a post request","red"); 
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
      color_print(str,"red"); 
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
          color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
    color_print("Could not allocate the memory needed on the heap","red");
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
      color_print(str,"red");
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
      color_print(str,"red");
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
      color_print(str,"red");
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
          color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
  TITLE - A summary of the data sent to the host. This text gets printed to the console
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data_socket(const char* HOST, const int PORT, const char* DATA, const char* TITLE, const int MESSAGE_SETTINGS)
{ 
  // Constants
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);
  const struct timeval SOCKET_TIMEOUT = {SOCKET_DATA_TIMEOUT_SETTINGS, 0};   
  
  // Variables 
  char buffer2[BUFFER_SIZE];
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
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
    color_print("Could not allocate the memory needed on the heap","red");
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
      color_print(str,"red");
    }
    close(SOCKET);
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
      color_print(str,"red");
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
      color_print(str,"red");
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
          color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
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
      color_print(str,"red"); 
    }
    close(SOCKET);
    pointer_reset_all;
    return 0;
  }
    
  close(SOCKET);
  pointer_reset_all;
  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
  #undef pointer_reset_all
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
    memcpy(data+strnlen(data,BUFFER_SIZE),SOCKET_END_STRING,5);
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
    color_print("Could not allocate the memory needed on the heap","red");
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
        memcpy(data,message,strnlen(message,BUFFER_SIZE) - 5);
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
Name: sync_check_reserve_proofs_database
Description: Checks if the block verifier needs to sync the reserve proofs database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_proofs_database()
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers_IP_addresses.IP_address[count],0,strlen(synced_block_verifiers_IP_addresses.IP_address[count]));
    memset(synced_block_verifiers_IP_addresses.vote_settings[count],0,strlen(synced_block_verifiers_IP_addresses.vote_settings[count]));
  }
  synced_block_verifiers_IP_addresses.vote_settings_true = 0;
  synced_block_verifiers_IP_addresses.vote_settings_false = 0;

  color_print("Connecting to a random network data node to get a list of current block verifiers","green");

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}",89);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_reserve_proofs_database");
  }

  start:

  // send the message to a random network data node
  count = (int)((rand() % (NETWORK_DATA_NODES_AMOUNT - 1 + 1)) + 1);
  if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }
  memset(data3,0,strlen(data3));

  if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data2,"Could not receive data from network data node ",46);
    memcpy(data2,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data2,"\nConnecting to a different network data node",44);
    color_print(data2,"red");
    memset(data2,0,strlen(data2));
    goto start;
  }

  if (verify_data(data3,0,0,0) == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data\nFunction: sync_check_reserve_proofs_database");
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data3,"block_verifiers_list",data2) == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not parse the message\nFunction: sync_check_reserve_proofs_database");
  } 

  // parse the block verifiers IP addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers_IP_addresses.IP_address[count],data2,strnlen(strstr(data2,"|"),BUFFER_SIZE));
    data2 = strstr(data2,"|") + 1;
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_proofs",0) == 0)
  {
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database\nFunction: sync_check_reserve_proofs_database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",124);
  memcpy(message+124,data,DATA_HASH_LENGTH);
  memcpy(message+252,"\"}",2);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_reserve_proofs_database");
  }

  color_print("Sending all block verifiers a message to check if the reserve proof database is synced","green"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data3,0,strlen(data3));
    send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0);
    if (verify_data(data,0,0,0) == 0)
    {
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4);
      synced_block_verifiers_IP_addresses.vote_settings_true++;
    }
    else
    {
      parse_json_data(data,"reserve_proofs_database",data3);
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],data3,strnlen(data3,BUFFER_SIZE));
      if (memcmp(data3,"true",4) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_true++;
      }
      else if (memcmp(data3,"false",5) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers_IP_addresses.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The reserve proof database is not synced","red");
    if (sync_reserve_proofs_database() == 0)
    {
      color_print("Could not sync the reserve proof database","red");
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The reserve proof database is synced","green");

  pointer_reset_all;
  return 1;

  #undef pointer_reset_all
  #undef SYNC_CHECK_RESERVE_PROOFS_DATABASE_ERROR  
}



/*
-----------------------------------------------------------------------------------------------------------
Name: sync_reserve_proofs_database
Description: Syncs the reserve proofs database
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_proofs_database()
{
  // Variables
  char* data = (char*)calloc(52428800,sizeof(char));  // 50 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;

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
    color_print(message,"red"); \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",36); \
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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  for (count2 = 1; count2 <= 50; count2++)
  {
    start:

    // select a random block verifier from the majority vote settings to sync the database from
    count = (int)(rand() % 99);
    if (memcmp(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers_IP_addresses.IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }

    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_proofs_",24);
    sprintf(data2+strlen(data),"%zu",count);
    color_print(data,"green");

    // get the database data hash
    memset(data,0,strlen(data));
    memset(data2+strlen(data2),0,strlen(data2));
    memcpy(data2+strlen(data2),"reserve_proofs_",15);
    sprintf(data2+strlen(data2),"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not get the database data hash for the reserve proofs database\nFunction: sync_reserve_proofs_database",0);
    }

    // create the message
    memset(data2,0,strlen(data2));
    memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"",115);
    sprintf(data2+115,"%zu",count2);
    memcpy(data2+strlen(data2),"\",\r\n \"data_hash\": \"",19);
    memcpy(data2+strlen(data2),data,DATA_HASH_LENGTH);
    memcpy(data2+strlen(data2),"\",\r\n}",5);

    // sign_data
    if (sign_data(data2,0) == 0)
    { 
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data\nFunction: sync_reserve_proofs_database",0);
    }
     
    memset(data,0,strlen(data));
    if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (verify_data(data,0,0,0) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
    }

    // parse the message
    memset(data3,0,strlen(data3));
    if (parse_json_data(data,"reserve_proofs_database",data3) == 0)
    {
      SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (memcmp(data3,"false",5) == 0)
    {
      // sync the database
      memset(data,0,strlen(data));
      memcpy(data,"reserve_proofs_",15);
      sprintf(data+strlen(data),"%zu",count);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,strlen(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_PROOFS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
      sprintf(data2+118,"%zu",count);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not sign_data\nFunction: sync_reserve_proofs_database",0);
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0,0) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_proofs_database",data3) == 0)
      {
        SYNC_RESERVE_PROOFS_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_proofs_",15);
      sprintf(data2+15,"%zu",count);

      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data3,0);
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
Parameters:
  BLOCK_HEIGHT - The block height
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_check_reserve_bytes_database(const char* BLOCK_HEIGHT)
{
  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers_IP_addresses.IP_address[count],0,strlen(synced_block_verifiers_IP_addresses.IP_address[count]));
    memset(synced_block_verifiers_IP_addresses.vote_settings[count],0,strlen(synced_block_verifiers_IP_addresses.vote_settings[count]));
  }
  synced_block_verifiers_IP_addresses.vote_settings_true = 0;
  synced_block_verifiers_IP_addresses.vote_settings_false = 0;

  color_print("Connecting to a random network data node to get a list of current block verifiers","green");

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}",89);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_reserve_bytes_database");
  }

  start:

  // send the message to a random network data node
  count = (int)((rand() % (NETWORK_DATA_NODES_AMOUNT - 1 + 1)) + 1);
  if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }
  memset(data3,0,strlen(data3));

  if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data2,"Could not receive data from network data node ",46);
    memcpy(data2,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data2,"\nConnecting to a different network data node",44);
    color_print(data2,"red");
    memset(data2,0,strlen(data2));
    goto start;
  }

  if (verify_data(data3,0,0,0) == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not verify data\nFunction: sync_check_reserve_bytes_database");
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data3,"block_verifiers_list",data2) == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not parse the message\nFunction: sync_check_reserve_bytes_database");
  } 

  // parse the block verifiers IP addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers_IP_addresses.IP_address[count],data2,strnlen(strstr(data2,"|"),BUFFER_SIZE));
    data2 = strstr(data2,"|") + 1;
  }

  // get the database data hash for the reserve bytes database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"reserve_bytes",0) == 0)
  {
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database\nFunction: sync_check_reserve_bytes_database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_ALL_UPDATE\",\r\n \"data_hash\": \"",123);
  memcpy(message+123,data,DATA_HASH_LENGTH);
  memcpy(message+251,"\"}",2);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_reserve_bytes_database");
  }

  color_print("Sending all block verifiers a message to check if the reserve bytes database is synced","green"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data3,0,strlen(data3));
    send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0);
    if (verify_data(data,0,0,0) == 0)
    {
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4);
      synced_block_verifiers_IP_addresses.vote_settings_true++;
    }
    else
    {
      parse_json_data(data,"reserve_bytes_database",data3);
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],data3,strnlen(data3,BUFFER_SIZE));
      if (memcmp(data3,"true",4) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_true++;
      }
      else if (memcmp(data3,"false",5) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers_IP_addresses.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The reserve bytes database is not synced","red");
    if (sync_reserve_bytes_database(BLOCK_HEIGHT) == 0)
    {
      color_print("Could not sync the reserve bytes database","red");
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
Parameters:
  BLOCK_HEIGHT - The block height
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sync_reserve_bytes_database(const char* BLOCK_HEIGHT)
{
  // Variables
  char* data = (char*)calloc(52428800,sizeof(char));  // 50 MB
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  size_t count2;
  size_t number;

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_RESERVE_BYTES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    color_print(message,"red"); \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",36); \
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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // get the current reserve bytes database
  sscanf(BLOCK_HEIGHT,"%zu", &number);
  number = (XCASH_PROOF_OF_STAKE_BLOCK_HEIGHT - number) / BLOCKS_PER_DAY_FIVE_MINUTE_BLOCK_TIME;

  for (count2 = 0; count2 <= number; count2++)
  {
    start:

    // select a random block verifier from the majority vote settings to sync the database from
    count = (int)(rand() % 99);
    if (memcmp(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers_IP_addresses.IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
    {
      goto start;
    }

    memset(data,0,strlen(data));
    memcpy(data,"Connecting to block verifier ",29);
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE));
    memcpy(data+strlen(data)," to sync reserve_bytes_",23);
    sprintf(data2+strlen(data),"%zu",count);
    color_print(data,"green");

    // get the database data hash
    memset(data,0,strlen(data));
    memset(data2+strlen(data2),0,strlen(data2));
    memcpy(data2+strlen(data2),"reserve_bytes_",14);
    sprintf(data2+strlen(data2),"%zu",count);
    if (get_database_data_hash(data,DATABASE_NAME,data2,0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not get the database data hash for the reserve bytes database\nFunction: sync_reserve_bytes_database",0);
    }

    // create the message
    memset(data2,0,strlen(data2));
    memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"file\": \"",114);
    sprintf(data2+114,"%zu",count2);
    memcpy(data2+strlen(data2),"\",\r\n \"data_hash\": \"",19);
    memcpy(data2+strlen(data2),data,DATA_HASH_LENGTH);
    memcpy(data2+strlen(data2),"\",\r\n}",5);

    // sign_data
    if (sign_data(data2,0) == 0)
    { 
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data\nFunction: sync_reserve_bytes_database",0);
    }
     
    memset(data,0,strlen(data));
    if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
    {
      SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
    }

    if (verify_data(data,0,0,0) == 0)
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
      sprintf(data+strlen(data),"%zu",count);
      memcpy(data+strlen(data)," is not synced, downloading it from ",36);
      memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],36);
      color_print(data,"red");

      // create the message
      memset(data2,0,strlen(data2));
      memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_RESERVE_BYTES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",117);
      sprintf(data2+117,"%zu",count);
      memcpy(data2+strlen(data2),"\",\r\n}",5);

      // sign_data
      if (sign_data(data2,0) == 0)
      { 
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not sign_data\nFunction: sync_reserve_bytes_database",0);
      }
     
      memset(data,0,strlen(data));
      if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      if (verify_data(data,0,0,0) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not verify data from ",1);
      }

      // parse the message
      memset(data3,0,strlen(data3));
      if (parse_json_data(data,"reserve_bytes_database",data3) == 0)
      {
        SYNC_RESERVE_BYTES_DATABASE_ERROR("Could not receive data from ",1);
      }

      // add the data to the database
      memset(data2,0,strlen(data2));
      memcpy(data2,"reserve_bytes_",14);
      sprintf(data2+15,"%zu",count);

      insert_multiple_documents_into_collection_json(DATABASE_NAME,data2,data3,0);
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
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_CHECK_DELEGATES_DATABASE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers_IP_addresses.IP_address[count],0,strlen(synced_block_verifiers_IP_addresses.IP_address[count]));
    memset(synced_block_verifiers_IP_addresses.vote_settings[count],0,strlen(synced_block_verifiers_IP_addresses.vote_settings[count]));
  }
  synced_block_verifiers_IP_addresses.vote_settings_true = 0;
  synced_block_verifiers_IP_addresses.vote_settings_false = 0;

  color_print("Connecting to a random network data node to get a list of current block verifiers","green");

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}",89);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_delegates_database");
  }

  start:

  // send the message to a random network data node
  count = (int)((rand() % (NETWORK_DATA_NODES_AMOUNT - 1 + 1)) + 1);
  if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }
  memset(data3,0,strlen(data3));

  if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data2,"Could not receive data from network data node ",46);
    memcpy(data2,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data2,"\nConnecting to a different network data node",44);
    color_print(data2,"red");
    memset(data2,0,strlen(data2));
    goto start;
  }

  if (verify_data(data3,0,0,0) == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not verify data\nFunction: sync_check_delegates_database");
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data3,"block_verifiers_list",data2) == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not parse the message\nFunction: sync_check_delegates_database");
  } 

  // parse the block verifiers IP addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers_IP_addresses.IP_address[count],data2,strnlen(strstr(data2,"|"),BUFFER_SIZE));
    data2 = strstr(data2,"|") + 1;
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"delegates",0) == 0)
  {
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database\nFunction: sync_check_delegates_database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",115);
  memcpy(message+115,data,DATA_HASH_LENGTH);
  memcpy(message+243,"\"}",2);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_DELEGATES_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_delegates_database");
  }

  color_print("Sending all block verifiers a message to check if the delegates database is synced","green"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data3,0,strlen(data3));
    send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0);
    if (verify_data(data,0,0,0) == 0)
    {
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4);
      synced_block_verifiers_IP_addresses.vote_settings_true++;
    }
    else
    {
      parse_json_data(data,"delegates_database",data3);
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],data3,strnlen(data3,BUFFER_SIZE));
      if (memcmp(data3,"true",4) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_true++;
      }
      else if (memcmp(data3,"false",5) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers_IP_addresses.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The delegates database is not synced","red");
    if (sync_delegates_database() == 0)
    {
      color_print("Could not sync the delegates database","red");
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The delegates database is synced","green");

  pointer_reset_all;
  return 1;

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

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SYNC_DELEGATES_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    color_print(message,"red"); \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",36); \
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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }
  
  start:

  // select a random block verifier from the majority vote settings to sync the database from
  count = (int)(rand() % 99);
  if (memcmp(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers_IP_addresses.IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the delegates database",31);
  sprintf(data2+strlen(data),"%zu",count);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,"delegates",0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not get the database data hash for the delegates database\nFunction: sync_delegates_database",0);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_DELEGATES_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  sprintf(data2+118,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_DELEGATES_DATABASE_ERROR("Could not sign_data\nFunction: sync_delegates_database",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0,0) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"delegates_database",data2) == 0)
  {
    SYNC_DELEGATES_DATABASE_ERROR("Could not receive data from ",1);
  }

  // add the data to the database
  insert_multiple_documents_into_collection_json(DATABASE_NAME,"delegates",data2,0);

  pointer_reset_all;
  return 1;

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
  char* data3 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  // since were going to be changing where data2 is referencing, we need to create a copy to pointer_reset
  char* datacopy = data2; 
  size_t count;

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \
  free(datacopy); \
  datacopy = NULL; \
  free(data3); \
  data3 = NULL;
  
  #define SYNC_CHECK_STATISTICS_DATABASE_ERROR(settings) \
  color_print(settings,"red"); \
  pointer_reset_all; \
  return 0;

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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }

  // reset the block_verifiers_IP_addresses 
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(synced_block_verifiers_IP_addresses.IP_address[count],0,strlen(synced_block_verifiers_IP_addresses.IP_address[count]));
    memset(synced_block_verifiers_IP_addresses.vote_settings[count],0,strlen(synced_block_verifiers_IP_addresses.vote_settings[count]));
  }
  synced_block_verifiers_IP_addresses.vote_settings_true = 0;
  synced_block_verifiers_IP_addresses.vote_settings_false = 0;

  color_print("Connecting to a random network data node to get a list of current block verifiers","green");

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_NETWORK_DATA_NODES_GET_CURRENT_BLOCK_VERIFIERS_LIST\",\r\n}",89);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_statistics_database");
  }

  start:

  // send the message to a random network data node
  count = (int)((rand() % (NETWORK_DATA_NODES_AMOUNT - 1 + 1)) + 1);
  if (strncmp(network_data_nodes_list.network_data_nodes_IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }
  memset(data3,0,strlen(data3));

  if (send_and_receive_data_socket(data3,network_data_nodes_list.network_data_nodes_IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    memcpy(data2,"Could not receive data from network data node ",46);
    memcpy(data2,network_data_nodes_list.network_data_nodes_IP_address[count],strnlen(network_data_nodes_list.network_data_nodes_IP_address[count],BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH));
    memcpy(data2,"\nConnecting to a different network data node",44);
    color_print(data2,"red");
    memset(data2,0,strlen(data2));
    goto start;
  }

  if (verify_data(data3,0,0,0) == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not verify data\nFunction: sync_check_statistics_database");
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data3,"block_verifiers_list",data2) == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not parse the message\nFunction: sync_check_statistics_database");
  } 

  // parse the block verifiers IP addresses
  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memcpy(synced_block_verifiers_IP_addresses.IP_address[count],data2,strnlen(strstr(data2,"|"),BUFFER_SIZE));
    data2 = strstr(data2,"|") + 1;
  }

  // get the database data hash for the reserve proofs database
  memset(data,0,strlen(data));
  if (get_database_data_hash(data,DATABASE_NAME,"statistics",0) == 0)
  {
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database\nFunction: sync_check_statistics_database");
  }

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_SYNC_CHECK_UPDATE\",\r\n \"data_hash\": \"",116);
  memcpy(message+116,data,DATA_HASH_LENGTH);
  memcpy(message+244,"\"}",2);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    SYNC_CHECK_STATISTICS_DATABASE_ERROR("Could not sign_data\nFunction: sync_check_statistics_database");
  }

  color_print("Sending all block verifiers a message to check if the statistics database is synced","green"); 

  for (count = 0; count < BLOCK_VERIFIERS_AMOUNT; count++)
  {
    memset(data,0,strlen(data));
    memset(data3,0,strlen(data3));
    send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"",0);
    if (verify_data(data,0,0,0) == 0)
    {
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4);
      synced_block_verifiers_IP_addresses.vote_settings_true++;
    }
    else
    {
      parse_json_data(data,"statistics_database",data3);
      memcpy(synced_block_verifiers_IP_addresses.vote_settings[count],data3,strnlen(data3,BUFFER_SIZE));
      if (memcmp(data3,"true",4) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_true++;
      }
      else if (memcmp(data3,"false",5) == 0)
      {
        synced_block_verifiers_IP_addresses.vote_settings_false++;
      }
    }   
  }

  if (synced_block_verifiers_IP_addresses.vote_settings_false >= BLOCK_VERIFIERS_VALID_AMOUNT)
  {
    color_print("The statistics database is not synced","red");
    if (sync_statistics_database() == 0)
    {
      color_print("Could not sync the statistics database","red");
      pointer_reset_all;
      return 0;
    }
  }

  color_print("The statistics database is synced","green");

  pointer_reset_all;
  return 1;

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

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL;

  #define SYNC_STATISTICS_DATABASE_ERROR(message,settings) \
  if (settings == 0) \
  { \
    color_print(message,"red"); \
    pointer_reset_all; \
    return 0; \
  } \
  else \
  { \
    memset(data,0,strlen(data)); \
    memcpy(data+strlen(data),message,strnlen(message,BUFFER_SIZE)); \
    memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE)); \
    memcpy(data+strlen(data),"\nConnecting to another block verifier",36); \
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
    color_print("Could not allocate the memory needed on the heap","red");
    exit(0);
  }
  
  start:

  // select a random block verifier from the majority vote settings to sync the database from
  count = (int)(rand() % 99);
  if (memcmp(synced_block_verifiers_IP_addresses.vote_settings[count],"true",4) != 0 || strncmp(synced_block_verifiers_IP_addresses.IP_address[count],block_verifiers_IP_address,BLOCK_VERIFIERS_IP_ADDRESS_TOTAL_LENGTH) == 0)
  {
    goto start;
  }

  memset(data,0,strlen(data));
  memcpy(data,"Connecting to block verifier ",29);
  memcpy(data+strlen(data),synced_block_verifiers_IP_addresses.IP_address[count],strnlen(synced_block_verifiers_IP_addresses.IP_address[count],BUFFER_SIZE));
  memcpy(data+strlen(data)," to sync the statistics database",31);
  sprintf(data2+strlen(data),"%zu",count);
  color_print(data,"green");

  // get the database data hash
  if (get_database_data_hash(data,DATABASE_NAME,"statistics",0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not get the database data hash for the statistics database\nFunction: sync_statistics_database",0);
  }

  // create the message
  memset(data2,0,strlen(data2));
  memcpy(data2,"{\r\n \"message_settings\": \"BLOCK_VERIFIERS_TO_BLOCK_VERIFIERS_STATISTICS_DATABASE_DOWNLOAD_FILE_UPDATE\",\r\n \"file\": \"",118);
  sprintf(data2+118,"%zu",count);
  memcpy(data2+strlen(data2),"\",\r\n}",5);

  // sign_data
  if (sign_data(data2,0) == 0)
  { 
    SYNC_STATISTICS_DATABASE_ERROR("Could not sign_data\nFunction: sync_statistics_database",0);
  }
     
  memset(data,0,strlen(data));
  if (send_and_receive_data_socket(data,synced_block_verifiers_IP_addresses.IP_address[count],SEND_DATA_PORT,data2,TOTAL_CONNECTION_TIME_SETTINGS,"",0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  if (verify_data(data,0,0,0) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not verify data from ",1);
  }

  // parse the message
  memset(data2,0,strlen(data2));
  if (parse_json_data(data,"statistics_database",data2) == 0)
  {
    SYNC_STATISTICS_DATABASE_ERROR("Could not receive data from ",1);
  }

  // add the data to the database
  insert_multiple_documents_into_collection_json(DATABASE_NAME,"statistics",data2,0);

  pointer_reset_all;
  return 1;

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

int get_delegate_online_status(char* HOST)
{
  // Variables
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(socket_settings);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  SOCK_NONBLOCK = Set the socket to non blocking mode, so it will use the timeout settings
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  if (SOCKET == -1)
  { 
    return 0;
  }

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(HOST); 
  if (HOST_NAME == NULL)
  {
    close(SOCKET);
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