# chatclient starts on host B, specifying host A’s hostname and port number on the command line.

# chatclient on host B gets the user’s “handle” by initial query (a one-word name, up to
# 10 characters). chatclient will display this handle as a prompt on host B, and will
# prepend it to all messages sent to host A. e.g., “ SteveO> Hi!!”

# chatclient on host B sends an initial message to chatserve on host A : PORTNUM.
# This causes a connection to be established between Host A and Host B. Host A and
# host B are now peers, and may alternate sending and receiving messages. Responses
# from host A should have host A’s “handle” prepended.

# Host B responds to Host A, or closes the connection with the command “\quit”

# If the connection is not closed, repeat from 6.

# Your programs should be able to send messages of up to 500 characters.
# ------------------------------------

import sys	# for sys.argv (command line input)

from socket import * 

def clientStart:
# start chatclient by specifying hostname/port
	# sys.argv[1] and [2] are serverName and serverPort
	clientSocket = socket(AF_INET, SOCK_STREAM)
    clientSocket.connect((sys.argv[1],int(sys.argv[2])))

# Gets username and provides input validation for length
# Returns username
def getUserHandle:
	# flag to break the while loop
	validHandle = false

	while(!validHandle)
		userHandle = raw_input("Enter a username 10 characters or less: ")
		if not userHandle:	
			print "You entered nothing; try again."

		elif len(userHandle) > 10
			print "Your name is too long; try again."
		
		# User handle is valild
		else:	
			"Thanks " + userHandle + "!"
			validHandle = true

	return userHandle

# This function receives user input and sends it to the server
# It also quits the program
def messageServer(userHandle):
# while !quit
	# read input
		message = raw_input("userHandle> ")
		clientSocket.send(message)
		# send input, to server
		# if "\quit", terminate program
			# clientSocket.close()
			sys.exit()


	# await messages (from server)
		### modifiedSentence = clientSocket.recv(1024)
		### print ‘From Server:’, modifiedSentence

		# append to screen
		# go to stop of while

def main:
	clientStart()
	userHandle = getUserHandle()
	messageServer(userHandle)

























