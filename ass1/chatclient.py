# 
# C372
# kangk@onid.oregonstate.edu
# 10/28/14
# Assignment 1
# File: chatclient.py
# Summary: Chats with chatserve.cpp. First it creates a connection with listening server, and then it can begin interchange. Client messages first, then server.

# General imports
import getopt
import os
import sys
# import signal

#Program specific imports
from socket import * 

def client():
    # checks if valid number of args were entered
    checkArgs();
    
    # create the TCP socket for sever 
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect((sys.argv[1],int(sys.argv[2])))  

    name = sock.recv(4096)    
    var = 1
    while var == 1:
        # Receive and send prompt
        msgOut(sock)

        message = msgIn(sock)
        if message == "\quit":
            print "The server has quit communication.\n"
            sys.exit()
        else:
            print name + "> " + message
    sock.close()
# This function receives the name of the server.                
def getName(s):
    name = s.recv(4096)
    return name


def checkArgs():
    if len(sys.argv) != 3:
        print "You must use the following syntax: python chatclient.py <host> <port>"
        sys.exit()

# Summary: Returns messages
def msgIn(s):
    message = str(s.recv(4096)) #recommended buff size
    return message
# Summary: This function sends messages.
def msgOut(s):
    sendMsg = raw_input("Client> ")
    s.send(sendMsg)
    if sendMsg == "\quit":
        s.close
        sys.exit()


if __name__ == '__main__':
    client()