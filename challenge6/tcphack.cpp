#include "client.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ip6header.h"
#include "tcpheader.hpp"

void sendPacket(const uint8_t* buffer, size_t length);
void sendTcp(int seq, int ack, int ctrl);

int main(void) {
	connect();

	printf("Connected\n");

	bool done = false;
	while (!done) {
		printf("sending\n");

		sendTcp(1, 0, 2);

		uint8_t* recv = receive(1000);
		int32_t recvLength = (get_received_length());

		if(recv == nullptr) {
			printf("timeout\n");
		} else {
			printf("received data: %d bytes\n", recvLength);

			ip6Header* ipHeader = ip6Header::deserialize(recv);
			tcpHeader* header = tcpHeader::deserialize(recv + SIZE_IP6HEADER);

			delete header;
			delete ipHeader;
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

void sendTcp(int seq, int ack, int ctrl) {
	tcpHeader header;
	header.sourcePort = rand() % 16384 + 49152;
	header.destPort = 7711;
	header.seqNum = seq;
	header.ackNum = ack;
	header.controlBits = ctrl;
	header.window = 8;

	uint8_t buffer[SIZE_TCPHEADER];

	header.serialize(buffer);
	sendPacket(buffer, SIZE_TCPHEADER);
}

void sendPacket(const uint8_t* buffer, size_t length) {
	// dest:   2001:067c:2564:a170:0a00:27ff:fe11:cecb
	// source: 2001:0610:1908:f000:3ea9:f4ff:fe47:ebc8
	static uint8_t destAddress[16] =   { 0x20, 0x01, 0x06, 0x7c, 0x25, 0x64, 0xa1, 0x70, 0x0a, 0x00, 0x27, 0xff, 0xfe, 0x11, 0xce, 0xcb };
	static uint8_t sourceAddress[16] = { 0x20, 0x01, 0x06, 0x10, 0x19, 0x08, 0xf0, 0x00, 0x3e, 0xa9, 0xf4, 0xff, 0xfe, 0x47, 0xeb, 0xc8 };

	ip6Header header;

	header.hopLimit = 8;
	header.nextHeader = 253;
	header.payloadLength = length;
	memcpy(header.destAddress, destAddress, 16);
	memcpy(header.sourceAddress, sourceAddress, 16);

	uint8_t serialized[SIZE_IP6HEADER + length];
	header.serialize(serialized);
	memcpy(serialized + SIZE_IP6HEADER, buffer, length);

	send(serialized, static_cast<int>(length + SIZE_IP6HEADER));
}
