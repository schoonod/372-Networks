"""
Name: Kabir Kang
Course: CS 372
Date: 11/26/14
Project: FTP via TCP Socketing: Project 2
Description: This program acts as the client sending FTP requests, delivering files, or ls lists
Use: I did not have time to implement getopt and since it was not expected in the grading criteria, adhere to the following usage
python ftclient.py <host(127.0.0.1)> <control port> <data port(control port+1 to make it easier)> <get or list> [filename]
"""
import getopt
import os
import sys
import signal
import socket

"""
Description:
The main function. It determines whether the user wants to list or get a file and makes that happen.
"""
def client():
    if len(sys.argv) < 5:
        print "Use: python ftclient.py 127.0.0.1 <controlPort> <dataPort> <get/list> [file]"
        sys.exit()
    if (int(sys.argv[3]) - int(sys.argv[2])) != 1:
        print "For convenience in testing, I ask that dataport be 1 more than control port. I.E. 30000 and 30001.\n Use: python ftclient.py 127.0.0.1 <controlPort> <dataPort> <get/\list> [file]"
        sys.exit()

    if sys.argv[4] == "get":
        #print "you want to get?"
        print "The client intends to GET"
        commandFlag = 1
        cmd = sys.argv[5] 
    elif sys.argv[4] == "list":
        #print "a list eh??"
        print "The client requests a LIST"
        commandFlag = 2
        cmd = "list"
    else:
        cmd = -1 # The error case
        sys.exit()
    sock = initiateContact()
    print "Initiating contact..."
    makeRequest(sock, cmd, commandFlag)
    print "Making request..."
    datasock = dataSocket()
    print "Data transfer complete..."
    sock.close()
    datasock.close()
        
"""
Description: Creates a control socket.
"""
def initiateContact():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((sys.argv[1],int(sys.argv[2])))
    return sock

"""
Description: Makes a request for a list or a file.
"""
def makeRequest(s,c, commandFlag):
    #print commandFlag
    if commandFlag == 2:
        s.send("list")
    elif commandFlag == 1:
        s.send(sys.argv[5])

"""
ALSO KNOWN AS receiveFile
Description: Takes in information from the server.
"""
def dataSocket():
    datasock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    datasock.bind((sys.argv[1],int(sys.argv[3])))
    datasock.listen(10)
    while 1:
        connectionSocket, addr = datasock.accept()
        #print "DATA"
        print "Data socket established"
        dataBuf = connectionSocket.recv(2999)
        print dataBuf
        break    
    if sys.argv[4] == "get":
        with open("file.txt",'w') as out:
            out.write(dataBuf)
    return datasock        

# Summary: signal handler
def handleSigs(signum, frame):
    sys.exit()


# This function sets up signal handlers.
def signalfy():
    signal.signal(signal.SIGQUIT, handleSigs)
    signal.signal(signal.SIGHUP, handleSigs)
    signal.signal(signal.SIGINT, handleSigs)

"""
This simply gets the client function running
"""
if __name__ == '__main__':
    client()