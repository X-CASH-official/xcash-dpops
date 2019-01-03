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

#include "xcash_proof_of_stake_functions_and_macros.h"
#include "xcash_proof_of_stake_test.h"

/*
-----------------------------------------------------------------------------------------------------------
Main function
-----------------------------------------------------------------------------------------------------------
*/

int main(int parameters_count, char* parameters[])
{
  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strcmp(parameters[1],"--test") == 0)
    {
      test();
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    exit(0);
  }

  create_server();
  /*// Constants
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

  // check if the program needs to run the test
  if (parameters_count == 2)
  {
    if (strcmp(parameters[1],"--test") == 0)
    {
      test();
    }
    else
    {
      color_print("Invalid parameters\n","red");
      printf(INVALID_PARAMETERS_ERROR_MESSAGE);
    }
    exit(0);
  }

   Create the socket  
  AF_INET = IPV4 support
  SOCK_STREAM = TCP protocol
  
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

   Set the socket options
  SOL_SOCKET = socket level
  SO_REUSEADDR = allows for reuse of the same address, so one can shutdown and restart the program without errros
  SO_REUSEPORT = allows for reuse of the same port, so one can shutdown and restart the program without errros
  
  if (setsockopt(SOCKET, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SOCKET_OPTION,sizeof(int)) < 0)
  {
    color_print("Error setting socket options","red"); 
    return 0;
  } 
  color_print("Socket created","green");
 
  // convert the port to a string
  sprintf(buffer2,"%d",SEND_DATA_PORT);  
 
  memset(&addr, 0, sizeof(addr));
   setup the connection
  AF_INET = IPV4
  INADDR_ANY = connect to 0.0.0.0
  use htons to convert the port from host byte order to network byte order short
  
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
         const size_t client_address_length = strlen(client_address);
         const size_t buffer2_length = strlen(buffer2);
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

          printf("Received %s from %s on port %s\r\n",buffer,client_address,buffer2);
          // send the message 
          if (send_data(CLIENT_SOCKET,buffer,1) == 1)
          {
            printf("Sent %s to %s on port %s\r\n",buffer,client_address,buffer2);
          } 
          else
          {
            memset(string,0,strlen(string));
            memcpy(string,"Error sending data to ",22);
            memcpy(string+22,client_address,client_address_length);
            memcpy(string+22+client_address_length," on port ",9);
            memcpy(string+31+client_address_length,buffer2,buffer2_length);
            color_print(string,"red"); 
            continue;
          } 
       
       }
     }
     close(CLIENT_SOCKET);
   }*/
   return 0; 
}