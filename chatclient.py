#!/usr/bin/python

"""
Author:
Date: 07/22/2015
Class: CS372.400
Assignment: Project 1
This chat client interfaces with the accompanying chat server.
Usage is:
  $ python chatclient.py [hostname] [port number]
Code marked with *** comes from:
https://docs.python.org/2/library/struct.html
Socket programming code comes from:
https://docs.python.org/2/howto/sockets.html
"""

import socket
import sys
from struct import *

#-----------
# Functions
#-----------

# takes a hostname and port number
# opens a socket and connects to the supplied
# host on the supplied port number and
# returns the socket file descriptor
def connect(host, port):
    if port < 1024 or port > 65535:
       print "Ports must be in range 1024 - 65535"
       sys.exit(0)

    # open the socket and connect
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect((host, port))
    except:
        print "Error connecting to " + host + ":" + str(port)
        sys.exit(0)
    return s

# takes a socket file descriptor and outgoing message
# sends the size of the upcoming message over the connection,
# then the message itself
def sendMsg(sockfd, outgoing):
    size = len(outgoing)            # size of the outgoing message
    data = pack('I', size)          # pack the size for transmission ***
    sockfd.send(data)               # send the size of the message
    sockfd.send(outgoing)           # send the message itself

# takes a socket file descriptor
# receives the size of the upcoming message over the connection,
# then receives the message itself. the message is then returned
def recvMsg(sockfd):
    data = sockfd.recv(4)               # get the size of the incoming message
    recvsize = unpack('I', data)        # unpack it ***
    incoming = sockfd.recv(recvsize[0]) # get it out of the python tuple
    return incoming         # return the message received

# takes no parameters
# prompts the user to input a "handle", 
# validates its length and that there are no spaces,
# then returns the handle
def getHandle():
    validHandle = False
    while validHandle == False:
        validHandle = True
        print "Enter your desired HANDLE, one word up to 10 characters\n$",
        handle = raw_input()
        for letter in handle:
            if letter == ' ':
                validHandle = False
                print("Invalid HANDLE: may not contain spaces")
        if len(handle) > 10:
            validHandle = False
            print("Invalid HANDLE: must be 10 characters or less")
    return handle


#--------------
# Main Program
#--------------

# check the cmd line args
if len(sys.argv) != 3:
    print "\nUsage:\n $ python chatclient.py [hostname] [port number]\n\n"
else:
    # connect to the server
    s = connect(sys.argv[1], int(sys.argv[2]))

    # get the user's handle
    handle = getHandle()

    quit = False
    # loop while we don't want to quit
    while quit == False:
        print (handle + "$"),
        rawmessage = raw_input()    # get the message
        if rawmessage == "\quit":   # check for quit command
            quit = True

        if quit == True:        # send quit message to server
            outgoing = "quit"
        else:
            outgoing = (handle + ">" + rawmessage) # else prepend the HANDLE

        # whether quitting or not, send the message
        sendMsg(s, outgoing)

        # only if not quitting, recv the next message
        if quit == False:
            incoming = recvMsg(s)
            if incoming == "quit":
                print("Connection terminated by server; exiting")
                s.close()           # close the connection
                quit = True
            else:
                print incoming          # print the message
        else:
            s.close()