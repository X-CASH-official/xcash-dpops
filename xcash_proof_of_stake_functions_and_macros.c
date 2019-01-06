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

int parse_json_data(char* data, char* field, char *result)
{
  // Constants
  const size_t STRING_LENGTH = strnlen(field,BUFFER_SIZE);  
 
  // Variables
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* str1;
  char* str2;
  int settings = 1;
  size_t start;  
 
  // modify the field to add the field syntax
  memcpy(str,"\"",1);
  memcpy(str+1,field,STRING_LENGTH);
  memcpy(str+1+STRING_LENGTH,"\": ",3);
  // get the start of the field's data
  start = strnlen(str,BUFFER_SIZE);
  // get the pointers location to the start of the field
  str1 = strstr(data,str);
  if (str1 == NULL)
  {
     // an error has occured, get the error message
     settings = 0;
     str1 = strstr(data,"\"message\": ");
     start = 11;
     if (str1 == NULL)
     {
       // their is no error message so return an error
       memcpy(result,"An error has occured",20);
       pointer_reset(str);
       pointer_reset(data2);
       return settings;
     }
  }
  // get the end location of the data
  str2 = strstr(str1,"\r\n");
  // get the length of the field's data
  const int length = str2 - str1 - start;
  if (length <= 0)
  {
    memcpy(result,"An error has occured",20);
    pointer_reset(str);
    pointer_reset(data2);
    return settings;
  }
  // copy the field's data
  memcpy(result,&str1[start],length);
 
  // check if the return value is a string and contains a , if so remove the [""] from the start and the end, and remove the , from the end
  if (strstr(result,"\"") != NULL && strstr(result,"[") != NULL && strstr(result,",") != NULL)
  {
   memcpy(data2,&result[2],strnlen(result,BUFFER_SIZE)-5);
   memcpy(result,data2,strnlen(data2,BUFFER_SIZE)+1);
  }
  // check if the return value is a string and contains [""], if so remove [""] from the start and the end
  else if (strstr(result,"\"") != NULL && strstr(result,"[") != NULL)
  {
   memcpy(data2,&result[2],strnlen(result,BUFFER_SIZE)-3);
   memcpy(result,data2,strnlen(data2,BUFFER_SIZE)+1);
  }
  // check if the return value is a string and contains a ,if so remove the "" from the start and the end, and remove the , from the end
  else if (strstr(result,"\"") != NULL && strstr(result,",") != NULL)
  {  
   memcpy(data2,&result[1],strnlen(result,BUFFER_SIZE)-3);
   memcpy(result,data2,strnlen(data2,BUFFER_SIZE)+1);
  }
  // check if the return value is a string, if so remove the "" from the start and the end
  else if (strstr(result,"\"") != NULL)
  {
   memcpy(data2,&result[1],strnlen(result,BUFFER_SIZE)-2);
   memcpy(result,data2,strnlen(data2,BUFFER_SIZE)+1);
  }  
  // check if the return value contains , and remove it
  else if (strstr(result,",") != NULL)
  {
   memcpy(data2,result,strnlen(result,BUFFER_SIZE)-1);
   memcpy(result,data2,strnlen(data2,BUFFER_SIZE)+1);
  }
  pointer_reset(str);
  pointer_reset(data2);
  return settings;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_http_request
Description: Sends a HTTP request
Parameters:
  result - Where the result is stored
  host - The hostname or IP address
  url - The URL
  port - The port
  http_settings - The HTTP method
  HTTP_HEADERS - The HTTP headers
  HTTP_HEADERS_LENGTH - The length of the HTTP headers
  data - The request data. If sending a GET request, the data is appended to the url. If sending a POST request, the data is sent in the request body
  receive_data_timeout_settings - The timeout settings for reading the data
  title - A summary of the data sent in the POST request. This text gets printed to the console
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_http_request(char *result, const char* host, const char* url, const int port, const char* http_settings, const char* HTTP_HEADERS[], size_t HTTP_HEADERS_LENGTH, const char* data, const int receive_data_timeout_settings, const char* title, const int settings)
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

  // create the HTTP request
  append_string(message,http_settings);
  append_string(message," ");
  append_string(message,url);
  if (http_settings == "GET")
  {
    append_string(message,"?");
    append_string(message,data);
  }
  append_string(message," HTTP/1.1\r\nHost: ");
  append_string(message,host);
  append_string(message,"\r\n");
  for (count = 0; count < HTTP_HEADERS_LENGTH; count++)
  {
    append_string(message,HTTP_HEADERS[count]);
    append_string(message,"\r\n");
  }
  if (http_settings == "POST")
  {
    append_string(message,"Content-Length: ");
    sprintf(str, "%d", (int)strnlen(data,BUFFER_SIZE));
    append_string(message,str);
  } 
  append_string(message,"\r\n\r\n");    
  if (http_settings == "POST")
  {
    append_string(message,data);
  }  
  memset(&response, 0, sizeof(response));

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
  {
    if (settings == 1)
    {
      color_print("Error creating socket for sending a post request","red");
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_RCVTIMEO = sets a receiving timeout for the socket
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) < 0)
  {
    if (settings == 1)
    {
      color_print("Error setting socket timeout for sending a post request","red"); 
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }  

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(host); 
  if (HOST_NAME == NULL)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,host,strnlen(host,BUFFER_SIZE));
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }

  // convert the port to a string  
  sprintf(buffer2,"%d",port);  

  // get the length of buffer2 and host, since they will not change at this point and we need them for faster string copying
  const size_t buffer2_length = strnlen(buffer2,BUFFER_SIZE);
  const size_t host_length = strnlen(host,BUFFER_SIZE);
  
  struct sockaddr_in serv_addr;
  memset(&serv_addr,0,sizeof(serv_addr));
  /* setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)HOST_NAME->h_addr_list[0])));
  serv_addr.sin_port = htons(port);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,host,host_length);
      memcpy(str+20+host_length," on port ",9);
      memcpy(str+29+host_length,buffer2,buffer2_length);
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
  if (settings == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Connected to ",13);
    memcpy(str+13,host,host_length);
    memcpy(str+13+host_length," on port ",9);
    memcpy(str+22+host_length,buffer2,buffer2_length);
    color_print(str,"green"); 
  }
    
  // send the message
  if (settings == 1)
  {
    printf("Sending %s to %s on port %s\r\n",title,host,buffer2);
  }
  if (send_data(SOCKET,message,0) == 0)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,host,host_length);
      memcpy(str+22+host_length," on port ",9);
      memcpy(str+31+host_length,buffer2,buffer2_length);
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
   
  // get the result
  receive_data_result = receive_data(SOCKET,message,"{",1,receive_data_timeout_settings);
  if (receive_data_result < 2)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,host,host_length);
      memcpy(str+26+host_length," on port ",9);
      memcpy(str+35+host_length,buffer2,buffer2_length);
      if (receive_data_result == 1)
      {
        memcpy(str+35+host_length+buffer2_length,", because of a timeout issue",28);
      }
      else if (receive_data_result == 0)
      { 
        memcpy(str+35+host_length+buffer2_length,", because of a potential buffer overflow issue",46);
      }
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }

  // check if the data recived is correct
  if (strstr(message,"{") == NULL && strstr(message,"error") == NULL)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,host,host_length);
      memcpy(str+26+host_length," on port ",9);
      memcpy(str+35+host_length,buffer2,buffer2_length);
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);  
    return 0;
  }
  if (settings == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Received data from ",19);
    memcpy(str+19,host,host_length);
    memcpy(str+19+host_length," on port ",9);
    memcpy(str+28+host_length,buffer2,buffer2_length);
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
    
  pointer_reset(str);
  pointer_reset(message);
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_data_socket
Description: Send a message through a socket
Parameters:
  result - The result from the host
  host - The host to send the message to
  port - The port to send the message through
  data - The message
  receive_data_timeout_settings - the timeout settings for reading the data
  title - A summary of the data sent to the host. This text gets printed to the console
  settings - 1 to display error and success messages, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data_socket(char *result, const char* host, const int port, const char* data, const int receive_data_timeout_settings, const char* title, const int settings)
{ 
  // Constants
  const size_t title_length = strnlen(title,BUFFER_SIZE);
  const size_t host_length = strnlen(host,BUFFER_SIZE);
  const struct timeval SOCKET_TIMEOUT = {SOCKET_TIMEOUT_SETTINGS, 0};   
  
  // Variables 
  char buffer2[BUFFER_SIZE];
  char* str = (char*)calloc(BUFFER_SIZE,sizeof(char)); 
  char* message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  int receive_data_result;

  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
  { 
    if (settings == 1)
    {
      memcpy(str,"Error creating socket for sending data to ",42);
      memcpy(str+42,host,host_length);
      color_print(str,"red");
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_RCVTIMEO = allow the socket to receive a timeout
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&SOCKET_TIMEOUT, sizeof(struct timeval)) < 0)
  {
    if (settings == 1)
    {
      memcpy(str,"Error setting socket timeout for sending data to ",49);
      memcpy(str+49,host,host_length);
      color_print(str,"red");
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  } 

  // convert the hostname if used, to an IP address
  const struct hostent* host_name = gethostbyname(host); 
  if (host_name == NULL)
  {
    if (settings == 1)
    {
      memcpy(str,"Error invalid hostname of ",26);
      memcpy(str+26,host,host_length);
      color_print(str,"red");
    }
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }
    
  // convert the port to a string  
  sprintf(buffer2,"%d",port); 
   
  const size_t buffer2_length = strnlen(buffer2,BUFFER_SIZE);
  
  struct sockaddr_in serv_addr;
  /* setup the connection
  AF_INET = IPV4
  use htons to convert the port from host byte order to network byte order short
  */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr*)host_name->h_addr_list[0])));
  serv_addr.sin_port = htons(port);

  // connect to the socket
  if (connect(SOCKET,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
  {
    if (settings == 1)
    {
      memcpy(str,"Error connecting to ",20);
      memcpy(str+20,host,host_length);
      memcpy(str+20+host_length," on port ",9);
      memcpy(str+29+host_length,buffer2,buffer2_length);
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
  if (settings == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Connected to ",13);
    memcpy(str+13,host,host_length);
    memcpy(str+13+host_length," on port ",9);
    memcpy(str+22+host_length,buffer2,buffer2_length);
    color_print(str,"green"); 
  }

  // send the message 
  memcpy(message,data,strnlen(data,BUFFER_SIZE));
  if (settings == 1)
  {
    printf("Sending %s to %s on port %s\r\n",title,host,buffer2);
  }
  if (send_data(SOCKET,message,1) == 0)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error sending data to ",22);
      memcpy(str+22,host,host_length);
      memcpy(str+22+host_length," on port ",9);
      memcpy(str+31+host_length,buffer2,buffer2_length);
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);    
    return 0;
  }
    
  // get the result
  receive_data_result = receive_data(SOCKET,message,SOCKET_END_STRING,1,receive_data_timeout_settings);
  if (receive_data_result < 2)
  {
    if (settings == 1)
    {
      memset(str,0,strnlen(str,BUFFER_SIZE));
      memcpy(str,"Error receiving data from ",26);
      memcpy(str+26,host,host_length);
      memcpy(str+26+host_length," on port ",9);
      memcpy(str+35+host_length,buffer2,buffer2_length);
      if (receive_data_result == 1)
      {
        memcpy(str+35+host_length+buffer2_length,", because of a timeout issue",28);
      }
      else if (receive_data_result == 0)
      {   
        memcpy(str+35+host_length+buffer2_length,", because of a potential buffer overflow issue",46);
      }
      color_print(str,"red"); 
    }
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
     
  memcpy(result,message,strnlen(message,BUFFER_SIZE));
  if (settings == 1)
  {
    memset(str,0,strnlen(str,BUFFER_SIZE));
    memcpy(str,"Received data from ",19);
    memcpy(str+19,host,host_length);
    memcpy(str+19+host_length," on port ",9);
    memcpy(str+28+host_length,buffer2,buffer2_length);
    color_print(str,"green");
  }
  
  // close the socket
  close(SOCKET);

  pointer_reset(message);
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: string_replace
Description: String replace
Parameters:
  data - The string to replace the data
  str1 - The string to be replaced
  str2 - The string to replace the other string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int string_replace(char *data,const char* str1, const char* str2)
{
  #define REPLACE_STRING "|REPLACE_STRING|"
  
  // check if the str to replace exist in the string
  if (strstr(data,str1) != NULL)
  { 
    // Variables
    char* datacount = (char*)calloc(BUFFER_SIZE,sizeof(char));
    char* datacopy = (char*)calloc(BUFFER_SIZE,sizeof(char));
    char* string;
    size_t data_length;
    size_t str2_length;
    size_t start;
    size_t total = 0;
    size_t count = 0;

    // get the occurences of str1    
    memcpy(datacount,data,strnlen(data,BUFFER_SIZE));
    while(datacount = strstr(datacount, str1))
    {
      total++;
      datacount++;
    } 

    // replace the string with the REPLACE_STRING
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(REPLACE_STRING,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,str1),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,str1)+strnlen(str1,BUFFER_SIZE);
           
      // copy the bytes before str1 to the new string
      memcpy(datacopy,data,start);
      // copy str2 to the new string
      memcpy(datacopy+start,REPLACE_STRING,str2_length);
      // copy the bytes after str1 to the new string
      memcpy(datacopy+start+str2_length,string,strnlen(string,BUFFER_SIZE));
      // copy the new string to the string pointer
      memset(data,0,data_length);
      memcpy(data,datacopy,strnlen(datacopy,BUFFER_SIZE));
    }
    // replace the REPLACE_STRING with str2
    for (count = 0; count < total; count++)
    {
      // reset the variables
      memset(datacopy,0,strnlen(datacopy,BUFFER_SIZE));
      data_length = strnlen(data,BUFFER_SIZE);
      str2_length = strnlen(str2,BUFFER_SIZE);
      start = data_length - strnlen(strstr(data,REPLACE_STRING),BUFFER_SIZE);
   
      // get a pointer to where the rest of the data string should be copied to
      string = strstr(data,REPLACE_STRING)+strnlen(REPLACE_STRING,BUFFER_SIZE);
           
      // copy the bytes before REPLACE_STRING to the new string
      memcpy(datacopy,data,start);
      // copy str2 to the new string
      memcpy(datacopy+start,str2,str2_length);
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
}


/*
-----------------------------------------------------------------------------------------------------------
Name: send_data
Description: Sends data to a socket
Parameters:
  socket - The socket
  data - The data to send
  appendstring - 1 to append an end string to the data, else 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int send_data(const int socket, char* data, const int appendstring)
{
  if (appendstring == 1)
  {
    memcpy(data+strnlen(data,BUFFER_SIZE),SOCKET_END_STRING,5);
  }    
  const int TOTAL = strnlen(data,BUFFER_SIZE);
  int sent = 0;
  int bytes = 0;
  do {
    bytes = write(socket,data+sent,TOTAL-sent);
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
  socket - The socket
  message - Where the data is stored
  string - The end string to know the socket is done receving data
  settings - 1 if a timeout is needed, otherwise 0
Return: 0 if an error would have occured from a buffer overflow, 1 if a timeout has occured, 2 if successful
-----------------------------------------------------------------------------------------------------------
*/

int receive_data(const int socket, char *message, const char* string, const int settings, const int socket_timeout_settings)
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
    if (recvfrom(socket, buffer, BUFFER_SIZE, MSG_DONTWAIT | MSG_PEEK, NULL, NULL) > BUFFER_SIZE)
    {
      pointer_reset(data);
      return 0;
    }    
    // read the socket to see if there is any data, use MSG_DONTWAIT so we dont block the program if there is no data
    recvfrom(socket, buffer, BUFFER_SIZE, MSG_DONTWAIT, NULL, NULL);  
    if (buffer[0] != '\0' && strstr(buffer,string) == NULL)
    {
      // there is data, but this is not the final data
      append_string(message,buffer);
    }
    if (buffer[0] != '\0' && strstr(buffer,string) != NULL)
    {
      // there is data, and this is the final data
      append_string(message,buffer);
      // if the final message has the SOCKET_END_STRING in the message, remove it
      if (strstr(message,SOCKET_END_STRING) != NULL)
      {
        // remove SOCKET_END_STRING from the message
        memcpy(data,message,strnlen(message,BUFFER_SIZE) - 5);
        memset(message, 0, strnlen(message,BUFFER_SIZE));
        memcpy(message,data,strnlen(data,BUFFER_SIZE));
        pointer_reset(data);
      }
      break;
    }

    // check for a timeout in receiving data
    if (settings == 1)
    {
      count++;
      if (count > (socket_timeout_settings * 5))
      {
        pointer_reset(data);
        return 1;
      }
    } 

    usleep(200000);   
  }
  return 2;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: random_string
Description: Creates a random string of specified length
Parameters:
  result - The string where you want the random string to be saved to
  length - The length of the random string
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int random_string(char *result, const size_t length)
{
  // Constants
  const size_t minimum = 0;
  const size_t maximum = 61;
  
  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t count;
  
  memcpy(data,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",62);
  for (count = 0; count < length; count++)
  {
    memcpy(result+count,&data[((rand() % (maximum - minimum + 1)) + minimum)],1);
  }
  pointer_reset(data); 
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: get_public_address
Description: Gets the public address of your wallet
Parameters:
  result - The string where you want the public address to be saved to
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int get_public_address(char *result, const int settings)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"get_address\"}",RECEIVE_DATA_TIMEOUT_SETTINGS,"get public address",settings) <= 0)
  {  
    pointer_reset(data);   
    return 0;
  }
  
  if (parse_json_data(data,(char*)"address",result) == 0)
  {
    pointer_reset(data); 
    return 0;
  }
  
  // check if the returned data is valid
  if (strnlen(result,BUFFER_SIZE) != XCASH_WALLET_LENGTH && strncmp(result,XCASH_WALLET_PREFIX,3) != 0)
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
  result - The string where you want the xcash_proof_of_stake_signature to be saved to
  random_data - The random character string that is created and then signed. This is random to increase security
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int sign_data(char *message, const int settings)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* random_data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));

  // get the public address
  if (get_public_address(public_address,0) == 0)
  {  
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data); 
    pointer_reset(string);  
    return 0;
  }

  // create the random data
  if (random_string(random_data,RANDOM_STRING_LENGTH) == 0)
  {
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data); 
    pointer_reset(string);  
    return 0;
  }

  // create the message
  size_t message_length = strnlen(message,BUFFER_SIZE)-1;
  memcpy(result,message,message_length);
  memcpy(result+message_length," \"public_address\": \"",20);
  memcpy(result+message_length+20,public_address,XCASH_WALLET_LENGTH);
  memcpy(result+message_length+20+XCASH_WALLET_LENGTH,"\",\r\n \"data\": \"",14);
  memcpy(result+message_length+34+XCASH_WALLET_LENGTH,random_data,RANDOM_STRING_LENGTH);
  memcpy(result+message_length+34+XCASH_WALLET_LENGTH+RANDOM_STRING_LENGTH,"\",\r\n}",5);
  if (string_replace(result,"\"","\\\"") == 0)
  {
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data); 
    pointer_reset(string);  
    return 0;
  }
 

  // sign_data
  const size_t result_length = strnlen(result,BUFFER_SIZE);
  memcpy(string,"{\"jsonrpc\":\"2.0\",\"id\":\"0\",\"method\":\"sign\",\"params\":{\"data\":\"",60);
  memcpy(string+60,result,result_length);
  memcpy(string+60+result_length,"\"}}",3);
  memset(result,0,result_length);

  if (send_http_request(data,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"sign data",settings) <= 0)
  {  
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data);
    pointer_reset(string);   
    return 0;
  }  
  if (parse_json_data(data,(char*)"signature",result) == 0)
  {
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data);
    pointer_reset(string); 
    return 0;
  }

  // check if the returned data is valid
  if (strnlen(result,BUFFER_SIZE) != XCASH_SIGN_DATA_LENGTH && strncmp(result,XCASH_SIGN_DATA_PREFIX,5) != 0)
  {
    pointer_reset(public_address);
    pointer_reset(random_data); 
    pointer_reset(result); 
    pointer_reset(data);
    pointer_reset(string); 
    return 0;
  }
  
  // create the message
  message_length = strnlen(message,BUFFER_SIZE) - 1;
  const size_t xcash_proof_of_stake_signature_length = strnlen(result,BUFFER_SIZE);
  memcpy(message+message_length," \"public_address\": \"",20);
  memcpy(message+message_length+20,public_address,XCASH_WALLET_LENGTH);
  memcpy(message+message_length+20+XCASH_WALLET_LENGTH,"\",\r\n \"data\": \"",14);
  memcpy(message+message_length+34+XCASH_WALLET_LENGTH,random_data,RANDOM_STRING_LENGTH);
  memcpy(message+message_length+34+XCASH_WALLET_LENGTH+RANDOM_STRING_LENGTH,"\",\r\n \"xcash_proof_of_stake_signature\": \"",40);
  memcpy(message+message_length+74+XCASH_WALLET_LENGTH+RANDOM_STRING_LENGTH,result,xcash_proof_of_stake_signature_length);
  memcpy(message+message_length+74+XCASH_WALLET_LENGTH+RANDOM_STRING_LENGTH+xcash_proof_of_stake_signature_length,"\",\r\n}",5);

  pointer_reset(public_address);
  pointer_reset(random_data); 
  pointer_reset(result); 
  pointer_reset(data);
  pointer_reset(string); 
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: verify_data
Description: Verifies signed data, for receiving data securely
Parameters:
  random_data - The signed data
  public_address - The public address who signed the data
  xcash_proof_of_stake_signature - The xcash_proof_of_stake_signature of the signed data
Return: 0 if the signed data is not verified, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int verify_data(char* message, const int settings)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"}; 
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);
    
  // Variables
  char* public_address = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* xcash_proof_of_stake_signature = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  char* string = (char*)calloc(BUFFER_SIZE,sizeof(char));
  size_t message_length = strnlen(message,BUFFER_SIZE) - 134;

  // parse the message
  if (parse_json_data(message,(char*)"public_address",public_address) == 0)
  {
    pointer_reset(public_address);
    pointer_reset(xcash_proof_of_stake_signature);  
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(string); 
    return 0;
  }
  if (parse_json_data(message,(char*)"xcash_proof_of_stake_signature",xcash_proof_of_stake_signature) == 0)
  {
    pointer_reset(public_address);
    pointer_reset(xcash_proof_of_stake_signature);  
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(string); 
    return 0;
  }

  // create the message
  memcpy(result,message,message_length);
  memcpy(result+message_length,"}",1);
  if (string_replace(result,"\"","\\\"") == 0)
  {
    pointer_reset(public_address);
    pointer_reset(xcash_proof_of_stake_signature);  
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(string); 
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

  if (send_http_request(result,"127.0.0.1","/json_rpc",XCASH_WALLET_PORT,"POST", HTTP_HEADERS, HTTP_HEADERS_LENGTH,string,RECEIVE_DATA_TIMEOUT_SETTINGS,"verify data",settings) <= 0)
  { 
    pointer_reset(public_address);
    pointer_reset(xcash_proof_of_stake_signature);  
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(string);   
    return 0;
  } 
    
  if (parse_json_data(result,(char*)"good",data) == 0)
  {
    pointer_reset(public_address);
    pointer_reset(xcash_proof_of_stake_signature);  
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(string); 
    return 0;
  }

  // check if the returned data is valid
  if (strncmp(data,"true",BUFFER_SIZE) != 0)
  {
     pointer_reset(public_address);
     pointer_reset(xcash_proof_of_stake_signature);  
     pointer_reset(result);
     pointer_reset(data);
     pointer_reset(string); 
     return 0;
  }
 
  pointer_reset(public_address);
  pointer_reset(xcash_proof_of_stake_signature);  
  pointer_reset(result);
  pointer_reset(data);
  pointer_reset(string); 
  return 1;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: server_received_data_xcash_proof_of_stake_test_data
Description: Runs the code when the server receives the xcash_proof_of_stake_test_data message
Parameters:
  message - The message
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int server_received_data_xcash_proof_of_stake_test_data(const int client_socket, char* message)
{
  // verify the message
  if (verify_data(message,0) == 0)
  {   
    return 0;
  }
  else
  {
    if (send_data(client_socket,message,1) == 1)
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
Thread functions
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
  kill((intptr_t)data->process_id, SIGTERM);
  return NULL;
}


/*
-----------------------------------------------------------------------------------------------------------
Name: create_server
Description: Creates the server
Parameters:
  settings - 1 to display error and success messages, otherwise 0
Return: 0 if an error has occured, 1 if successfull
-----------------------------------------------------------------------------------------------------------
*/

int create_server(const int settings)
{
  // Constants
  const char* HTTP_HEADERS[] = {"Content-Type: application/json","Accept: application/json"};   
  const int SOCKET_OPTION = 1;  
  const size_t HTTP_HEADERS_LENGTH = sizeof(HTTP_HEADERS)/sizeof(HTTP_HEADERS[0]);

  // Variables
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  char client_address[CLADDR_LEN];
  char* string;
  char* result;
  char* data;
  char* data2;
  char* message; 
  int len;
  int receive_data_result; 
  size_t string_length;
  struct sockaddr_in addr, cl_addr;

  // threads
  pthread_t thread_id;

  // set the main process to ignore if forked processes return a value or not, since the timeout for the total connection time is run on a different thread
  signal(SIGCHLD, SIG_IGN);  
 
  // allocate the memory we need in the main process, this way when a forked process is created the memory will also be allocated for each forked process
  string = (char*)calloc(BUFFER_SIZE,sizeof(char));  
  result = (char*)calloc(BUFFER_SIZE,sizeof(char));
  data = (char*)calloc(BUFFER_SIZE,sizeof(char));
  data2 = (char*)calloc(BUFFER_SIZE,sizeof(char));
  message = (char*)calloc(BUFFER_SIZE,sizeof(char));
  
  /* Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  */
  const int SOCKET = socket(AF_INET, SOCK_STREAM, 0);
  if (SOCKET < 0)
  {
    if (settings == 1)
    {
      color_print("Error creating socket","red");
    }    
    pointer_reset(string);
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(data2);
    pointer_reset(message);
    return 0;
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) < 0)
  {
    if (settings == 1)
    {
      color_print("Error setting socket options","red"); 
    }
    return 0;
  } 
  if (settings == 1)
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
   if (settings == 1)
   {
     memset(string,0,strnlen(string,BUFFER_SIZE));
     memcpy(string,"Error connecting to port ",25);
     memcpy(string+25,buffer2,strnlen(buffer2,BUFFER_SIZE));
     color_print(string,"red"); 
   }
   pointer_reset(string);
   pointer_reset(result);
   pointer_reset(data);
   pointer_reset(data2);
   pointer_reset(message);
   return 0;
  } 
  if (settings == 1)
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
    const size_t client_address_length = strnlen(inet_ntoa(cl_addr.sin_addr),BUFFER_SIZE);
  
    if (CLIENT_SOCKET < 0)
    {
      if (settings == 1)
      {
        memset(string,0,strnlen(string,BUFFER_SIZE));
        memcpy(string,"Error accepting connection from ",32);
        memcpy(string+32,inet_ntoa(cl_addr.sin_addr),client_address_length);
        memcpy(string+32+client_address_length," on ",4);
        memcpy(string+36+client_address_length,buffer2,strnlen(buffer2,BUFFER_SIZE));
        color_print(string,"red"); 
      }
      continue;
    }
    if (settings == 1)
    {
      memset(string,0,strnlen(string,BUFFER_SIZE));
      memcpy(string,"Connection accepted from ",25);
      memcpy(string+25,inet_ntoa(cl_addr.sin_addr),client_address_length);
      memcpy(string+25+client_address_length," on ",4);
      memcpy(string+29+client_address_length,buffer2,strnlen(buffer2,BUFFER_SIZE));
      color_print(string,"green"); 
    }

    inet_ntop(AF_INET, &(cl_addr.sin_addr), client_address, CLADDR_LEN);

    if (fork() == 0)
    {     
      // create a struct for the parameters
      struct total_connection_time_thread_parameters parameters = {
        getpid(),
        client_address,
        buffer2,
        0
      };
       if (settings == 1)
       {
         // create a timeout for this connection, since we need to limit the amount of time a client has to send data from once it connected
         if (pthread_create(&thread_id, NULL, &total_connection_time_thread, (void *)&parameters) != 0)
         {
           // close the forked process
           close(CLIENT_SOCKET);
           pointer_reset(result);
           pointer_reset(data);
           pointer_reset(data2);
           pointer_reset(message);
           _exit(0);
         }
         // set the thread to dettach once completed, since we do not need to use anything it will return.
         if (pthread_detach(thread_id) != 0)
         {
           // close the forked process
           close(CLIENT_SOCKET);
           pointer_reset(result);
           pointer_reset(data);
           pointer_reset(data2);
           pointer_reset(message);
           _exit(0);
         }
      }

       // close the main socket, since the socket is now copied to the forked process
       close(SOCKET); 

       for (;;)
       {
         const size_t client_address_length = strnlen(client_address,BUFFER_SIZE);
         const size_t buffer2_length = strnlen(buffer2,BUFFER_SIZE);
         // receive the data
         memset(buffer, 0, BUFFER_SIZE); 
         receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,0,TOTAL_CONNECTION_TIME_SETTINGS);
         if (receive_data_result < 2)
         {
           if (settings == 1)
           {
             memset(string,0,strnlen(string,BUFFER_SIZE));
             memcpy(string,"Error receiving data from ",26);
             memcpy(string+26,client_address,client_address_length);
             memcpy(string+26+client_address_length," on port ",9);
             memcpy(string+35+client_address_length,buffer2,buffer2_length);
             if (receive_data_result == 1)
             {
               memcpy(string+35+client_address_length+buffer2_length,", because of a timeout issue",28);
             }
             else if (receive_data_result == 0)
             {
               memcpy(string+35+client_address_length+buffer2_length,", because of a potential buffer overflow issue",46);
             }
             color_print(string,"red"); 
           }
           // close the forked process, since the client had an error sending data       
           close(CLIENT_SOCKET);
           pointer_reset(string);
           pointer_reset(result);
           pointer_reset(data);
           pointer_reset(data2);
           pointer_reset(message);
           _exit(0);
         }
         else if (receive_data_result == 2)
         {
          // update the parameters, since we have received data from the client
          parameters.data_received = 1;
         }    



         // check if a certain type of message has been received         
         if (strstr(buffer,"\"message_settings\": \"XCASH_PROOF_OF_STAKE_TEST_DATA\"") != NULL)
         {
           server_received_data_xcash_proof_of_stake_test_data(CLIENT_SOCKET,buffer);
         }
         else if (strstr(buffer,"\"message_settings\": \"TESTNODE_TO_NODES\"") != NULL)
         {
           //
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
             memcpy(string+22,client_address,client_address_length);
             memcpy(string+22+client_address_length," on port ",9);
             memcpy(string+31+client_address_length,buffer2,buffer2_length);
             color_print(string,"red"); 
             continue;
           } 
         }

         
       
       }
     }
     close(CLIENT_SOCKET);
   }
}