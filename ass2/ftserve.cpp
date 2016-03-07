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
#include <typeinfo>
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
int createDataSocket(int controlSocket, char* command);
// Sends requested directory listing to client
void listDirectory(int controlSocket, char* command);
// Sends requested file to client
void transferFile(int controlSocket, char* command);
// Utilize the above
void handleRequest(int, int);
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// MAIN --------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    
    // Setup socket
    int port = atoi(argv[1]);
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ipInfo;
    memset(&ipInfo, '0', sizeof(ipInfo));
    ipInfo.sin_family = AF_INET;
    ipInfo.sin_port = htons(port);
    ipInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listeningSocket, (struct sockaddr*)&ipInfo, sizeof(ipInfo));
    listen(listeningSocket, 10);
    char list[] = "-l";
    while(1) {
        struct sockaddr_in ipInfo;
        memset(&ipInfo, '0', sizeof(ipInfo));
        int controlSocket = accept(listeningSocket, (struct sockaddr *) NULL, NULL);
        char command[500];
        memset(&command[0], 0, sizeof(command));
        recv(controlSocket, command, 500, 0);
        
        char str[500];
        strncpy(str, command, 500);
        
        cout << "command1 is " << command << endl;
        char *dirFile;
        cout << "dirFile1 is " << dirFile << endl;
        
        dirFile = strtok(str, " ");
        cout << "dirFile2 is " << dirFile << endl;
        
        // This gives us either -l or -g
        if (strcmp(dirFile,list)==0){
            DIR *directory = opendir(".");
            struct dirent * dEntry;
            int clientDataPort = 0;
            int ftpDataSocket = 0;
            cout << "hi10" << endl;
            cout << "command2 is " << command << endl;
            
            dirFile = strtok(NULL, " ");  // This gives us -dataport
            cout << "dirFile3 is " << dirFile << "...dataport bitches" << endl;
            
            clientDataPort = atoi(dirFile);
            cout << "clientDataPort is " << clientDataPort << endl;
            cout << "clientDataPort type is " << typeid(clientDataPort).name() << endl;
            
            struct sockaddr_in ipInfo2;
            cout << "hi11" << endl;
            
            memset(&ipInfo2, '0', sizeof(ipInfo2));
            cout << "hi12" << endl;
            
            getpeername(controlSocket, (struct sockaddr *) &ipInfo2, (unsigned int*)sizeof(ipInfo2));
            ipInfo2.sin_port = htons(clientDataPort);
            ftpDataSocket = connect(ftpDataSocket, (struct sockaddr *) &ipInfo2, sizeof(ipInfo2));
            while ((dEntry = readdir(directory))){
                cout << "directory entry is " << dEntry->d_name << endl;
                send(ftpDataSocket, dEntry->d_name, strlen(dEntry->d_name), 0);
                send(ftpDataSocket, "\n", 1, 0);
            }
            closedir(directory);
            
        }
        cout << "hiiiiiiii" << endl;
        
        transferFile(controlSocket, command);
    }
    
    return 0;
};



void transferFile(int controlSocket, char* command){
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
    //    ftpDataSocket = createDataSocket(controlSocket, clientDataPort);
    
    
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