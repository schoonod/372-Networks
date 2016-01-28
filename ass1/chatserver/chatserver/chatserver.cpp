//
//  main.cpp
//  chatserve
//
//  Created by Dane Schoonover on 1/27/16.
//  Copyright © 2016 dane. All rights reserved.
//

//
//  main.cpp
//  chatserve
//
//  Created by Dane Schoonover on 1/27/16.
//  Copyright © 2016 dane. All rights reserved.

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int checkArg(int, char **);
void createSocket(int);
void bindSocket();
void listenSocket();
void acceptConn();
void receiveMessage(int);
void sendMessage(int);
void clearMessages();


struct sockaddr_in servaddr;
int serverSocket, connfd;
int closeFlag = 0;
char servName[11] = "ServerSays";
char recMessage[501];
char message[501];

/*
 MAIN FUNCTION
 This function initializes most actions. It carries out the setup
 of the socket, listening, etc.
 */
int main(int argc, char ** argv){
    int portNum;
    
    // if ((commence = checkArg(argc, argv)) == 1)
    //     return 1;

    portNum = atoi(argv[1]);
    createSocket(portNum);
    bindSocket();
    listenSocket();
    acceptConn();
    
    return 0;
}

/*
 CHECK ARGUMENTS
 This function checks that there is one argument, the port number.
 */
// int checkArg(int argc, char** argv){
//     if (argc != 2){
//         cerr << "This program requires 1 argument, the port number." << endl;
//         return 1;
//     }
//     return 0;
// }

/*
 CREATE SOCKET
 This function does socket initialization.
 */
void createSocket(int portNum){
    
    // create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    
    // memset(&servaddr, '0', sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNum);
}

/*
 BIND SOCKET
 This binds the socket.
 */
void bindSocket(){
    bind(serverSocket, (struct sockaddr*)&servaddr, sizeof(servaddr));
}

/*
 LISTEN SOCKET
 This listens in.
 */
void listenSocket(){
    listen(serverSocket, 10);
}

/*
 ACCEPT CONNECTION
 This function accepts connections and allows for communication.
 */
void acceptConn(){
    while (1){
        closeFlag = 0;
        connfd = accept(serverSocket, (struct sockaddr*)NULL, NULL);
        cout << "Client has connected. Awaiting message... " << endl;
        send(connfd, servName, strlen(servName), 0);
        
        while (closeFlag == 0){
            clearMessages();
            
            receiveMessage(connfd);
            
            sendMessage(connfd);
            
            
            if (closeFlag == 1){
                close(connfd);
                exit(1);
            }
        } 
    }
}

/*
 RECEIVE MESSAGE
 This function allows the server to receive messages from the client.
 */
void receiveMessage(int connfd){
    recv(connfd, recMessage, 500, 0);
    if (strcmp(recMessage, "\\quit") == 0){
        cout << "Client has disconnected." << endl;
        closeFlag = 1;
    }
}

/*
 SEND MESSAGE
 This function allows the server to send a message to the client.
 */
void sendMessage(int connfd){
    if (closeFlag == 0){
        cout << "ClientSays> " << recMessage << endl;
        cout << servName << "> ";
        cin.getline(message, 81);
        
    }
    if (strcmp(message, "\\quit") == 0){
        cout << "You have chosen to close this connection..." << endl;
        closeFlag = 1;
    }
    
    send(connfd, message, strlen(message), 0);   
    
}

/*
 CLEAR MESSAGES
 This clears the memory of the sending and receiving message.
 */
void clearMessages(){
    memset(&message[0], 0, sizeof(message));
    memset(&recMessage[0], 0, sizeof(recMessage));
}