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
// Get port input by use upon program start
int getPort(char ** argv);

// Returns the handle for the server
string setHandle();

// Returns serverSocket; serverSocket = socket(AF_INET,SOCK_STREAM)
int setSocket();

// Set socket address info
// Takes port
// Returns address info
struct sockaddr_in getIpInfo(int port);             // function SETIPINFO of struct type SOCKADDR_IN of data type STRUCT

// Socket is bound to a local address; serverSocket.bind((‘’,serverPort))
// Takes serverSocket, ip info, size of ip info
void bindSocket(int, struct sockaddr_in*, int);

// Socket is listening; serverSocket.listen(1)
// Takes serveSocket
void listenSocket(int);

// Accepts a connection and create the new chat socket
// Takes serverSocket and serverHandle
// Returns a chatSocket
int acceptConnection(int, string);

// Receives a message from the client
// Takes a chatSocket
// Returns true if message received, returns false if client quit
int receiveMessage(int);

// Send a message to the client
// Takes a chatSocket and serverHandle
// Returns true if server sends message, returns false if server quits
int sendMessage(int, string);



//----------------------------------------------
// Main
//----------------------------------------------
int main(int argc, char ** argv){
    
    int port = getPort(argv);
    string serverHandle = setHandle();
    
    int serverSocket = setSocket();
    struct sockaddr_in ipInfo = getIpInfo(port);    // instance IPINFO of struct type SOCKADDR_IN of data type STRUCT
    
    bindSocket(serverSocket, &ipInfo, sizeof(ipInfo));
    
    listenSocket(serverSocket);
    
    // Loop forever
    while(1){
        int chatSocket = acceptConnection(serverSocket, serverHandle);
        
        if(!receiveMessage(chatSocket))
            close(chatSocket);
        if(!sendMessage(chatSocket, serverHandle))
            close(chatSocket);
    }
    
    return 0;
}



//----------------------------------------------
// Functions
//----------------------------------------------

// Get port/Set Handle
int getPort(char ** argv){
    int port = atoi(argv[1]);
    return port;
}


string setHandle(){
    string handle = "Server";
    return handle;
}


// Create Socket
int setSocket(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    return serverSocket;
}


struct sockaddr_in getIpInfo(int port){
    struct sockaddr_in ipInfo;
    ipInfo.sin_family = AF_INET;
    ipInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    ipInfo.sin_port = htons(port);
    
    return ipInfo;
}

// MIGHT HAVE SOME ISSUES WITH THE STRUCT POINT HERE
void bindSocket(int socket, struct sockaddr_in* &ipInfo, int size){
    bind(socket, (struct sockaddr_in*)&ipInfo, size);

}

// Listen and open a chat socket
void listenSocket(int socket){
    listen(socket, 1);
    cout << "Server is listening" << endl;
};

int acceptConnection(int socket, string serverHandle){
    // New connection socket does not need/use the peer socket so NULL, NULL
    // is used for addr and sizeof(addr)
    int newConnectionSocket = accept(socket, NULL, NULL);
    cout << "Client connected" << endl;
    send(newConnectionSocket, &serverHandle, serverHandle.length(),0);
    return newConnectionSocket;
};


// Receive/Send messages
int receiveMessage(int chatSocket){
    string newMessage;
    recv(chatSocket, &newMessage, 500, 0);
    if(newMessage.compare("\\quit") == 0){
        cout << "Client disconnected" << endl;
        return 0;
    }
    else {
        cout << newMessage << endl;
        return 1;
    }
};

int sendMessage(int chatSocket, string serverHandle){
    string newMessage;
    
    getline(cin, newMessage);
    
    if(newMessage.compare("\\quit") == 0){
        cout << "Server disconnected" << endl;
        return 0;
    }
    
    else {
        cout << serverHandle << "> " << newMessage << endl;
        return 1;
    }
};












