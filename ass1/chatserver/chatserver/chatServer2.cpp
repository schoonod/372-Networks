#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;



//----------------------------------------------
// Prototypes
//----------------------------------------------


// Returns the handle for the server
// string setHandle();

// Returns serverSocket; serverSocket = socket(AF_INET,SOCK_STREAM)
int setSocket();

// Takes socket and port
void bindSocket(int socket, int port);             

// Socket is listening; serverSocket.listen(1)
// Takes serveSocket
void listenSocket(int);

// Accepts a connection and create the new chat socket
// Takes serverSocket and serverHandle
// Returns a chatSocket
int acceptConnection(int);

// Receives a message from the client
// Takes a chatSocket
void receiveMessage(int);

// Send a message to the client
// Takes a chatSocket and serverHandle
void sendMessage(int);


char serverHandle[7] = "Server";
//----------------------------------------------
// Main
//----------------------------------------------


int main(int argc, char ** argv){
    
    int port = atoi(argv[1]);
    // string serverHandle = setHandle();

    
    int serverSocket = setSocket();
    
    bindSocket(serverSocket, port);
    
    listenSocket(serverSocket);
    
    int chatSocket = acceptConnection(serverSocket);    
    
    // Loop forever
    while(1){
        receiveMessage(chatSocket);

        if (!chatSocket)
            break;

        sendMessage(chatSocket);
    }

    return 0;
}



//----------------------------------------------
// Functions
//----------------------------------------------

// Get Handle
// string setHandle(){
//     string handle = "Server";
//     return handle;
// }


// Create Socket
int setSocket(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    return serverSocket;
}

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

int acceptConnection(int socket){
    // New connection socket does not need/use the peer socket so NULL, NULL
    // is used for addr and sizeof(addr)
    int newConnectionSocket = accept(socket, (struct sockaddr*)NULL, NULL);
    cout << "Client connected, awaiting message." << endl;
    send(newConnectionSocket, serverHandle, strlen(serverHandle),0);
    return newConnectionSocket;
};


// Receive/Send messages
void receiveMessage(int chatSocket){
    char newMessage[500];
    memset(&newMessage[0], 0, sizeof(newMessage));
    recv(chatSocket, newMessage, 500, 0);
    if(strcmp(newMessage, "\\quit") == 0){
        cout << "Client disconnected" << endl;
        close(chatSocket);
     }

    else 
        cout << newMessage << endl;

};

void sendMessage(int chatSocket){
    char newMessage[500];
    memset(&newMessage[0], 0, sizeof(newMessage));
    cin.getline(newMessage, 500);
    
    if(strcmp(newMessage, "\\quit") == 0){
        cout << "Server disconnected" << endl;
        close(chatSocket);
    }
    
    else {
        send(chatSocket, newMessage, strlen(newMessage), 0);

    }

};














