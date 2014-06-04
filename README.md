Client Messenger:
written in C++, by Or Gariany
Allows connection to a Msngr Server (by me).
handling Peer-to-Peer UDP messeges between clients and chat rooms.

client Cli useage:
	c <server ip> - Opens a connection to server.
	lu - Prints the entier user list on the server.
	lcu - Prints the connected users list.
	lr - Print the entier room list on the server.
	lru <roomname> - Prints the users list connected to <roomname>.
	o <username | roomname> - Opens a session with the user / enters a room.
	r <roomname> - Creats a room named <roomname>.
	s <message> - Sends a message.
	l <clientname> - Prints the current status of <clientname>.
	cs - Disconnects current session.
	d - Disconnects from server
	x- Exits application.

known issues:
