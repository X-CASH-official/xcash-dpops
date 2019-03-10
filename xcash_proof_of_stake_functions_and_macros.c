#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "string.h"
#include <netdb.h> 
#include "netinet/in.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h> 

#include "xcash_proof_of_stake_functions_and_macros.h"
#include "xcash_proof_of_stake_test.h"


/*
-----------------------------------------------------------------------------------------------------------
Functions
-----------------------------------------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------------------------------------
Name: parse_json_data
Description: Parses json data
Parameters:
  data - The json data to parse
  field - The field you want to parse
  result - Where the result is stored
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(const char* DATA, const char* FIELD_NAME, char *result)
{
  // Constants
  const size_t STRING_LENGTH = strnlen(FIELD_NAME,BUFFER_SIZE);  

  // Variables
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* str1;
  char* str2;
  int settings = 1;
  size_t start; 

  // define macros
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(data2); \
  data2 = NULL; 

  // check if the field is in the data
  if (strstr(DATA,FIELD_NAME) != NULL)
  { 
    // modify the field to add the field syntax
    memcpy(str,"\"",1);
    memcpy(str+1,FIELD_NAME,STRING_LENGTH);
    memcpy(str+1+STRING_LENGTH,"\": ",3);
    // get the start of the field's data
    start = strnlen(str,BUFFER_SIZE);
    // get the pointers location to the start of the field
    str1 = strstr(DATA,str);
    if (str1 == NULL)
    {
       // an error has occured, get the error message
       settings = 0;
       str1 = strstr(DATA,"\"message\": ");
       start = 11;
       if (str1 == NULL)
       {
         // their is no error message so return an error
         memcpy(result,"An error has occured",20);
         pointer_reset_all;
         return settings;
       }
    }
    // get the end location of the data
    str2 = strstr(str1,"\r\n");
    // get the length of the field's data
    const int LENGTH = str2 - str1 - start;
    if (LENGTH <= 0)
    {
      memcpy(result,"An error has occured",20);
      pointer_reset_all;
      return settings;
    }
    // copy the field's data
    memcpy(result,&str1[start],LENGTH);
 
    // remove all the formating from the result
    string_replace(result, "\"", "");
    string_replace(result, ",", "");
    string_replace(result, "[", "");
    string_replace(result, "]", "");
    string_replace(result, "{", "");
    string_replace(result, "}", "");
  }

  pointer_reset_all;
  return settings;

  #undef pointer_reset_all
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
  TITLE - A summary of the data sent in the POST request. This text gets printed to the console
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_http_request(char *result, const char* HOST, const char* URL, const int PORT, const char* HTTP_SETTINGS, const char* HTTP_HEADERS[], const size_t HTTP_HEADERS_LENGTH, const char* DATA, const int DATA_TIMEOUT_SETTINGS, const char* TITLE, const int MESSAGE_SETTINGS)
{
  // Constants
  const struct timeval SOCKET_TIMEOUT = {SOCKET_TIMEOUT_SETTINGS, 0};  

  // Variables
  char response[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char* post_request_data;
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;  
  size_t receive_data_result; 

  // define macros
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(message); \
  message = NULL;

  // create the HTTP request
  append_string(message,HTTP_SETTINGS);
  append_string(message," ");
  append_string(message,URL);
  if (strncmp(HTTP_SETTINGS,"GET",BUFFER_SIZE) == 0)
  {
    append_string(message,"?");
    append_string(message,DATA);
  }
  append_string(message," HTTP/1.1\r\nHost: ");
  append_string(message,HOST);
  append_string(message,"\r\n");
  for (count = 0; count < HTTP_HEADERS_LENGTH; count++)
  {
    append_string(message,HTTP_HEADERS[count]);
    append_string(message,"\r\n");
  }
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    append_string(message,"Content-Length: ");
    sprintf(str, "%d", (int)strnlen(DATA,BUFFER_SIZE));
    append_string(message,str);
  } 
  append_string(message,"\r\n\r\n");    
  if (strncmp(HTTP_SETTINGS,"POST",BUFFER_SIZE) == 0)
  {
    append_string(message,DATA);
  }  
  memset(&response, 0, sizeof(response));

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error creating socket for sending a post request","red");
    }
    pointer_reset_all;
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_RCVTIMEO = sets a receiving timeout for the socket
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error setting socket timeout for sending a post request","red"); 
    }
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
    pointer_reset_all;
    return 0;
  }

  // convert the port to a string  
  sprintf(buffer2,"%d",PORT);  

  // get the length of buffer2 and host, since they will not change at this point and we need them for faster string copying
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  const size_t HOST_LENGTH = strnlen(HOST,BUFFER_SIZE);
  
  struct sockaddr_in serv_addr;
  memset(&serv_addr,0,sizeof(serv_addr));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
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
     
  // close the socket
  close(SOCKET);

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
    
  pointer_reset_all;
  #undef pointer_reset_all
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_and_receive_data_socket
Description: Send a message through a socket
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
  const struct timeval SOCKET_TIMEOUT = {SOCKET_TIMEOUT_SETTINGS, 0};   

  // Variables 
  char buffer2[BUFFER_SIZE];
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int receive_data_result;

   // define macros
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(message); \
  message = NULL;

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
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

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket to receive a timeout
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error setting socket timeout for sending data to ",49);
      memcpy(str+49,HOST,HOST_LENGTH);
      color_print(str,"red");
    }
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
    pointer_reset_all;
    return 0;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  struct sockaddr_in serv_addr;
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      color_print(str,"red"); 
    }
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
  
  // close the socket
  close(SOCKET);

  pointer_reset_all;
  #undef pointer_reset_all
  return 1;
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
  const struct timeval SOCKET_TIMEOUT = {SOCKET_TIMEOUT_SETTINGS, 0};   
  
  // Variables 
  char buffer2[BUFFER_SIZE];
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));

   // define macros
  #define pointer_reset_all \
  free(str); \
  str = NULL; \
  free(message); \
  message = NULL;

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
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

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket to receive a timeout
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error setting socket timeout for sending data to ",49);
      memcpy(str+49,HOST,HOST_LENGTH);
      color_print(str,"red");
    }
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
    pointer_reset_all;
    return 0;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",PORT); 
   
  const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
  struct sockaddr_in serv_addr;
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(PORT);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,HOST,HOST_LENGTH);
      memcpy(str+20+HOST_LENGTH," on port ",9);
      memcpy(str+29+HOST_LENGTH,buffer2,BUFFER2_LENGTH);
      color_print(str,"red"); 
    }
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
    pointer_reset_all;   
    return 0;
  }
    
  // close the socket
  close(SOCKET);

  pointer_reset_all;
  #undef pointer_reset_all
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_count
Description: Counts the occurences of a substring in a string
Parameters:
  data - The string to count the occurence in
  string - The substring to count the occurences of
Return: The number of occurences of the substring in the string
-----------------------------------------------------------------------------------------------------------
*/

size_t string_count(const char* DATA, const char* STRING)
{
  // Constants
  const size_t STRING_LENGTH = strnlen(STRING,BUFFER_SIZE);
  
  // Variables
  char* datacopy = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  size_t count = 0;

  // get the occurences of the string 
  memcpy(datacopy,DATA,strnlen(DATA,BUFFER_SIZE));
  while((datacopy = strstr(datacopy, STRING)) != NULL)
  {
    count++;
    datacopy+= STRING_LENGTH;
  } 

  pointer_reset(datacopy);
  return count;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: string_replace
Description: String replace
Parameters:
  data - The string to replace the data
  STR1 - The string to be replaced
  STR2 - The string to replace the other string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int string_replace(char *data, const char* STR1, const char* STR2)
{  
  // check if the str to replace exist in the string
  if (strstr(data,STR1) != NULL)
  { 
    // Variables
    char* datacopy = (char*)calloc(BUFFER_SIZE,sizeof(char));
    char* string;
    size_t data_length;
    size_t str2_length;
    size_t start;
    size_t total = 0;
    size_t count = 0;  

    // define macros
    #define REPLACE_STRING "|REPLACE_STRING|"  

    // get the occurences of STR1   
    total = string_count(data,(char*)STR1);

    // replace the string with the REPLACE_STRING
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(REPLACE_STRING,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,STR1),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,STR1)+strnlen(STR1,BUFFER_SIZE);
           
      // copy the bytes before STR1 to the new string
      memcpy(datacopy,data,start);
      // copy STR2 to the new string
      memcpy(datacopy+start,REPLACE_STRING,str2_length);
      // copy the bytes after STR1 to the new string
      memcpy(datacopy+start+str2_length,string,strnlen(string,BUFFER_SIZE));
      // copy the new string to the string pointer
      memset(data,0,data_length);
      memcpy(data,datacopy,strnlen(datacopy,BUFFER_SIZE));
    }
    // replace the REPLACE_STRING with STR2
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(STR2,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,REPLACE_STRING),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,REPLACE_STRING)+strnlen(REPLACE_STRING,BUFFER_SIZE);
           
      // copy the bytes before REPLACE_STRING to the new string
      memcpy(datacopy,data,start);
      // copy STR2 to the new string
      memcpy(datacopy+start,STR2,str2_length);
      // copy the bytes after REPLACE_STRING to the new string
      memcpy(datacopy+start+str2_length,string,strnlen(string,BUFFER_SIZE));
      // copy the new string to the string pointer
      memset(data,0,data_length);
      memcpy(data,datacopy,strnlen(datacopy,BUFFER_SIZE));
    }
    pointer_reset(datacopy);
    return 1;
  }
  else
  {
    return 0;
  } 

  #undef REPLACE_STRING
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
Name: random_string
Description: Creates a random string of specified length
Parameters:
  result - The string where you want the random string to be saved to
  LENGTH - The length of the random string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int random_string(char *result, const size_t LENGTH)
{  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;

  // define macros
  #define STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" 
  #define MINIMUM 0
  #define MAXIMUM 61
  
  memcpy(data,STRING,62);
  for (count = 0; count < LENGTH; count++)
  {
    memcpy(result+count,&data[((rand() % (MAXIMUM - MINIMUM + 1)) + MINIMUM)],1);
  }
  pointer_reset(data); 
  return 1;
  
  #undef STRING
  #undef MINIMUM
  #undef MAXIMUM  
}


/*
-----------------------------------------------------------------------------------------------------------
Name: get_public_address
Description: Gets the public address of your wallet
Parameters:
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define GET_PUBLIC_ADDRESS_DATA "{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}"

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,GET_PUBLIC_ADDRESS_DATA,RECEIVE_DATA_TIMEOUT_SETTINGS,"get public address",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"address",xcash_wallet_public_address) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
  
  // check if the returned data is valid
  if (strnlen(xcash_wallet_public_address,BUFFER_SIZE) != XCASH_WALLET_LENGTH && strncmp(xcash_wallet_public_address,XCASH_WALLET_PREFIX,3) != 0)
  {
     pointer_reset(data); 
     return 0;
  }
  
  pointer_reset(data); 
  return 1;

  #undef GET_PUBLIC_ADDRESS_DATA
}


/*
-----------------------------------------------------------------------------------------------------------
Name: get_block_template
Description: Gets the block template for creating a new block
Parameters:
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_block_template(char *result, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(message); \
  message = NULL; \
  free(data); \
  data = NULL; \

  // create the message
  memcpy(message,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_template\",\"params\":{\"wallet_address\":\"",84);
  memcpy(message+84,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+84+XCASH_WALLET_LENGTH,"\",\"reserve_size\":255}",21);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,message,RECEIVE_DATA_TIMEOUT_SETTINGS,"get block template",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset_all;
    return 0;
  }
  
  if (parse_json_data(data,"blocktemplate_blob",result) == 0)
  {
    pointer_reset_all;
    return 0;
  }
  
  pointer_reset_all; 
  return 1;
  
  #undef pointer_reset_all
}


/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_block_height
Description: Gets the current block height of the network
Parameters:
  result - The string where you want the current block height to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_current_block_height(char *result, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_block_count\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get current block height",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"count",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
    
  pointer_reset(data); 
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: get_previous_block_hash
Description: Gets the previous block hash of the network
Parameters:
  result - The string where you want the previous block hash to be saved to
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_previous_block_hash(char *result, const int MESSAGE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_DAEMON_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_last_block_header\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get previous block hash",MESSAGE_SETTINGS) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,"hash",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
      
  pointer_reset(data); 
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: sign_data
Description: Signs data with your XCA address, for sending data securely
Parameters:
  message - The sign_data
  MESSAGE_SETTINGS - 1 to print the messages, otherwise 0. This is used for the testing flag to not print any success or error messages
  CURRENT_ROUND_PART - The current round part
  CURRENT_ROUND_PART_BACKUP_NODE - The current backup node for the current round part
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sign_data(char *message, const int HTTP_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* previous_block_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* random_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(previous_block_hash); \
  previous_block_hash = NULL; \
  free(random_data); \
  random_data = NULL; \
  free(result); \
  result = NULL; \
  free(data); \
  data = NULL; \
  free(string); \
  string = NULL;



  // create the random data
  if (random_string(random_data,RANDOM_STRING_LENGTH) == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  // get the previous block hash
  if (get_previous_block_hash(previous_block_hash,0) == 0)
  {  
    pointer_reset_all;
    return 0;
  }

  // create the message
  size_t message_length = strnlen(message,BUFFER_SIZE)-1;
  const size_t previous_block_hash_LENGTH = strnlen(previous_block_hash,BUFFER_SIZE);
  memcpy(result,message,message_length);
  memcpy(result+message_length," \"public_address\": \"",20);
  memcpy(result+message_length+20,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(result+message_length+20+XCASH_WALLET_LENGTH,"\",\r\n \"previous_block_hash\": \"",29);
  memcpy(result+message_length+49+XCASH_WALLET_LENGTH,previous_block_hash,previous_block_hash_LENGTH);  
  memcpy(result+message_length+49+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"current_round_part\": \"",28);
  memcpy(result+message_length+77+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,current_round_part,1);
  memcpy(result+message_length+78+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"current_round_part_backup_node\": \"",40);
  memcpy(result+message_length+118+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,current_round_part_backup_node,1);
  memcpy(result+message_length+119+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"data\": \"",14);
  memcpy(result+message_length+133+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,random_data,RANDOM_STRING_LENGTH);
  memcpy(result+message_length+133+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH+RANDOM_STRING_LENGTH,"\",\r\n}",5);

  // format the message
  if (string_replace(result,"\"","\\\"") == 0)
  {
    pointer_reset_all; 
    return 0;
  }

  // sign_data
  const size_t RESULT_LENGTH = strnlen(result,BUFFER_SIZE);
  memcpy(string,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(string+60,result,RESULT_LENGTH);
  memcpy(string+60+RESULT_LENGTH,"\"}}",3);
  memset(result,0,RESULT_LENGTH);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",HTTP_SETTINGS) <= 0)
  {  
    pointer_reset_all;
    return 0;
  } 

  if (parse_json_data(data,"signature",result) == 0)
  {
    pointer_reset_all;
    return 0;
  }

  // check if the returned data is valid
  if (strnlen(result,BUFFER_SIZE) != XCASH_SIGN_DATA_LENGTH && strncmp(result,XCASH_SIGN_DATA_PREFIX,5) != 0)
  {
    pointer_reset_all;
    return 0;
  }
  
  // create the message
  message_length = strnlen(message,BUFFER_SIZE) - 1;
  const size_t XCASH_PROOF_OF_STAKE_SIGNATURE_LENGTH = strnlen(result,BUFFER_SIZE);
  
  memcpy(message+message_length," \"public_address\": \"",20);
  memcpy(message+message_length+20,xcash_wallet_public_address,XCASH_WALLET_LENGTH);
  memcpy(message+message_length+20+XCASH_WALLET_LENGTH,"\",\r\n \"previous_block_hash\": \"",29);
  memcpy(message+message_length+49+XCASH_WALLET_LENGTH,previous_block_hash,previous_block_hash_LENGTH);  
  memcpy(message+message_length+49+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"current_round_part\": \"",28);
  memcpy(message+message_length+77+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,current_round_part,1);
  memcpy(message+message_length+78+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"current_round_part_backup_node\": \"",40);
  memcpy(message+message_length+118+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,current_round_part_backup_node,1);
  memcpy(message+message_length+119+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,"\",\r\n \"data\": \"",14);
  memcpy(message+message_length+133+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH,random_data,RANDOM_STRING_LENGTH);
  memcpy(message+message_length+133+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH+RANDOM_STRING_LENGTH,"\",\r\n \"xcash_proof_of_stake_signature\": \"",40);
  memcpy(message+message_length+173+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH+RANDOM_STRING_LENGTH,result,XCASH_PROOF_OF_STAKE_SIGNATURE_LENGTH);
  memcpy(message+message_length+173+XCASH_WALLET_LENGTH+previous_block_hash_LENGTH+RANDOM_STRING_LENGTH+XCASH_PROOF_OF_STAKE_SIGNATURE_LENGTH,"\",\r\n}",5);

  pointer_reset_all;
  #undef pointer_reset_all
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: verify_data
Description: Verifies signed data, for receiving data securely
Parameters:
  message - The signed data
  HTTP_SETTINGS - The http settings
  VERIFY_CURRENT_ROUND_PART_SETTINGS - 1 to verify the current_round_part, otherwise 0
  CURRENT_ROUND_PART - The current round part
  VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS - 1 to verify the current_round_part_backup_node, otherwise 0
  CURRENT_ROUND_PART_BACKUP_NODE - The current backup node for the current round part
Return: 0 if the signed data is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_data(const char* MESSAGE, const int HTTP_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_SETTINGS, const int VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
  
  // Variables
  char* message_settings = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* previous_block_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_previous_block_hash = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_current_round_part = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message_current_round_part_backup_node = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* xcash_proof_of_stake_signature = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t message_length = strnlen(MESSAGE,BUFFER_SIZE) - 134;

  // define macros
  #define pointer_reset_all \
  free(message_settings); \
  message_settings = NULL; \
  free(public_address); \
  public_address = NULL; \
  free(previous_block_hash); \
  previous_block_hash = NULL; \
  free(message_previous_block_hash); \
  message_previous_block_hash = NULL; \
  free(message_current_round_part); \
  message_current_round_part = NULL; \
  free(message_current_round_part_backup_node); \
  message_current_round_part_backup_node = NULL; \
  free(xcash_proof_of_stake_signature); \
  xcash_proof_of_stake_signature = NULL; \
  free(result); \
  result = NULL; \
  free(data); \
  data = NULL; \
  free(string); \
  string = NULL;

  // parse the message
  if (parse_json_data(MESSAGE,"message_settings",message_settings) == 0 || parse_json_data(MESSAGE,"public_address",public_address) == 0 || parse_json_data(MESSAGE,"previous_block_hash",message_previous_block_hash) == 0 || parse_json_data(MESSAGE,"current_round_part",message_current_round_part) == 0 || parse_json_data(MESSAGE,"current_round_part_backup_node",message_current_round_part_backup_node) == 0 || parse_json_data(MESSAGE,"xcash_proof_of_stake_signature",xcash_proof_of_stake_signature) == 0)
  {
    pointer_reset_all;
    return 0;
  }

  // verify if the message came from the correct node
  if (strncmp(message_settings,"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_MAIN_NODE_START_PART_OF_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_LIST_OF_ENABLED_NODES",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_ROUND_CHANGE",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_CONSENSUS_NODE_CHANGE",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_XCASH_PROOF_OF_STAKE_MESSAGE",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEW_PART_OF_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_NEXT_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"CONSENSUS_NODE_TO_NODES_AND_MAIN_NODES_RECALCULATING_VOTES",BUFFER_SIZE) == 0)
  {
    if (strncmp(public_address,CONSENSUS_NODE_PUBLIC_ADDRESS,BUFFER_SIZE) != 0)
    {
      pointer_reset_all;
      return 0;
    }
  }
  if (strncmp(message_settings,"MAIN_NODES_TO_NODES_PART_1_OF_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"MAIN_NODES_TO_NODES_PART_2_OF_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"MAIN_NODES_TO_NODES_PART_3_OF_ROUND",BUFFER_SIZE) == 0 || strncmp(message_settings,"MAIN_NODES_TO_NODES_PART_4_OF_ROUND",BUFFER_SIZE) == 0)
  {
    if (strncmp(public_address,main_nodes_public_address,BUFFER_SIZE) != 0)
    {
      pointer_reset_all;
      return 0;
    }
  }    
  if (strncmp(message_settings,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",BUFFER_SIZE) == 0 || strncmp(message_settings,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",BUFFER_SIZE) == 0 || strncmp(message_settings,"NODES_TO_CONSENSUS_NODE_MAIN_NODE_SOCKET_TIMEOUT_ROUND_CHANGE",BUFFER_SIZE) == 0 || strncmp(message_settings,"NODES_TO_NODES_VOTE_RESULTS",BUFFER_SIZE) == 0 || strncmp(message_settings,"NODES_TO_CONSENSUS_NODE_VOTE_RESULTS",BUFFER_SIZE) == 0)
  {
    memcpy(data,public_address,XCASH_WALLET_LENGTH);
    memcpy(data+XCASH_WALLET_LENGTH,"|",1);
    if (strstr(nodes_public_address_list,data) != NULL)
    {
      pointer_reset_all;
      return 0;
    }
    memset(data,0,strnlen(data,BUFFER_SIZE));
  }




  // verify if the previous block hash is correct
  if (get_previous_block_hash(previous_block_hash,0) == 0)
  {  
    pointer_reset_all;
    return 0;
  }
  if (strncmp(previous_block_hash,message_previous_block_hash,BUFFER_SIZE) != 0)
  {
    pointer_reset_all;
    return 0;
  }

  // verify if the current_round_part is correct
  if (VERIFY_CURRENT_ROUND_PART_SETTINGS == 1)
  {
    if (strncmp(current_round_part,message_current_round_part,BUFFER_SIZE) != 0)
    {
      pointer_reset_all;
      return 0;
    }
  }

  // verify if the current_round_part_backup_node
  if (VERIFY_CURRENT_ROUND_PART_BACKUP_NODE_SETTINGS == 1)
  {
    if (strncmp(current_round_part_backup_node,message_current_round_part_backup_node,BUFFER_SIZE) != 0)
    {
      pointer_reset_all;
      return 0;
    }
  }
  
  // create the message
  memcpy(result,MESSAGE,message_length);
  memcpy(result+message_length,"}",1);
  if (string_replace(result,"\"","\\\"") == 0)
  {
    pointer_reset_all;
    return 0;
  } 
    
  // create the message
  message_length = strnlen(result,BUFFER_SIZE);
  memcpy(string,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"verify\",\"params\":{\"data\":\"",62);
  memcpy(string+62,result,message_length);
  memcpy(string+62+message_length,"\",\"address\":\"",13);
  memcpy(string+75+message_length,public_address,XCASH_WALLET_LENGTH);
  memcpy(string+75+message_length+XCASH_WALLET_LENGTH,"\",\"signature\":\"",15);
  memcpy(string+90+message_length+XCASH_WALLET_LENGTH,xcash_proof_of_stake_signature,XCASH_SIGN_DATA_LENGTH);
  memcpy(string+90+message_length+XCASH_WALLET_LENGTH+XCASH_SIGN_DATA_LENGTH,"\"}}",3);
  memset(result,0,strnlen(result,BUFFER_SIZE));

  if (send_http_request(result,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"verify data",HTTP_SETTINGS) <= 0)
  { 
    pointer_reset_all; 
    return 0;
  } 
    
  if (parse_json_data(result,"good",data) == 0)
  {
    pointer_reset_all;
    return 0;
  }

  // check if the returned data is valid
  if (strncmp(data,"true",BUFFER_SIZE) != 0)
  {
     pointer_reset_all; 
     return 0;
  }
 
  pointer_reset_all;
  #undef pointer_reset_all
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: read_file
Description: Reads the file
Parameters:
  result - The data read from the file
  file_name - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int read_file(char *result, const char* FILE_NAME)
{
  // Variables
  FILE* file;
  int settings = 0;

  // check if the file exist
  file = fopen(FILE_NAME,"r");
  if (file != NULL)
  {
    // the file exist, read the data in the result
    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0, SEEK_SET); 
    fread(result, file_size, sizeof(char), file);
    fclose(file);
    settings = 1;
  }
  return settings;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: write_file
Description: Writes data to a file
Parameters:
  DATA = The data to write to the file
  file_name - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int write_file(const char* DATA, const char* FILE_NAME)
{
  // Variables
  FILE* file;
  int settings = 0;

  file = fopen(FILE_NAME,"w");
  if (file != NULL)
  {
    fprintf(file,"%s",DATA);
    fclose(file);
    settings = 1;
  }
  return settings;  
}



/*
-----------------------------------------------------------------------------------------------------------
Server functions
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: get_current_consensus_nodes_IP_address
Description: Gets the current consensus nodes IP address, so the node knows which consensus node to send and receive data from
Parameters:
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_current_consensus_nodes_IP_address()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL; \
  free(message2); \
  message2 = NULL;

  // create the message
  memcpy(message,"{\r\n \"message_settings\": \"CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS\",\r\n}",102);

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    color_print("Could not sign_data","red");
    pointer_reset_all;
    return 0;
  }
 
  // send the message to the consensus node and consensus backup node
  const int SEND_AND_RECEIVE_DATA_SOCKET_DATA1 = send_and_receive_data_socket(data,CONSENSUS_NODES_IP_ADDRESS,SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",0);
  const int SEND_AND_RECEIVE_DATA_SOCKET_DATA2 = send_and_receive_data_socket(data2,CONSENSUS_BACKUP_NODES_IP_ADDRESS,SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"NODE_TO_CONSENSUS_NODE_SEND_CURRENT_CONSENSUS_NODE_IP_ADDRESS",0);

  // verify the data
  if (verify_data(data,0,1,1) == 0 || verify_data(data2,0,1,1) == 0)
  {   
    color_print("Could not verify data from the consensus node","red");
    pointer_reset_all;
    return 0; 
  }

  // parse the data
  memset(message,0,strnlen(message,BUFFER_SIZE));
  if (parse_json_data(data,"current_consensus_node_IP_address",message) == 0 || parse_json_data(data2,"current_consensus_node_IP_address",message2) == 0)
  {
    color_print("Could not parse current_consensus_node_IP_address from the CONSENSUS_NODE_TO_NODE_RECEIVE_CURRENT_CONSENSUS_NODE_IP_ADDRESS message","red");
    pointer_reset_all;
    return 0; 
  }
  
  // check what consensus node IP address is the current consensus node IP address
  if (SEND_AND_RECEIVE_DATA_SOCKET_DATA1 == 1 && SEND_AND_RECEIVE_DATA_SOCKET_DATA2 == 1 && strncmp(message,message2,BUFFER_SIZE) != 0)
  {
    // The consensus node and the consensus backup node returned different results
    color_print("The consensus node and the consensus backup node returned different results\n","red");
    pointer_reset_all;
    return 0;
  }
  else if (SEND_AND_RECEIVE_DATA_SOCKET_DATA1 == 1 && SEND_AND_RECEIVE_DATA_SOCKET_DATA2 == 1 && strncmp(message,message2,BUFFER_SIZE) == 0)
  {
    memcpy(current_consensus_nodes_IP_address,message,strnlen(message,BUFFER_SIZE));    
  }
  else if (SEND_AND_RECEIVE_DATA_SOCKET_DATA1 == 1 && SEND_AND_RECEIVE_DATA_SOCKET_DATA2 == 0)
  {
    memcpy(current_consensus_nodes_IP_address,message,strnlen(message,BUFFER_SIZE));  
  }
  else if (SEND_AND_RECEIVE_DATA_SOCKET_DATA1 == 0 && SEND_AND_RECEIVE_DATA_SOCKET_DATA2 == 1)
  {
    memcpy(current_consensus_nodes_IP_address,message2,strnlen(message2,BUFFER_SIZE));  
  }
  else
  {
    color_print("An error has occured\n","red");
    pointer_reset_all;
    return 0; 
  }
  
  #undef pointer_reset_all
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: get_updated_node_list
Description: Gets the updated node list, so it will know what nodes to accept data from
Parameters:
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_updated_node_list()
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* message2 = (char*)calloc(BUFFER_SIZE,sizeof(char));

   // define macros
  #define pointer_reset_all \
  free(data); \
  data = NULL; \
  free(message); \
  message = NULL; \
  free(message2); \
  message2 = NULL;

  // iniltize the variables
  memset(data,0,strnlen(data,BUFFER_SIZE));
  memcpy(data,"0",1);

  // read the nodes_updated_time.txt
  read_file(data,NODES_UPDATED_TIME_FILE_NAME);

  // create the message
  const size_t DATA_LENGTH = strnlen(data,BUFFER_SIZE);
  memcpy(message,"{\r\n \"message_settings\": \"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST\",\r\n \"nodes_updated_time\": \"",98);
  memcpy(message+98,data,DATA_LENGTH);
  memcpy(message+98+DATA_LENGTH,"\",\r\n}",5);
  memset(data,0,strnlen(data,BUFFER_SIZE));

  // sign_data
  if (sign_data(message,0) == 0)
  { 
    color_print("Could not sign_data","red");
    pointer_reset_all;
    return 0;
  }
 
  // send the message to the consensus node and consensus backup node
  if (send_and_receive_data_socket(data,current_consensus_nodes_IP_address,SEND_DATA_PORT,message,TOTAL_CONNECTION_TIME_SETTINGS,"NODE_TO_CONSENSUS_NODE_SEND_UPDATED_NODE_LIST",0) == 0)
  {
    color_print("Could not send data to the consensus node","red");
    pointer_reset_all;
    return 0; 
  }
  
  // verify the data
  if (verify_data(data,0,1,1) == 0)
  {   
    color_print("Could not verify data from the consensus node","red");
    pointer_reset_all;
    return 0; 
  }

  // parse the data
  memset(message,0,strnlen(message,BUFFER_SIZE));
  if (parse_json_data(data,"nodes_name_list",message) == 0 || parse_json_data(data,"nodes_public_address_list",message2) == 0)
  {
    color_print("Could not parse data from the CONSENSUS_NODE_TO_NODE_RECEIVE_UPDATED_NODE_LIST message","red");
    pointer_reset_all;
    return 0; 
  }

  // check if we need to update the node list
  memset(nodes_public_address_list,0,strnlen(nodes_public_address_list,BUFFER_SIZE));
  memset(nodes_name_list,0,strnlen(nodes_name_list,BUFFER_SIZE));
  if (strncmp(message,"UPDATED_NODE_LIST",BUFFER_SIZE) != 0 && strncmp(message2,"UPDATED_NODE_LIST",BUFFER_SIZE) != 0)
  {
    // update the node list and load the nodes list into the global variables  
    write_file(message,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME); 
    write_file(message2,NODES_NAME_LIST_FILE_NAME);     
    memcpy(nodes_public_address_list,message,strnlen(message,BUFFER_SIZE));
    memcpy(nodes_name_list,message2,strnlen(message2,BUFFER_SIZE));
    color_print("The node list has been updated successfully\nLoaded the data, and saved the node list to files\n","green");
  }
  else
  {
    // the node list has already been updated
    read_file(nodes_public_address_list,NODES_PUBLIC_ADDRESS_LIST_FILE_NAME);
    read_file(nodes_name_list,NODES_NAME_LIST_FILE_NAME);
    color_print("The node list is already up to date\nLoaded the current node list from the files.\n","green");
  }

  #undef pointer_reset_all
  return 1;
}



/*
-----------------------------------------------------------------------------------------------------------
Name: server_received_data_xcash_proof_of_stake_test_data
Description: Runs the code when the server receives the xcash_proof_of_stake_test_data message
Parameters:
  CLIENT_SOCKET - The socket to send data to
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_received_data_xcash_proof_of_stake_test_data(const int CLIENT_SOCKET, char* MESSAGE)
{
  // verify the message
  if (verify_data(MESSAGE,0,1,1) == 0)
  {   
    return 0;
  }
  else
  {
    if (send_data(CLIENT_SOCKET,MESSAGE,1) == 1)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }  
}


/*
-----------------------------------------------------------------------------------------------------------
Name: server_receive_data_socket_consensus_node_to_node
Description: Runs the code when the server receives the CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_receive_data_socket_consensus_node_to_node(const int CLIENT_SOCKET, pthread_t thread_id, char* message)
{
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // verify the data
  if (verify_data(message,1,0,0) == 0)
  {
    // close the forked process
    color_print("Message could not be verified from consensus node for CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS","red");
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }

  // parse the message
  if (parse_json_data(message,"main_nodes_public_address",data) == 0)
  {
    // close the forked process
    color_print("Could not parse main_nodes_public_address from the CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS message","red");
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }
  memcpy(main_nodes_public_address,data,strnlen(data,BUFFER_SIZE));

  if (parse_json_data(message,"current_round_part",data) == 0)
  {
    // close the forked process
    color_print("Could not parse current_round_part from the CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS message","red");
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }
  memcpy(current_round_part,data,strnlen(data,BUFFER_SIZE));

  if (parse_json_data(message,"current_round_part_backup_node",data) == 0)
  {
    // close the forked process
    color_print("Could not parse current_round_part_backup_node from the CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS message","red");
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }
  memcpy(current_round_part_backup_node,data,strnlen(data,BUFFER_SIZE));



  // create a timeout from the time the consensus node lets us know who the main node is for this part of the round, to the time the main node sends us data.
  if (strncmp(current_round_part,"1",BUFFER_SIZE) == 0 || strncmp(current_round_part,"3",BUFFER_SIZE) == 0)
  {
    memcpy(data,"VRF_PUBLIC_AND_PRIVATE_KEY",26);
  }
  else if (strncmp(current_round_part,"2",BUFFER_SIZE) == 0)
  {
    memcpy(data,"VRF_RANDOM_DATA",15);
  }
  else if (strncmp(current_round_part,"4",BUFFER_SIZE) == 0)
  {
    memcpy(data,"BLOCK_PRODUCER",14);
  }
  // create a struct for the parameters
  struct mainnode_timeout_thread_parameters parameters = {
    (const pid_t)getpid(),
    0,
    (const char*)data,
    (const char*)current_round_part,
    (const char*)current_round_part_backup_node,

  };
  // create a timeout for this connection, since we need to limit the amount of time a client has to send data from once it connected
  if (pthread_create(&thread_id, NULL, &mainnode_timeout_thread, (void *)&parameters) != 0)
  {
    // close the forked process
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }
  // set the thread to dettach once completed, since we do not need to use anything it will return.
  if (pthread_detach(thread_id) != 0)
  {
    // close the forked process
    close(CLIENT_SOCKET);
    pointer_reset(data);
    _exit(0);
  }
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Thread functions
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: read_file_thread
Description: Reads a file on a separate thread
Parameters:
  parameters - A pointer to the read_file_thread_parameters struct
  struct read_file_thread_parameters
    result - The data read from the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* read_file_thread(void* parameters)
{
  struct read_file_thread_parameters* data = parameters;
  int settings = read_file(data->result, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: write_file_thread
Description: Writes a file on a separate thread
Parameters:
  parameters - A pointer to the write_file_thread_parameters struct
  struct write_file_thread_parameters
    DATA - The data to write to the file
    FILE_NAME - The file name
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

void* write_file_thread(void* parameters)
{
  struct write_file_thread_parameters* data = parameters;
  int settings = write_file(data->DATA, data->FILE_NAME);
  pthread_exit((void *)(intptr_t)settings);
}



/*
-----------------------------------------------------------------------------------------------------------
Name: thread_settings
Description: Waits for a thread to return a value, and returns the value from the thread
Parameters:
   thread_id - The thread id of the thread
Return: The value that the thread returned
-----------------------------------------------------------------------------------------------------------
*/

int thread_settings(pthread_t thread_id)
{
  void* thread_settings;
  pthread_join(thread_id, &thread_settings);
  int settings = (intptr_t)thread_settings;
  return settings;
}


/*
-----------------------------------------------------------------------------------------------------------
Server Thread functions
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: total_connection_time_thread
Description: Closes the forked process after a certain connection timeout
Parameters:
  parameters - A pointer to the total_connection_time_thread_parameters struct
  struct total_connection_time_thread_parameters
    process_id - The process id of the forked process
    client_address - The client's IP address for the forked process
    port - The client's connected port for the forked process
    data_received - 1 if data was received in the timeout time, otherwise 0
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* total_connection_time_thread(void* parameters)
{ 
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct total_connection_time_thread_parameters* data = parameters;
  int client_address_length = strnlen(data->client_address,BUFFER_SIZE);
  int data_port_length = strnlen(data->port,BUFFER_SIZE);
  sleep(TOTAL_CONNECTION_TIME_SETTINGS);  
  printf("Total connection time has been reached for %s on port %s\n", data->client_address,data->port); 
  if (data->data_received == 1)
  {
    memcpy(string,"Client at ",10);
    memcpy(string+10,data->client_address,client_address_length);
    memcpy(string+10+client_address_length," on port ",9);
    memcpy(string+19+client_address_length,data->port,data_port_length);
    memcpy(string+19+client_address_length+data_port_length," has sent data in the timeout time",34);
    color_print(string,"green");
  }
  else
  {
    memcpy(string,"Client at ",10);
    memcpy(string+10,data->client_address,client_address_length);
    memcpy(string+10+client_address_length," on port ",9);
    memcpy(string+19+client_address_length,data->port,data_port_length);
    memcpy(string+19+client_address_length+data_port_length," did not send any data in the timeout time",42);
    color_print(string,"red"); 
  }
  pointer_reset(string);
  kill((intptr_t)data->process_id, SIGKILL);  
  return NULL;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: mainnode_timeout_thread
Description: Closes the forked process after a certain connection timeout. This is so the node knows if it should send a message to the consensus node that the main node never sent any data, and if a round change should occur
Parameters:
  parameters - A pointer to the mainnode_timeout_thread_parameters struct
  struct mainnode_timeout_thread_parameters
    process_id - The process id of the forked process
    data_received - 1 if the node has received data from the main node, otherwise 0
    main_nodes_public_address - The public address of the main node for that part of the round
    current_round_part - The current round part (1-4).
    current_round_part_backup_node - The current main node in the current round part (0-5)
Return: NULL
-----------------------------------------------------------------------------------------------------------
*/

void* mainnode_timeout_thread(void* parameters)
{
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  struct mainnode_timeout_thread_parameters* data = parameters;
  const size_t main_node_length = strnlen(data->main_node,BUFFER_SIZE);
  sleep(TOTAL_CONNECTION_TIME_SETTINGS_MAIN_NODE_TIMEOUT);  
  printf("Total connection time for the main node has been reached"); 
  if (data->data_received == 1)
  {
    memcpy(string,"Received data from main node, ",30);
    memcpy(string+30,data->main_node,main_node_length);
    memcpy(string+30+main_node_length," backup node number ",20);
    memcpy(string+50+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+51+main_node_length," in current round part ",23);
    memcpy(string+74+main_node_length,data->current_round_part,1);
    color_print(string,"green");
  }
  else
  {
    memcpy(string,"main node, ",11);
    memcpy(string+11,data->main_node,main_node_length);
    memcpy(string+11+main_node_length," backup node number ",20);
    memcpy(string+31+main_node_length,data->current_round_part_backup_node,1);
    memcpy(string+32+main_node_length," in current round part ",23);
    memcpy(string+55+main_node_length,data->current_round_part,1);
    memcpy(string+56+main_node_length," did not send any data before the timeout",41);
    color_print(string,"red");    
  }
  pointer_reset(string);
  kill((intptr_t)data->process_id, SIGTERM);
  return NULL;
}


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
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"};   
  const int SOCKET_OPTION = 1;  
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char client_address[BUFFER_SIZE];  
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));   
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  int len;
  int receive_data_result; 
  struct sockaddr_in addr, cl_addr;  

  // define macros
  #define pointer_reset_all \
  free(string); \
  string = NULL; \
  free(result); \
  result = NULL; \
  free(data); \
  data = NULL; \
  free(data2); \
  data2 = NULL; \
  free(message); \
  message = NULL; 

  // threads
  pthread_t thread_id;

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);  
    
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error creating socket","red");
    }    
    pointer_reset_all;
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) < 0)
  {
    if (MESSAGE_SETTINGS == 1)
    {
      color_print("Error setting socket options","red"); 
    }
    return 0;
  } 
  if (MESSAGE_SETTINGS == 1)
  {
    color_print("Socket created","green");
  }
 
  // convert the port to a string
  sprintf(buffer2,"%d",SEND_DATA_PORT);  
 
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
  if (bind(SOCKET, (struct sockaddr *) &addr, sizeof(addr)) < 0)
  {
   if (MESSAGE_SETTINGS == 1)
   {
     memset(string,0,strnlen(string,BUFFER_SIZE));
     memcpy(string,"Error connecting to port ",25);
     memcpy(string+25,buffer2,strnlen(buffer2,BUFFER_SIZE));
     color_print(string,"red"); 
   }
   pointer_reset_all;
   return 0;
  } 
  if (MESSAGE_SETTINGS == 1)
  {
    memset(string,0,strnlen(string,BUFFER_SIZE));
    memcpy(string,"Connected to port ",18);
    memcpy(string+18,buffer2,strnlen(buffer2,BUFFER_SIZE));
    color_print(string,"green");

    printf("Waiting for a connection...\n");
  }
  // set the maximum simultaneous connections
  listen(SOCKET, MAXIMUM_CONNECTIONS);
  for (;;)
  {
    len = sizeof(cl_addr);
    const int CLIENT_SOCKET = accept(SOCKET, (struct sockaddr *) &cl_addr, (socklen_t*)&len);
    inet_ntop(AF_INET, &(cl_addr.sin_addr), client_address, BUFFER_SIZE);
    const size_t CLIENT_ADDRESS_LENGTH = strnlen(client_address,BUFFER_SIZE);
    const size_t BUFFER2_LENGTH = strnlen(buffer2,BUFFER_SIZE);
  
    if (CLIENT_SOCKET < 0)
    {
      if (MESSAGE_SETTINGS == 1)
      {
        memset(string,0,strnlen(string,BUFFER_SIZE));
        memcpy(string,"Error accepting connection from ",32);
        memcpy(string+32,inet_ntoa(cl_addr.sin_addr),CLIENT_ADDRESS_LENGTH);
        memcpy(string+32+CLIENT_ADDRESS_LENGTH," on ",4);
        memcpy(string+36+CLIENT_ADDRESS_LENGTH,buffer2,strnlen(buffer2,BUFFER_SIZE));
        color_print(string,"red"); 
      }
      continue;
    }
    if (MESSAGE_SETTINGS == 1)
    {
      memset(string,0,strnlen(string,BUFFER_SIZE));
      memcpy(string,"Connection accepted from ",25);
      memcpy(string+25,inet_ntoa(cl_addr.sin_addr),CLIENT_ADDRESS_LENGTH);
      memcpy(string+25+CLIENT_ADDRESS_LENGTH," on ",4);
      memcpy(string+29+CLIENT_ADDRESS_LENGTH,buffer2,strnlen(buffer2,BUFFER_SIZE));
      color_print(string,"green"); 
    }

   

    if (fork() == 0)
    {     
      // create a struct for the parameters
      struct total_connection_time_thread_parameters parameters = {
        getpid(),
        client_address,
        buffer2,
        0
      };
          // create a timeout for this connection, since we need to limit the amount of time a client has to send data from once it connected
         if (pthread_create(&thread_id, NULL, &total_connection_time_thread, (void *)&parameters) != 0)
         {
           // close the forked process
           close(CLIENT_SOCKET);
           pointer_reset_all;
           _exit(0);
         }
         // set the thread to dettach once completed, since we do not need to use anything it will return.
         if (pthread_detach(thread_id) != 0)
         {
           // close the forked process
           close(CLIENT_SOCKET);
           pointer_reset_all;
           _exit(0);
         }
      

       // close the main socket, since the socket is now copied to the forked process
       close(SOCKET); 

       for (;;)
       {         
         // receive the data
         memset(buffer, 0, BUFFER_SIZE); 
         receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,0,TOTAL_CONNECTION_TIME_SETTINGS);
         if (receive_data_result < 2)
         {
           if (MESSAGE_SETTINGS == 1)
           {
             memset(string,0,strnlen(string,BUFFER_SIZE));
             memcpy(string,"Error receiving data from ",26);
             memcpy(string+26,client_address,CLIENT_ADDRESS_LENGTH);
             memcpy(string+26+CLIENT_ADDRESS_LENGTH," on port ",9);
             memcpy(string+35+CLIENT_ADDRESS_LENGTH,buffer2,BUFFER2_LENGTH);
             if (receive_data_result == 1)
             {
               memcpy(string+35+CLIENT_ADDRESS_LENGTH+BUFFER2_LENGTH,", because of a timeout issue",28);
             }
             else if (receive_data_result == 0)
             {
               memcpy(string+35+CLIENT_ADDRESS_LENGTH+BUFFER2_LENGTH,", because of a potential buffer overflow issue",46);
             }
             color_print(string,"red"); 
           }
           // close the forked process, since the client had an error sending data       
           close(CLIENT_SOCKET);
           pointer_reset_all;
           _exit(0);
         }
         else if (receive_data_result == 2)
         {
          // update the parameters, since we have received data from the client
          parameters.data_received = 1;
         }    



         // check if a certain type of message has been received         
         if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL && strncmp(server_message,"XCASH_PROOF_OF_STAKE_TEST_DATA",BUFFER_SIZE) == 0)
         {
           server_received_data_xcash_proof_of_stake_test_data(CLIENT_SOCKET,buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"NODE_TO_CONSENSUS_NODE_RECEIVE_UPDATED_NODE_LIST\"") != NULL && strncmp(server_message,"NODE_TO_CONSENSUS_NODE_RECEIVE_UPDATED_NODE_LIST",BUFFER_SIZE) == 0)
         {
           server_receive_data_socket_consensus_node_to_node(CLIENT_SOCKET,thread_id,buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS\"") != NULL && strncmp(server_message,"CONSENSUS_NODE_TO_NODES_MAIN_NODE_PUBLIC_ADDRESS",BUFFER_SIZE) == 0)
         {
           // check to see if the node should start only at the begining of a round
           if (strncmp(current_round_part,"",strnlen(current_round_part,BUFFER_SIZE)) == 0 && strncmp(current_round_part,"",strnlen(current_round_part,BUFFER_SIZE)) == 0)
           {
             if (strstr(buffer,"\"current_round_part\": \"1\"") != NULL && strstr(buffer,"\"current_round_part_backup_node\": \"0\"") != NULL)
             {
               server_receive_data_socket_consensus_node_to_node(CLIENT_SOCKET,thread_id,buffer);
             }
           }
           else
           {
             server_receive_data_socket_consensus_node_to_node(CLIENT_SOCKET,thread_id,buffer);
           }           
         }
         else
         {
           printf("Received %s from %s on port %s\r\n",buffer,client_address,buffer2);
           // send the message 
           if (send_data(CLIENT_SOCKET,buffer,1) == 1)
           {
             printf("Sent %s to %s on port %s\r\n",buffer,client_address,buffer2);
           } 
           else
           {
             memset(string,0,strnlen(string,BUFFER_SIZE));
             memcpy(string,"Error sending data to ",22);
             memcpy(string+22,client_address,CLIENT_ADDRESS_LENGTH);
             memcpy(string+22+CLIENT_ADDRESS_LENGTH," on port ",9);
             memcpy(string+31+CLIENT_ADDRESS_LENGTH,buffer2,BUFFER2_LENGTH);
             color_print(string,"red"); 
             continue;
           } 
         }

         
       
       }
     }
     #undef pointer_reset_all
     close(CLIENT_SOCKET);
   }
}