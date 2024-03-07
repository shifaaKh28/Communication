#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include "List.c"

#define BUFFER_SIZE 65536
#define MAX_CLIENTS 1

/*
 * This function closes the listening socket and the client socket.
 * Parameters:
 *   - listeningSocket: The file descriptor of the listening socket to be closed.
 *   - clientSocket: The file descriptor of the client socket to be closed.
 */
void closeSockets(int listening_Socket, int client_Socket) 
{
    // Close the client socket
    close(client_Socket);
    // Close the listening socket
    close(listening_Socket);
}
/*
 * Main function to run the TCP receiver program.
 * 
 * This function sets up a listening socket, binds it to the server address, and listens for incoming TCP connections.
 * Upon accepting a connection, it receives data from the client, calculates statistics for each run, and sends back an exit message.
 * After processing all runs, it prints statistics including the number of runs, individual run data, and average time and speed.
 * 
 * Parameters:
 *   - argc: The number of command-line arguments.
 *   - argv: An array of command-line argument strings.
 * 
 * Returns:
 *   - 0 on successful execution.
 *   - -1 if an error occurs.
 */
int main(int argc, char *argv[]) {

     // Define the exit message to be sent to client
    char *message = "exit\n";

/*
 * Structure representing the server address.
 */
   struct sockaddr_in serverAddr;
   // Initialize the server address structure to zero
   memset(&serverAddr, 0, sizeof(serverAddr));


/*
 * Structure representing the client address.
 */
struct sockaddr_in clientAddr;
// Initialize the length of the client address structure
socklen_t clientAddr_size = sizeof(clientAddr);

      // Allocate memory for the linked list to store dat
    List *list = List_alloc();

    printf("Starting Receiver...\n");
    // Create a socket for listening to incoming connections.
    int listening_Sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_Sock == -1) 
    {
        return -1;
    }

 // Set the SO_REUSEADDR option to allow reusing the address and port
    int reusing = 1;
    if (setsockopt(listening_Sock, SOL_SOCKET, SO_REUSEADDR, &reusing, sizeof(int)) == -1) 
    {
        perror("setsockopt(2)");
        closeSockets(listening_Sock, -1);
        return 1;
    }
   
    // Fill the server address structure.
    serverAddr.sin_family = AF_INET;                // Set address family to IPv4
    serverAddr.sin_port = htons(atoi(argv[2]));     // Set port number from command-line argument
    serverAddr.sin_addr.s_addr = INADDR_ANY;        // Bind to all available network interfaces

  // Bind the server address to the socket.
    if (bind(listening_Sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) 
    {
        perror("bind() failed");
        closeSockets(listening_Sock, -1);
        return -1;
    }
    if (listen(listening_Sock, MAX_CLIENTS) == -1) 
    {
        perror("listen() failed");
        closeSockets(listening_Sock, -1);
        return -1;
    }

    printf("Waiting for TCP-connections...\n");


    while (1)  // Loop to accept multiple connections
    {
        // Accept the connection.
        int clientSocket = accept(listening_Sock, (struct sockaddr *)&clientAddr, &clientAddr_size);

        // Check errors.
        if (clientSocket == -1) 
        {
            closeSockets(listening_Sock, -1);
            return -1;
        }
        printf("Sender connected, beginning to receive file...\n");

        // Receive the data from the client(Sender).
        char buffer[BUFFER_SIZE] = {0};

       // Create a counter to keep track of run number
        int counter = 1;

/*
 * Loop to receive data from the client.
 * This loop continuously receives data from the client until a termination condition is met.
 * It clears the buffer, captures the start time, and then receives data in chunks using the recv function.
 * If it's the first round of receiving data, it captures the start time.
 * If a termination command ("Finish\n" or "Exit\n") is received, the loop exits.
 */
        do
        {
            memset(buffer, 0, sizeof(buffer)); // Clear the buffer
            clock_t start_time, end_time;
  // Variables to  keep track bytes received and other parameters
            int bytes;
            int curr = 1;
            int totalBytes = 0;
            while ((bytes = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) 
            {
                if (curr)
                {
                    start_time = clock();
                }
                  // Check if the "Finish" or "Exit" command is received
                if ((strstr(buffer, "Finish\n") != NULL) || (strstr(buffer, "Exit\n") != NULL))
                {
                    printf("File transfer completed.\n");
                    break;
                }
                // Handle the receives data
                if (bytes < 0) 
                {
                    perror("recv(2)");
                    closeSockets(listening_Sock, clientSocket);
                    return 1;
                } else if (bytes == 0) 
                {
                   // Handle client disconnection
                    closeSockets(listening_Sock, clientSocket);
                    break;
                } else 
                {
                    if (buffer[BUFFER_SIZE - 1] != '\0')
                        buffer[BUFFER_SIZE - 1] = '\0';
                }

                curr = 0;
                // Update total bytes received and clear the buffer
                totalBytes += bytes;
                bzero(buffer, BUFFER_SIZE); 
            }
            end_time = clock();
            printf("the total bytes have been received is: %d\n", totalBytes);

           
            double ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

            List_insertLast(list, counter, ms, bytes / ms);
            fprintf(stdout, "Run #%d Data: Time: %fms Speed: %fMB/s\n", counter, ms, bytes / ms);
            counter++;
        } while (strstr(buffer, "Exit\n") == NULL);

        int bytes_sent = send(clientSocket, message, strlen(message), 0);
        printf("Sender sent exit message\n");// Send an exit message to the client

        if (bytes_sent < 0) // Handle error in sending data
        {
            perror("send(2)");
            closeSockets(listening_Sock, clientSocket);
            return 1;
        }
// Close the sockets after communication with the client is done
        closeSockets(listening_Sock, clientSocket);
        fprintf(stdout, "Client %s:%d disconnected\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        break;
    }

    fprintf(stdout, "finished!\n\n");

// Print statistics
    printf("-----------------------------\n");
    printf("Statistics:\n");
    printf("Algorithm is: %s\n", argv[4]);

    // Traverse the linked list and print data for each run
    Node* node = list->_head; 

    for (int i = 0; i < list->_size-1; i++)
    {
        printf("Run #%d Data: Time: %f ms, Speed: %f MB/s\n", node->run, node->time, node->speed);
        node = node->next;
    }

// Calculate and print average time and bandwidth
    node = list->_head;
    float avgTime = 0.0;
    float avgSpeed = 0.0;

    for (int i = 0; i < list->_size-1; i++)
    {
        avgTime += node->time;
        avgSpeed += node->speed;
        node = node->next;
    }
   float averageTime = avgTime / (list->_size );
float averageBandwidth = avgSpeed / (list->_size);

printf("Average Time: %f ms\n", averageTime);
printf("Average Bandwidth: %f MB/s\n", averageBandwidth);
printf("-----------------------------\n");
printf("Receiver end.\n");

    List_free(list);// Free the memory allocated for the linked list
    return 0;

}
