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
  const size_t DATA_LENGTH = strlen(DATA);
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE); 

  // Variables
  char response[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char* post_request_data;
  char str[BUFFER_SIZE]; 
  char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  size_t count; 
  size_t counter = 0; 
  size_t receive_data_result; 
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(int);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  // check if the memory needed was allocated on the heap successfully
  if (message == NULL)
  {
    memcpy(error_message.function[error_message.total],"send_http_request",17);
    memcpy(error_message.data[error_message.total],"Could not allocate the memory needed on the heap",48);
    error_message.total++;
    print_error_message;  
    exit(0);
  }

  memset(str,0,sizeof(str));

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
    snprintf(str, sizeof(str)-1, "%zu", strlen(DATA));
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
    pointer_reset(message);
    return 0;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(const struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],"Error setting socket timeout for sending a post request",55);
      error_message.total++;       
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }  

  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.","");
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {       
      memset(str,0,sizeof(str));
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,HOST,strnlen(HOST,BUFFER_SIZE));
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }

  // get a random IP address to connect to from the host if they have multiple IP addresses
  count = 0;
  while(HOST_NAME->h_addr_list[count] != NULL)
  {  
    count++;
  }

  if (count > 0)
  {
    count = rand() % count;
  }

  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT);  

  // get the length of buffer2 and host, since they will not change at this point and we need them for faster string copying
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[count])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {        
        if (MESSAGE_SETTINGS == 1)
        {
          memset(str,0,sizeof(str));
          memcpy(str,"Error connecting to ",20);
          memcpy(str+20,HOST,HOST_LENGTH);
          memcpy(str+20+HOST_LENGTH," on port ",9);
          memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
          memcpy(error_message.function[error_message.total],"send_http_request",17);
          memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
          error_message.total++; 
        }
        pointer_reset(message);
        close(SOCKET);
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
      memset(str,0,sizeof(str));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,sizeof(str));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }

  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,sizeof(str));
    memcpy(str,"Connected to ",13);
    memcpy(str+13,HOST,HOST_LENGTH);
    memcpy(str+13+HOST_LENGTH," on port ",9);
    memcpy(str+22+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green"); 
  }
    
  // send the message
  if (MESSAGE_SETTINGS == 1)
  {
    fprintf(stderr,"Sending %s to %s on port %s\r\n",TITLE,HOST,buffer2);
  }
  if (send_data(SOCKET,(unsigned char*)message,0,0,"") == 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,sizeof(str));
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,HOST,HOST_LENGTH);
      memcpy(str+22+HOST_LENGTH," on port ",9);
      memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }
   
  // get the result
  receive_data_result = receive_data(SOCKET,message,"{",1,DATA_TIMEOUT_SETTINGS);
  if (receive_data_result < 2)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,sizeof(str));
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
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }

  // check if the data recived is correct
  if (strstr(message,"{") == NULL && strstr(message,"error") == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memset(str,0,sizeof(str));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(str+26+HOST_LENGTH," on port ",9);
      memcpy(str+35+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_http_request",17);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;  
    }
    pointer_reset(message);
    close(SOCKET);
    return 0;
  }
  if (MESSAGE_SETTINGS == 1)
  {
    memset(str,0,sizeof(str));
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
    memcpy(result+strlen(result),post_request_data,strnlen(post_request_data,BUFFER_SIZE));
  }
  else
  {
    // the HTTP result does not contain a header
    memcpy(result+strlen(result),message,strnlen(message,BUFFER_SIZE));
  }
  pointer_reset(message);
  close(SOCKET);
  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
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
  char str[BUFFER_SIZE];
  char message[BUFFER_SIZE]; 
  size_t count;
  int receive_data_result;
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(int);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  memset(str,0,sizeof(str));

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
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++; 
    }
    return 0;
  }

  /* Set the socket options for sending and receiving data
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket on receiving data, to use the timeout settings
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(const struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error setting socket timeout for sending data to ",49);
      memcpy(str+49,HOST,HOST_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;
    }
    close(SOCKET);
    return 0;
  } 

  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.","");
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,HOST,HOST_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;
    }
    close(SOCKET);
    return 0;
  }

  // get a random IP address to connect to from the host if they have multiple IP addresses
  count = 0;
  while(HOST_NAME->h_addr_list[count] != NULL)
  {  
    count++;
  }

  if (count > 0)
  {
    count = rand() % count;
  }
    
  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[count])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {        
        if (MESSAGE_SETTINGS == 1)
        {
          memcpy(str,"Error connecting to ",20);
          memcpy(str+20,HOST,HOST_LENGTH);
          memcpy(str+20+HOST_LENGTH," on port ",9);
          memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
          memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
          memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
          error_message.total++;
        }
        close(SOCKET);
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
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;
    }
    close(SOCKET);
    return 0;
  }

  // set the socket to blocking mode
  socket_settings &= (~O_NONBLOCK);
  if (fcntl(SOCKET, F_SETFL, socket_settings) == -1)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++; 
    }
    close(SOCKET);
    return 0;
  }

  if (MESSAGE_SETTINGS == 1)
  {
    memcpy(str,"Connected to ",13);
    memcpy(str+13,HOST,HOST_LENGTH);
    memcpy(str+13+HOST_LENGTH," on port ",9);
    memcpy(str+22+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green"); 
  }

  // send the message 
  if (MESSAGE_SETTINGS == 1)
  {
    fprintf(stderr,"Sending %s to %s on port %s\r\n",TITLE,HOST,buffer2);
  }
  memset(message,0,sizeof(message));
  memcpy(message,DATA,strnlen(DATA,sizeof(message)));
  if (send_data(SOCKET,(unsigned char*)message,0,1,"") == 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,HOST,HOST_LENGTH);
      memcpy(str+22+HOST_LENGTH," on port ",9);
      memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      memcpy(error_message.function[error_message.total],"send_and_receive_data_socket",28);
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;
    }
    close(SOCKET);
    return 0;
  }
    
  // get the result
  memset(result,0,strnlen(result,BUFFER_SIZE));
  receive_data_result = receive_data(SOCKET,result,SOCKET_END_STRING,1,DATA_TIMEOUT_SETTINGS);
  if (receive_data_result < 2)
  {
    if (MESSAGE_SETTINGS == 1)
    {
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
      memcpy(error_message.data[error_message.total],str,strnlen(str,sizeof(error_message.data[error_message.total])));
      error_message.total++;
    }
    close(SOCKET);
    return 0;
  }
     
  if (MESSAGE_SETTINGS == 1)
  {
    memcpy(str,"Received data from ",19);
    memcpy(str+19,HOST,HOST_LENGTH);
    memcpy(str+19+HOST_LENGTH," on port ",9);
    memcpy(str+28+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    color_print(str,"green");
  }
  
  close(SOCKET);
  return 1;

  #undef SOCKET_FILE_DESCRIPTORS_LENGTH
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
  size_t count;
  const struct timeval SOCKET_TIMEOUT = {SOCKET_DATA_TIMEOUT_SETTINGS, 0};   
  
  // Variables  
  char buffer2[BUFFER_SIZE];
  char str[BUFFER_SIZE];
  char message[BUFFER_SIZE];
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(int);

  // define macros
  #define SEND_DATA_SOCKET_ERROR(message) \
  memcpy(error_message.function[error_message.total],"send_data_socket",16); \
  memcpy(error_message.data[error_message.total],message,strnlen(message,sizeof(error_message.data[error_message.total]))); \
  error_message.total++; \
  close(SOCKET); \
  return 0;

  memset(str,0,sizeof(str));
  
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
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(const struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) != 0)
  {   
    memcpy(str,"Error setting socket timeout for sending data to ",49);
    memcpy(str+49,HOST,HOST_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  } 

  // convert the hostname if used, to an IP address
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.","");
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {    
    memcpy(str,"Error invalid hostname of ",26);
    memcpy(str+26,HOST,HOST_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }

  // get a random IP address to connect to from the host if they have multiple IP addresses
  count = 0;
  while(HOST_NAME->h_addr_list[count] != NULL)
  {  
    count++;
  }

  if (count > 0)
  {
    count = rand() % count;
  }
    
  // convert the port to a string  
  snprintf(buffer2,sizeof(buffer2)-1,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[count])));
  serv_addr.sin_port = htons(PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) != 0)
  {    
    if (poll(&socket_file_descriptors,1,SOCKET_CONNECTION_TIMEOUT_SETTINGS) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings != 0)
      {  
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
    memcpy(str,"Error connecting to ",20);
    memcpy(str+20,HOST,HOST_LENGTH);
    memcpy(str+20+HOST_LENGTH," on port ",9);
    memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }

  // send the message 
  memset(message,0,sizeof(message));
  memcpy(message,DATA,strnlen(DATA,sizeof(message)));
  if (send_data(SOCKET,(unsigned char*)message,0,1,"") == 0)
  {  
    memcpy(str,"Error sending data to ",22);
    memcpy(str+22,HOST,HOST_LENGTH);
    memcpy(str+22+HOST_LENGTH," on port ",9);
    memcpy(str+31+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
    SEND_DATA_SOCKET_ERROR(str);
  }
    
  close(SOCKET);
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
  // Variables
  size_t count;
  time_t current_date_and_time;
  struct tm* current_UTC_date_and_time;
  long long int total;
  long long int sent;
  long long int bytes;

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
    char* message = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
    char current_date_and_time_data[BUFFER_SIZE];

    memset(current_date_and_time_data,0,sizeof(current_date_and_time_data));

    // get the current time
    get_current_UTC_time(current_date_and_time,current_UTC_date_and_time);

    strftime(current_date_and_time_data,sizeof(current_date_and_time_data),"%a, %d %b %Y %H:%M:%S GMT",current_UTC_date_and_time);

    memcpy(message,"HTTP/1.1 ",9);
    snprintf(message+9,MAXIMUM_BUFFER_SIZE-9,"%d",MESSAGE_SETTINGS);
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
    memcpy(message+strlen(message),"\r\nConnection: close\r\nContent-Length: ",37);
    snprintf(message+strlen(message),MAXIMUM_BUFFER_SIZE,"%ld",DATA_LENGTH);
    memcpy(message+strlen(message),"\r\nContent-Language: en\r\nContent-Type: ",38);
    memcpy(message+strlen(message),MESSAGE_DATA_SETTINGS,strnlen(MESSAGE_DATA_SETTINGS,MAXIMUM_BUFFER_SIZE));
    memcpy(message+strlen(message),"\r\nServer: XCASH_DPOPS version 1.0.0\r\nDate: ",52);
    memcpy(message+strlen(message),current_date_and_time_data,strnlen(current_date_and_time_data,MAXIMUM_BUFFER_SIZE));
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
       
  sent = 0;
  bytes = 0;
  do {
    bytes = write(SOCKET,data+sent,total-sent);
    if (bytes < 0)
    {             
      return 0;
    }
    else if (bytes == 0)  
    {
      break;
    }
    sent+=bytes;
    } while (sent < total);
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
  char* buffer = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(MAXIMUM_BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(buffer); \
  buffer = NULL; \
  free(data); \
  data = NULL;

  memset(message, 0, strlen(message)); 
  memset(data,0,strlen(data));
  for (;;)
  { 
    memset(buffer, 0, strlen(buffer));
    // check the size of the data that were about to receive. If the total data plus the data were about to receive is over 50 MB then dont accept it, since it will cause a buffer overflow
    if (((int)recvfrom(SOCKET, buffer, MAXIMUM_BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= MAXIMUM_BUFFER_SIZE - (int)strlen(data) && (int)strlen(data) > 0) || ((int)recvfrom(SOCKET, buffer, MAXIMUM_BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) >= MAXIMUM_BUFFER_SIZE && strlen(data) == 0))
    {
      pointer_reset_all;
      return 0;
    }    
    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data
    recvfrom(SOCKET, buffer, MAXIMUM_BUFFER_SIZE, MSG_DONTWAIT, NULL, NULL);  
    if (buffer[0] != '\0' && (strstr(buffer,STRING) == NULL && strstr(buffer,HTTP_SOCKET_END_STRING) == NULL))
    {
      // there is data, but this is not the final data
      memcpy(data+strlen(data),buffer,strnlen(buffer,MAXIMUM_BUFFER_SIZE));
    }
    if (buffer[0] != '\0' && (strstr(buffer,STRING) != NULL || strstr(buffer,HTTP_SOCKET_END_STRING) != NULL))
    {
      // there is data, and this is the final data
      memcpy(data+strlen(data),buffer,strnlen(buffer,MAXIMUM_BUFFER_SIZE));
      // if the final message has the SOCKET_END_STRING in the message, remove it
      if (strstr(data,SOCKET_END_STRING) != NULL)
      {
        // remove SOCKET_END_STRING from the message
        memcpy(message,data,strnlen(data,MAXIMUM_BUFFER_SIZE) - (sizeof(SOCKET_END_STRING)-1));
      }
      else
      {
        memcpy(message,data,strnlen(data,MAXIMUM_BUFFER_SIZE));
      }      
      break;
    }

    // check for a timeout in receiving data
    if (RECEIVE_DATA_SOCKET_TIMEOUT_SETTINGS == 1)
    {
      count++;
      if (count > (RECEIVE_DATA_SOCKET_TIMEOUT * 5))
      {
        pointer_reset_all;
        return 1;
      }
    }
    usleep(200000);   
  }
  pointer_reset_all;
  return 2;

  #undef pointer_reset_all
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
  size_t count;

  // Variables
  char str[BUFFER_SIZE];
  struct sockaddr_in serv_addr;
  struct pollfd socket_file_descriptors;
  int socket_settings;
  socklen_t socket_option_settings = sizeof(int);

  // define macros
  #define SOCKET_FILE_DESCRIPTORS_LENGTH 1

  memset(str,0,sizeof(str));

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
  memset(str,0,sizeof(str));
  memcpy(str,HOST,strnlen(HOST,sizeof(str)));
  string_replace(str,sizeof(str),"http://","");
  string_replace(str,sizeof(str),"https://","");
  string_replace(str,sizeof(str),"www.","");
  const struct hostent* HOST_NAME = gethostbyname(str); 
  if (HOST_NAME == NULL)
  {
    close(SOCKET);    
    return 0;
  } 

  // get a random IP address to connect to from the host if they have multiple IP addresses
  count = 0;
  while(HOST_NAME->h_addr_list[count] != NULL)
  {  
    count++;
  }

  if (count > 0)
  {
    count = rand() % count;
  } 
  
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(struct sockaddr_in));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[count])));
  serv_addr.sin_port = htons(SEND_DATA_PORT);

  /* set the first poll structure to our socket
  POLLOUT - set it to POLLOUT since the socket is non blocking and it can write data to the socket
  */
  socket_file_descriptors.fd = SOCKET;
  socket_file_descriptors.events = POLLOUT;

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,1000) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
    {   
      if (socket_settings == 0)
      {        
        close(SOCKET);
        return 1;
      } 
    }
  }

  // check for the XCASH daemon port
  serv_addr.sin_port = htons(XCASH_DAEMON_PORT);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) != 0)
  {    
    if (poll(&socket_file_descriptors,SOCKET_FILE_DESCRIPTORS_LENGTH,1000) == 1 && getsockopt(SOCKET,SOL_SOCKET,SO_ERROR,&socket_settings,&socket_option_settings) == 0)
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