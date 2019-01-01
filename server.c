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

/*
-----------------------------------------------------------------------------------------------------------
Using define statements instead of constants for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/

#define PORT 18285 // The X-CASH RPC port
#define SEND_DATA_PORT 18288 // The port that is used by all nodes to send and receive data
#define CLIENT_AND_SERVER_DATA_PORT 8000 // The port that is used to send data locally between a nodes client and server program
#define BUFFER_SIZE 64000
#define CLADDR_LEN 100
#define MAXIMUM_CONNECTIONS 100 // The maximum connections a node can have at one time
#define SOCKET_TIMEOUT_SETTINGS 1 // The time in between read calls where there is no data
#define RECEIVE_DATA_TIMEOUT_SETTINGS 5 // The maximum amount of time to wait for the total data, once data has been read
#define TOTAL_CONNECTION_TIME_SETTINGS 10 // The total time a client is given to connect to the server and send data
#define SOCKET_END_STRING "|END|" // End string when sending data between clients, or client to server on the same computer, to signal the end of sending data

/*
-----------------------------------------------------------------------------------------------------------
Using define macros instead of functions for increased efficiency
-----------------------------------------------------------------------------------------------------------
*/



/*
-----------------------------------------------------------------------------------------------------------
Name: color_print
Description: Prints a string in a color
Parameters:
  string - char*
  color - char*
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define color_print(string,color) \
color == "red" ? printf("\033[1;31m%s\033[0m\n",string) : \
color == "green" ? printf("\033[1;32m%s\033[0m\n",string) : \
color == "yellow" ? printf("\033[1;33m%s\033[0m\n",string) : \
color == "blue" ? printf("\033[1;34m%s\033[0m\n",string) : \
color == "purple" ? printf("\033[1;35m%s\033[0m\n",string) : \
color == "lightblue" ? printf("\033[1;36m%s\033[0m\n",string) : \
printf("%s",string);


/*
-----------------------------------------------------------------------------------------------------------
Name: append_string
Description: Appends strings securely to stop buffer overflows, and to always null terminate the string 
Parameters:
  string1 - char*
  string2 - char*
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define append_string(string1,string2) \
strncat(string1,string2,BUFFER_SIZE - strlen(string1) - 1);


/*
-----------------------------------------------------------------------------------------------------------
Name: pointer_reset
Description: Reset the memory used by the pointer, and sets the pointer to NULL to avoid a dangling pointer
Parameters:
  pointer - Any pointer type
Return: Writes the correct code
-----------------------------------------------------------------------------------------------------------
*/

#define pointer_reset(pointer) \
free(pointer); \
pointer = NULL;


/*
-----------------------------------------------------------------------------------------------------------
Global structures
-----------------------------------------------------------------------------------------------------------
*/


 struct total_connection_time_thread_parameters {
    pid_t process_id; // Holds the forked process ID that the client is connected to
    char* client_address; // Holds the IP address of the client
    char* port; // Holds the port number of the client 
    int data_received; // 1 if the server has received data from the client, otherwise 0
};


/*
-----------------------------------------------------------------------------------------------------------
Function prototypes
-----------------------------------------------------------------------------------------------------------
*/

int parse_json_data(char* data, char* field, char *result);
int send_http_request(char *result, const char* host, const char* url, const int port, const char* http_settings, const char* HTTP_HEADERS[], size_t HTTP_HEADERS_LENGTH, const char* data, const int receive_data_timeout_settings, const char* title);
int string_replace(char *data,const char* str1, const char* str2);
int send_data(const int socket, char* data, const int appendstring);
int receive_data(const int socket, char *message, const char* string, const int settings, const int socket_timeout_settings);
// threads
void* total_connection_time_thread(void* parameters);


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
  const size_t STRING_LENGTH = strlen(field);   

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
  start = strlen(str);
  // get the pointers location to the start of the field
  str1 = strstr(data,str);
  if (str1 == NULL)
  {
     // an error has occured, get the error message
     settings = 0;
     str1 = strstr(data,"\"message\": ");
     start = strlen("\"message\": ");
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
  // copy the field's data
  memcpy(result,&str1[start],length);

  // check if the return value is a string and contains a , if so remove the [""] from the start and the end, and remove the , from the end
  if (strstr(result,"\"") != NULL && strstr(result,"[") != NULL && strstr(result,",") != NULL)
  {
   memcpy(data2,&result[2],strlen(result)-5);
   memcpy(result,data2,strlen(data2)+1);
  }
  // check if the return value is a string and contains [""], if so remove [""] from the start and the end
  else if (strstr(result,"\"") != NULL && strstr(result,"[") != NULL)
  {
   memcpy(data2,&result[2],strlen(result)-3);
   memcpy(result,data2,strlen(data2)+1);
  }
  // check if the return value is a string and contains a ,if so remove the "" from the start and the end, and remove the , from the end
  else if (strstr(result,"\"") != NULL && strstr(result,",") != NULL)
  {   
   memcpy(data2,&result[1],strlen(result)-3);
   memcpy(result,data2,strlen(data2)+1);
  }
  // check if the return value is a string, if so remove the "" from the start and the end
  else if (strstr(result,"\"") != NULL)
  {
   memcpy(data2,&result[1],strlen(result)-2);
   memcpy(result,data2,strlen(data2)+1);
  }  
  // check if the return value contains , and remove it
  else if (strstr(result,",") != NULL)
  {
   memcpy(data2,result,strlen(result)-1);
   memcpy(result,data2,strlen(data2)+1);
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

int send_http_request(char *result, const char* host, const char* url, const int port, const char* http_settings, const char* HTTP_HEADERS[], size_t HTTP_HEADERS_LENGTH, const char* data, const int receive_data_timeout_settings, const char* title)
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
    sprintf(str, "%d", (int)strlen(data));
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
    color_print("Error creating socket for sending a post request","red");
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
    color_print("Error setting socket timeout for sending a post request","red"); 
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }  

  // convert the hostname if used, to an IP address
  const struct hostent* HOST_NAME = gethostbyname(host); 
  if (HOST_NAME == NULL)
  {
    memset(str,0,strlen(str));
    memcpy(str,"Error invalid hostname of ",26);
    memcpy(str+26,host,strlen(host));
    color_print(str,"red"); 
    pointer_reset(str);
    pointer_reset(message);
    return 0;
  }

  // convert the port to a string  
  sprintf(buffer2,"%d",port);  

  // get the length of buffer2 and host, since they will not change at this point and we need them for faster string copying
  const size_t buffer2_length = strlen(buffer2);
  const size_t host_length = strlen(host);
  
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
    memset(str,0,strlen(str));
    memcpy(str,"Error connecting to ",20);
    memcpy(str+20,host,host_length);
    memcpy(str+20+host_length," on port ",9);
    memcpy(str+29+host_length,buffer2,buffer2_length);
    color_print(str,"red"); 
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
  memset(str,0,strlen(str));
  memcpy(str,"Connected to ",13);
  memcpy(str+13,host,host_length);
  memcpy(str+13+host_length," on port ",9);
  memcpy(str+22+host_length,buffer2,buffer2_length);
  color_print(str,"green"); 
    
  // send the message
  printf("Sending %s to %s on port %s\r\n",title,host,buffer2);
  if (send_data(SOCKET,message,0) == 0)
  {
    memset(str,0,strlen(str));
    memcpy(str,"Error sending data to ",22);
    memcpy(str+22,host,host_length);
    memcpy(str+22+host_length," on port ",9);
    memcpy(str+31+host_length,buffer2,buffer2_length);
    color_print(str,"red"); 
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }
   
  // get the result
  receive_data_result = receive_data(SOCKET,message,"{",1,receive_data_timeout_settings);
  if (receive_data_result < 2)
  {
    memset(str,0,strlen(str));
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
    pointer_reset(str);
    pointer_reset(message);   
    return 0;
  }

  // check if the data recived is correct
  if (strstr(message,"{") == NULL && strstr(message,"error") == NULL)
  {
    memset(str,0,strlen(str));
    memcpy(str,"Error receiving data from ",26);
    memcpy(str+26,host,host_length);
    memcpy(str+26+host_length," on port ",9);
    memcpy(str+35+host_length,buffer2,buffer2_length);
    color_print(str,"red"); 
    pointer_reset(str);
    pointer_reset(message);  
    return 0;
  }
  memset(str,0,strlen(str));
  memcpy(str,"Received data from ",19);
  memcpy(str+19,host,host_length);
  memcpy(str+19+host_length," on port ",9);
  memcpy(str+28+host_length,buffer2,buffer2_length);
  color_print(str,"green"); 
     
  // close the socket
  close(SOCKET);

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
    
  pointer_reset(str);
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
Return: 0 if an error has occured, 1 if successfull.
-----------------------------------------------------------------------------------------------------------
*/

int string_replace(char *data,const char* str1, const char* str2)
{
  // check if the str to replace exist in the string
  if (strstr(data,str1) != NULL)
  {
    // copy the data to a temporary string and reset the data
    char* datacopy = (char*)calloc(BUFFER_SIZE,sizeof(char));
    memcpy(datacopy,data,strlen(data));
    memset(data,0,strlen(data));
    // get a pointer to the start of the string to be replaced
    char* start_str = strstr(datacopy,str1);
    // copy the string from the start of the string to the data to be replaced
    const int START = strlen(datacopy) - strlen(start_str);
    // create the new string
    memcpy(data,datacopy,START);
    memcpy(data+START,str2,strlen(str2));
    memset(datacopy,0,strlen(datacopy));
    memcpy(datacopy,&start_str[strlen(str1)],strlen(start_str)-strlen(str1));
    append_string(data,datacopy);
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
    memcpy(data+strlen(data),SOCKET_END_STRING,5);
  }    
  const int TOTAL = strlen(data);
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

  memset(message, 0, strlen(message));  
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
        memcpy(data,message,strlen(message) - 5);
        memset(message, 0, strlen(message));
        memcpy(message,data,strlen(data));
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
  int client_address_length = strlen(data->client_address);
  int data_port_length = strlen(data->port);
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
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main()
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
    color_print("Error creating socket","red");
    pointer_reset(string);
    pointer_reset(result);
    pointer_reset(data);
    pointer_reset(data2);
    pointer_reset(message);
    exit(0);
  }

  /* Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  */
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) < 0)
  {
    color_print("Error setting socket options","red"); 
    return 0;
  } 
  color_print("Socket created","green");
 
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
   memset(string,0,strlen(string));
   memcpy(string,"Error connecting to port ",25);
   memcpy(string+25,buffer2,strlen(buffer2));
   color_print(string,"red"); 
   pointer_reset(string);
   pointer_reset(result);
   pointer_reset(data);
   pointer_reset(data2);
   pointer_reset(message);
   exit(0);
  } 
  memset(string,0,strlen(string));
  memcpy(string,"Connected to port ",18);
  memcpy(string+18,buffer2,strlen(buffer2));
  color_print(string,"green");

  printf("Waiting for a connection...\n");
  // set the maximum simultaneous connections
  listen(SOCKET, MAXIMUM_CONNECTIONS);
  for (;;)
  {
    len = sizeof(cl_addr);
    const int CLIENT_SOCKET = accept(SOCKET, (struct sockaddr *) &cl_addr, (socklen_t*)&len);
  
    if (CLIENT_SOCKET < 0)
    {
      memset(string,0,strlen(string));
      memcpy(string,"Error accepting connection from ",32);
      memcpy(string+32,inet_ntoa(cl_addr.sin_addr),strlen(inet_ntoa(cl_addr.sin_addr)));
      memcpy(string+32+strlen(inet_ntoa(cl_addr.sin_addr))," on ",4);
      memcpy(string+36+strlen(inet_ntoa(cl_addr.sin_addr)),buffer2,strlen(buffer2));
      color_print(string,"red"); 
      continue;
    }
    memset(string,0,strlen(string));
    memcpy(string,"Connection accepted from ",25);
    memcpy(string+25,inet_ntoa(cl_addr.sin_addr),strlen(inet_ntoa(cl_addr.sin_addr)));
    memcpy(string+25+strlen(inet_ntoa(cl_addr.sin_addr))," on ",4);
    memcpy(string+29+strlen(inet_ntoa(cl_addr.sin_addr)),buffer2,strlen(buffer2));
    color_print(string,"green"); 

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

       // close the main socket, since the socket is now copied to the forked process
       close(SOCKET); 

       for (;;)
       {
         const size_t  client_address_length = strlen(client_address);
         const size_t  buffer2_length = strlen(buffer2);
         // receive the data
         memset(buffer, 0, BUFFER_SIZE); 
         receive_data_result = receive_data(CLIENT_SOCKET,buffer,SOCKET_END_STRING,0,TOTAL_CONNECTION_TIME_SETTINGS);
         if (receive_data_result < 2)
         {
           memset(string,0,strlen(string));
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
       
       }
     }
     close(CLIENT_SOCKET);
   }
   return 0; 
}