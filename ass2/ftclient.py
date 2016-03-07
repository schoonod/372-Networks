# Dane Schoonover
# CS372
# Project 2
# Description:

import getopt
import os


# For sys.argv (command line input)
import sys	

# Socket api
from socket import * 

# Validate params
def validateParameters(params):
	if len(params) < 4:
		print "Input correct number of arguments"
		exit(1)

	if len(params) > 5:
		print "Input correct number of arguments"
		exit(1)

	# if len(params) = 4:

	# if len(params) = 5:

# Start ftclient by specifying hostname/port
def controlConstructor(serverName, serverPort):
	controlSocket = socket(AF_INET, SOCK_STREAM)
	controlSocket.connect((serverName, serverPort))
	return controlSocket

def dataConstructor(hostName, hostPort):
	dataSocket = socket(AF_INET, SOCK_STREAM)
	dataSocket.bind((hostName, hostPort))
	dataSocket.listen(1)
	return dataSocket

def printDirectory(dataSocket):
	print "waiting1?"
	conn, addr = dataSocket.accept()
	directory = conn.recv(2048)
	print directory

def receiveFile(dataSocket, fileName):
	conn, addr = dataSocket.accept()
	fileData = conn.recv(2048)
	fileName = open(fileName, 'w')
	fileName.write(fileData)
	
def dirRequest(controlSocket, dataSocket, command, dataPort):
	controlSocket.send(("%s %s") % (command, dataPort))
	printDirectory(dataSocket)

# Make request to host for file directory or file
def fileRequest(controlSocket, dataSocket, command, fileName, dataPort):
	controlSocket.send(("%s %s %s") % (command, fileName, dataPort))
	receiveFile(dataSocket, fileName)

	
if __name__ == '__main__':
	validateParameters(sys.argv[1:])

	serverName = sys.argv[1]

	serverPort = sys.argv[2]
	serverPort = int(serverPort)	

	command = sys.argv[3]

	controlSocket = controlConstructor(serverName, serverPort)

	if len(sys.argv) == 5:
		dataPort = int(sys.argv[4])
		dataSocket = dataConstructor('localhost', dataPort)
		dirRequest(controlSocket, dataSocket, command, dataPort)

	else:
		fileName = sys.argv[4]
		dataPort = int(sys.argv[5])
		dataSocket = dataConstructor('localhost', dataPort)
		fileRequest(controlSocket, dataSocket, command, fileName, dataPort)

	dataSocket.close()





