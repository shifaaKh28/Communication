
## Overview

This part of the assignment involves implementing basic networking functionality using TCP. The key tasks include creating a linked list implementation, a TCP sender, and a TCP receiver. The sender generates and sends data, while the receiver captures and stores the data in a file.

## Files in This Part

1. **List.c**: 
   - Implements a simple linked list that can be used to store data.
   - Provides basic operations like insertion, deletion, and traversal.

2. **List.h**: 
   - The header file for `List.c`. 
   - Declares the functions and structures used in the linked list implementation.

3. **Makefile**: 
   - Contains the compilation instructions for building the project.
   - Compiles the TCP sender, receiver, and linked list into executable files.

4. **TCP_Sender.c**: 
   - A program that sends data over a TCP connection to a receiver.
   - It generates random data and transmits it to a specified IP address and port.

5. **TCP_Receiver.c**: 
   - A program that receives data sent by the TCP sender.
   - It captures the incoming data and saves it to a file for further analysis.

## Compilation and Execution

### Compiling the Programs

To compile the programs, run the following command in the terminal:

```bash
make
```

This will generate the `TCP_Sender`, `TCP_Receiver`, and any other necessary executables.

### Running the TCP Sender

To run the TCP sender, use the following command:

```bash
./TCP_Sender -ip <receiver_ip> -p <port>
```

Replace `<receiver_ip>` with the IP address of the machine running the receiver and `<port>` with the port number on which the receiver is listening.

### Running the TCP Receiver

To run the TCP receiver, use the following command:

```bash
./TCP_Receiver -p <port>
```

Replace `<port>` with the port number on which you want to listen for incoming connections.

## How It Works

- **TCP_Sender**: Generates a specified amount of random data and sends it to the TCP_Receiver. The sender waits for an acknowledgment from the receiver after each data transfer. It provides options to send the data multiple times based on user input.
  
- **TCP_Receiver**: Listens on a specified port for incoming connections. Upon receiving data, it writes the data to a file and sends an acknowledgment back to the sender.

This README provides an overview of the work done in Part 1 of the assignment, explaining the purpose of each file and how to compile and run the programs.
