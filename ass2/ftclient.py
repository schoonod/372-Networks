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
def clientStart(serverName, serverPort):
	controlSocket = socket(AF_INET, SOCK_STREAM)
	controlSocket.connect((serverName, int(serverPort)))
	return (controlSocket)

def printDirectory(controlSocket):
	dataSocket, address = controlSocket.accept()
	directory = dataSocket.recv(2048)
	print directory
	dataSocket.close()

def receiveFile(controlSocket, fileName):
	dataSocket, address = controlSocket.accept()
	fileData = dataSocket.recv(2048)
	file = open(fileName, 'w')
	file.write(fileData)
	dataSocket.close()
	
def dirRequest(controlSocket, command, dataPort):
	controlSocket.send(("%s %s") % (command, dataPort))
	printDirectory(controlSocket)

# Make request to host for file directory or file
def fileRequest(controlSocket, command, fileName, dataPort):
	controlSocket.send(("%s %s %s") % (command, fileName, dataPort))
	receiveFile(controlSocket, fileName)

	
if __name__ == '__main__':

	print len(sys.argv)
	print sys.argv[0]
	validateParameters(sys.argv[1:])

	serverName = sys.argv[1]
	print "Servername is " + serverName

	serverPort = sys.argv[2]
	print "Serverport is " + serverPort

	controlSocket = clientStart(serverName, serverPort)


	command = sys.argv[3]
	
	if len(sys.argv) == 5:
		dataPort = sys.argv[4]
		controlSocket.bind(('localhost', dataPort))
		controlSocket.listen(0)
		dirRequest(controlSocket, command, dataPort)

	else:
		fileName = sys.argv[4]
		dataPort = sys.argv[5]
		controlSocket.bind(('localhost', dataPort))
		controlSocket.listen(0)
		fileRequest(controlSocket, command, fileName, dataPort)

	controlSocket.close()









