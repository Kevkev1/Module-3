#include "client.hpp"

#include <stdlib.h>
#include <stdio.h>

int main(void) {
	connect();

	bool done = false;
	while (!done) {
		// TODO: Implement your client for the server by combining:
		//        - Send packets, use void send(unsigned char* data, int length).
		//           The data passed to send should contain raw
		//           IP/TCP/(optionally HTTP) headers and data.
		//        - Receive packets, you can retreive byte arrays using
		//           unsigned char* receive(unsigned int timeout).
		//           The timeout passed to this function will indicate the maximum amount of
		//           milliseconds for receive to complete. When the timeout expires before a
		//           packet is received, a null pointer will be returned.
		//
		//           The data you'll receive and send will and should contain all packet 
		//           data from the network layer and up.
		//
		//           The length of the packet that was received last can be queried using
		//           int get_received_length(void).
	}
}
