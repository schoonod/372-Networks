//
//  main.cpp
//  ftserve
//
//  Created by Dane Schoonover on 3/2/16.
//  Copyright © 2016 dane. All rights reserved.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
using namespace std;

//----------------------------------------------
// LISTENING SOCKET PROTOTYPES
//----------------------------------------------
// Returns serverSocket; serverSocket = socket(AF_INET,SOCK_STREAM)
int setSocket();

// Takes socket and port
void bindSocket(int socket, int port);

// Socket is listening; serverSocket.listen(1)
// Takes serveSocket
void listenSocket(int);


// Send a message to the client
// Takes a chatSocket and serverHandle
void sendMessage(int);

// Utilize the above
int startup(int);


//----------------------------------------------
// INCOMING CONNECTION HANDLER PROTOTYPES
//----------------------------------------------
// Accept incoming connection and returns a new socket
int acceptConnection(int);

// Receives a message from the client
// Takes a chatSocket
char* receiveCommand(int);

int createDataSocket(int, int);

// Sends requested directory listing to client
void listDirectory(int dataSocket);

// Sends requested file to client
void transferFile(int dataSocket, char*);

// Utilize the above
void handleRequest(int, int);





// MAIN --------------------------------------------------------------------------------

int main(int argc, const char * argv[]) {

    // Setup socket
    int port = atoi(argv[1]);
    int serverSocket = startup(port);
    cout << "Server started" << endl;
    
    // Accept incoming connections and handle requests
    handleRequest(serverSocket, port);
    
    return 0;
}
// -------------------------------------------------------------------------------------


// MAIN FUNCTIONS ----------------------------------------------------------------------

int startup(int port){
    // Create a listening Socket
    int serverSocket = setSocket();
    bindSocket(serverSocket, port);
    listenSocket(serverSocket);
    return serverSocket;
}

void handleRequest(int serverSocket, int port){
    while(1) {
        int dataSocket = 0;
        
        // Create a socket with each client
        int uniqueClient = acceptConnection(serverSocket);
        
        // Receive commands from the client
        char* command = receiveCommand(uniqueClient);
        
        // Create data socket for pass data to client (if command is sent)
//        if (command)
        dataSocket = createDataSocket(uniqueClient, port);
        
        // Send data based on command
        if (strcmp(command,"-l")==0)
            listDirectory(dataSocket);
        else
            transferFile(dataSocket, command);
        
        free((char*)command);
    }
}
// -------------------------------------------------------------------------------------



// INCOMING CONNECTION HANDLER ---------------------------------------------------------
int acceptConnection(int socket){
    // New connection socket does not need/use the peer address info so NULL, NULL
    // is used for addr and sizeof(addr)
    int newConnectionSocket = accept(socket, (struct sockaddr*)NULL, NULL);
    cout << "Client connected, awaiting command." << endl;
    return newConnectionSocket;
};

char* receiveCommand(int serverSocket){
    char* newMessage = (char*)malloc(500);
    memset(&newMessage[0], 0, sizeof(newMessage));
    recv(serverSocket, newMessage, 500, 0);
    return newMessage;
};

int createDataSocket(int uniqueClient, int port){
    // New data socket for FTP
    int dataSocket = 0;
    
    // Dataport
    int dataport = port - 1;
    
    // Assign client IP info to Socket
    struct sockaddr_in ipInfo;
    memset(&ipInfo, '0', sizeof(ipInfo));
    getpeername(uniqueClient, (struct sockaddr *) &ipInfo, (unsigned int*)sizeof(ipInfo));
    
    // Assign FTP data port to Socket
    ipInfo.sin_port = htons(dataport);
    
    // Connect data Socket to client
    connect(dataSocket, (struct sockaddr *) &ipInfo, sizeof(ipInfo));
    
    // Return connected Socket
    return dataSocket;
};

void listDirectory(int dataSocket){
    // http://stackoverflow.com/questions/30516546/how-are-dirent-entries-ordered
    DIR *directory = opendir(".");
    struct dirent * dEntry;
    
    while ((dEntry = readdir(directory))){
        
        // Send a file
        send(dataSocket, dEntry->d_name, strlen(dEntry->d_name), 0);
        
        // Send a new line
        send(dataSocket, "\n", 1, 0);
    }
    
    closedir(directory);
           
};

void transferFile(int dataSocket, char* command){
    
    char *temp = command;
//    char *temp2;
    char *filename;
    
    // Find the filename
    temp = strchr(temp, ' ');
    filename = temp++;
    
    
    
    
};

// -------------------------------------------------------------------------------------






// OPEN SOCKET -------------------------------------------------------------------------
// Create Socket
int setSocket(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    return serverSocket;
}

// Bind server address info to socket
void bindSocket(int socket, int port){
    struct sockaddr_in ipInfo;
    memset(&ipInfo, '0', sizeof(ipInfo));
    ipInfo.sin_family = AF_INET;
    ipInfo.sin_port = htons(port);
    ipInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(socket, (struct sockaddr*)&ipInfo, sizeof(ipInfo));
}

// Listen and open a chat socket
void listenSocket(int socket){
    listen(socket, 1);
    cout << "Server is listening" << endl;
};
// -------------------------------------------------------------------------------------
