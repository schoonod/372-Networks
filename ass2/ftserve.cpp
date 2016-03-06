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

//--------------------------------------------------------------------------------------
// LISTENING SOCKET PROTOTYPES
//--------------------------------------------------------------------------------------
// Returns listeningSocket; listeningSocket = socket(AF_INET,SOCK_STREAM)
int setSocket();
// Takes socket and port
void bindSocket(int socket, int port);
// Socket is listening; listeningSocket.listen(1)
// Takes serveSocket
void listenSocket(int);
// Send a message to the client
// Takes a chatSocket and serverHandle
void sendMessage(int);
// Utilize the above
int startup(int);
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// INCOMING CONNECTION HANDLER PROTOTYPES ----------------------------------------------
//--------------------------------------------------------------------------------------
// Accept incoming connection and returns a new socket
int acceptConnection(int);
// Receives a message from the client
// Takes a control socket
char* receiveCommand(int);
// Get command for listing director OR sending file
char* dirFileCommand(char* command);
// Return ftpDataSocket
int createDataSocket(int ftpControlSocket, char* command);
// Sends requested directory listing to client
void listDirectory(int ftpControlSocket, char* command);
// Sends requested file to client
void transferFile(int ftpControlSocket, char* command);
// Utilize the above
void handleRequest(int, int);
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// MAIN --------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {

    // Setup socket
    int port = atoi(argv[1]);
    cout << "Port number " << port <<  " received." << endl;
    
    int listeningSocket = startup(port);
    cout << "Server started" << endl;
    
    // Accept incoming connections and handle requests
    handleRequest(listeningSocket, port);
    
    return 0;
};
// -------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// MAIN FUNCTION DEFINITIONS -----------------------------------------------------------
//--------------------------------------------------------------------------------------
int startup(int port){
    // Create a listening Socket
    int listeningSocket = setSocket();
    bindSocket(listeningSocket, port);
    listenSocket(listeningSocket);
    return listeningSocket;
};

void handleRequest(int listeningSocket, int port){
    char list[] = "-l";
    while(1) {
        
        // Create a socket with each client
        int ftpControlSocket = acceptConnection(listeningSocket);
        cout << "Connection accepted success." << endl;
        
        // Receive full command string from client
        char* command = receiveCommand(ftpControlSocket);
        
        char* dirFile = dirFileCommand(command);
        
        // Send data based on command
        if (strcmp(dirFile,list)==0)
            listDirectory(ftpControlSocket, command);
        else
            transferFile(ftpControlSocket, command);
        
        free((char*)command);
    }
};
//--------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
// INCOMING CONNECTION HANDLER ---------------------------------------------------------
//--------------------------------------------------------------------------------------
int acceptConnection(int listeningSocket){
    // New connection socket does not need/use the peer address info so NULL, NULL
    // is used for addr and sizeof(addr)
    int newConnectionSocket = accept(listeningSocket, (struct sockaddr*)NULL, NULL);
    cout << "Client connected, awaiting command." << endl;
    return newConnectionSocket;
};

char* receiveCommand(int listeningSocket){
    char* newMessage = (char*)malloc(500);
    memset(&newMessage[0], 0, sizeof(newMessage));
    recv(listeningSocket, newMessage, 500, 0);
    return newMessage;
};

char* dirFileCommand(char* command){
    char* str = command;
    char* dirFile;
    
    // This gives us either -l or -g
    dirFile = strtok(str, " ");
 
    return dirFile;
};

int createDataSocket(int ftpControlSocket, int ftpDataPort){
    // New data socket for FTP
    int ftpDataSocket = 0;
    

    // Assign client IP info to Socket
    struct sockaddr_in ipInfo;
    memset(&ipInfo, '0', sizeof(ipInfo));
    getpeername(ftpControlSocket, (struct sockaddr *) &ipInfo, (unsigned int*)sizeof(ipInfo));
    
    // Assign FTP data port to Socket
    ipInfo.sin_port = htons(ftpDataPort);
    
    // Connect data Socket to client
    connect(ftpDataSocket, (struct sockaddr *) &ipInfo, sizeof(ipInfo));
    
    // Return connected Socket
    return ftpDataSocket;
};


void listDirectory(int ftpControlSocket, char* command){
    // http://stackoverflow.com/questions/30516546/how-are-dirent-entries-ordered
    DIR *directory = opendir(".");
    struct dirent * dEntry;
    int clientDataPort = 0;
    int ftpDataSocket = 0;
    
    
    char *dataport;
    
    dataport = strtok(command, " ");  // This gives us -l
    dataport = strtok(NULL, " ");     // This gives us the dataport
    
    clientDataPort = atoi(dataport);

    // Returns clientDataPort
    ftpDataSocket = createDataSocket(ftpControlSocket, clientDataPort);

    while ((dEntry = readdir(directory))){
        // Send a filename
        send(ftpDataSocket, dEntry->d_name, strlen(dEntry->d_name), 0);
        // Send a new line
        send(ftpDataSocket, "\n", 1, 0);
    }
    
    closedir(directory);
           
};

void transferFile(int ftpControlSocket, char* command){
    int clientDataPort = 0;
    int ftpDataSocket = 0;
    
    // Commands incoming from pythong client are: -g <filename.txt> <datasocket>
    char *str = command;
    char *temp;
    char *filename;   // filenamec
    char *dataport;   // dataport
    
    
    // Find the filename
    temp = strtok(str, " ");        // this gives us tokens, and also 'g'
    
    temp = strtok(NULL, " ");       // this gives us 'filename.txt'
    filename = temp;
    
    temp = strtok(NULL, " ");       // this gives us 'dataport'
    dataport = temp;
    
    clientDataPort = atoi(dataport);
    
    // Returns clientDataPort
    ftpDataSocket = createDataSocket(ftpControlSocket, clientDataPort);
    
    
    // TRANSFER FILE
    // http://www.cplusplus.com/reference/cstdio/fread/ this link tells us
    // to do magical things with c++ to read a file
    
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    
    pFile = fopen (filename , "rb");
    if (pFile==NULL) {
        fputs ("File error",stderr);
        exit (1);
    }
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {
        fputs ("Memory error",stderr);
        exit (2);
    }
    
    // copy the file into the buffer:
    result = fread(buffer,1,lSize,pFile);
    if (result != lSize) {
        fputs ("Reading error",stderr);
        exit (3);
    }
    
    // The whole file is now loaded in the memory buffer
    // Send the file
    send(ftpDataSocket, buffer, lSize, 0);
    
    
    // terminate
    fclose (pFile);
    free (buffer);
    close(ftpDataSocket);
};
// -------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
// OPEN SOCKET -------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Create Socket
int setSocket(){
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    return listeningSocket;
};

// Bind server address info to socket
void bindSocket(int socket, int port){
    struct sockaddr_in ipInfo;
    memset(&ipInfo, '0', sizeof(ipInfo));
    ipInfo.sin_family = AF_INET;
    ipInfo.sin_port = htons(port);
    ipInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(socket, (struct sockaddr*)&ipInfo, sizeof(ipInfo));
};

// Listen and open a chat socket
void listenSocket(int socket){
    listen(socket, 1);
    cout << "Server is listening" << endl;
};
// -------------------------------------------------------------------------------------