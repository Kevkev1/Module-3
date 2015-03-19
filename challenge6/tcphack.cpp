#include "client.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ip6header.h"

void sendPacket(const uint8_t* buffer, size_t length);

int main(void) {
	connect();

	printf("Connected\n");

	bool done = false;
	while (!done) {
		printf("sending\n");

		send(nullptr, 0);

		uint8_t* recv = receive(1000);

		if(recv == nullptr) {
			printf("timeout\n");
		} else {
			printf("received data\n");
		}

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

void sendPacket(const uint8_t* buffer, size_t length) {
	// 2001:067c:2564:a170:0a00:27ff:fe11:cecb
	static uint8_t destAddress[16] = { 0x20, 0x01, 0x06, 0x7c, 0x25, 0x64, 0xa1, 0x70, 0x0a, 0x00, 0x27, 0xff, 0xfe, 0x11, 0xce, 0xcb };

	ip6Header header;

	header.hopLimit = 8;
	header.nextHeader = 6;
	header.payloadLength = length;
	memcpy(header.destAddress, destAddress, 16);

	uint8_t serialized[40 + length];
	header.serialize(serialized);
	memcpy(serialized + 40, buffer, length);

	send(serialized, static_cast<int>(length + 40));
}
