import getopt
import os
import sys	# for sys.argv (command line input)

from socket import * 

def clientStart():
	# start chatclient by specifying hostname/port
	# sys.argv[1] and [2] are serverName and serverPort
	# clientSocket is our socket variable name
	clientSocket = socket(AF_INET, SOCK_STREAM)
	clientSocket.connect((sys.argv[1], int(sys.argv[2])))

	# Receive server handle upon connection
	serverName = clientSocket.recv(4096)

	return (clientSocket, serverName)

# Gets username and provides input validation for length
# Returns username
def getUserHandle():
	# flag to break the while loop
	validHandle = False

	while not validHandle:
		userHandle = raw_input('Enter a username 10 characters or less: ')
		if not userHandle:	
			print 'You entered nothing; try again.'

		elif len(userHandle) > 10:
			print 'Your name is too long; try again.'
		
		# User handle is valild
		else:	
			'Thanks ' + userHandle + '!'
			validHandle = True

	return userHandle

# Sequentially talks with server and quits program when instructed
def chatting(clientSocket, serverName, userHandle):
	quit = False

	# May not need to use quit in order to close program
	while not quit:
	
		# read input and send
		clientMessage = raw_input(userHandle + '> ')
		if clientMessage == '\quit':
			quit = True
			print "Bye bye"
			clientSocket.close()
			sys.exit()
		else:
			clientSocket.send(clientMessage)
			sys.stdout.write(serverName + '> ')

		# wait for server message
		serverMessage = str(clientSocket.recv(1024))
		print serverMessage

		# while loop resets to send another message to the server


if __name__ == '__main__':
	(clientSocket, serverName) = clientStart()
	userHandle = getUserHandle()
	chatting(clientSocket, serverName, userHandle)

























