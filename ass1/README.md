Please follow these steps in order to use the chat programs on FLIP:

1. Compile 'chatServer.cpp' with the following command:
	
		g++ -o <executable_name> chatserve.cpp

	example:

		g++ -o server chatserve.cpp

	- <executable_name> is the name you wish to call your executable file
	- do not include the < >


2. Execute the server with the following command
		
		<executable_name> <port_number>

	example:

		server 30000

	- <port_number> should not be a well-known port number


3. Execute the chat client with the following command

		python chatclient.py <server_address> <server_port>

	example:

		python chatclient.py localhost 30000

	- server port must match server specified port


4. Control Program. The client can send a message first after declaring
a username. Once a message is sent to the server, the client must wait
for a message from the server before sending another.

To stop the client and server from either program, type '\quit', without
quotes.