#include "client.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ip6header.h"
#include "tcpheader.hpp"

void sendPacket(uint8_t* buffer, size_t length);
void sendTcp(int seq, int ack, int ctrl, const uint8_t* buffer, size_t length);
void sendHttp(const char* str, int seq, int ack, int ctrl);
void addData(const uint8_t* data, size_t lenght, int offset);
void printData();
void calculateChecksum(uint8_t* buffer);

uint8_t *totalData = new uint8_t[0];

int main(void) {
	connect();

	printf("Connected\n");

	bool done = false;
	bool handshake = false;
	bool fin = false;
	while (!done) {
		printf("sending\n");

		if(!handshake) sendTcp(1, 0, 2, nullptr, 0);

		uint8_t* recv = receive(1000);
		int32_t recvLength = (get_received_length());

		if(recv == nullptr) {
			printf("timeout\n");
		} else {
			printf("received data: %d bytes\n", recvLength);

			ip6Header* ipHeader = ip6Header::deserialize(recv);
			tcpHeader* header = tcpHeader::deserialize(recv + SIZE_IP6HEADER);

			if(header->controlBits == 18) {
				sendTcp(header->ackNum+1, header->seqNum+1, 16, nullptr, 0);
				handshake = true;
				sendHttp("GET http://[2001:67c:2564:a170:a00:27ff:fe11:cecb]:7711/?nr=1490443\r\n", header->ackNum+1, header->seqNum+1, 16);
			}

			if(header->controlBits == 17) {
				sendTcp(header->ackNum+1, header->seqNum+1, 17, nullptr, 0);
				fin = true;
			}

			if(header->controlBits == 16 || header->controlBits == 24) {
				if(fin) done = true;
				else {
					printf("After\n");
					size_t dataLength = recvLength - (SIZE_IP6HEADER + SIZE_TCPHEADER);
					uint8_t data[dataLength];
					memcpy(data, recv + (SIZE_IP6HEADER + SIZE_TCPHEADER), dataLength);
					addData(data, dataLength, header->dataOffset);
					printData();
					if(header->controlBits == 24) {
						printData();
						sendTcp(header->ackNum+1, header->seqNum+1, 16, nullptr, 0);
					}
				}
			}

			delete header;
			delete ipHeader;
		}
	}
}

void calculateChecksum(uint8_t* buffer) {
	ip6Header* ipHeader = ip6Header::deserialize(buffer);

	uint8_t temp[470];
	memcpy(temp, ipHeader->sourceAddress, 16);
	memcpy(temp + 16, ipHeader->destAddress, 16);

	temp[32] = ipHeader->payloadLength >> 8 & 0xFF;
	temp[33] = ipHeader->payloadLength & 0xFF;

	temp[39] = ipHeader->nextHeader;
	memcpy(temp + 40, buffer + SIZE_IP6HEADER, SIZE_TCPHEADER);
	
	int sum = 0;
	for(int i=0; i<470; i++) {
		sum+= temp[i];
	}
	
	buffer[SIZE_IP6HEADER + 16] = sum >> 8 & 0xFF;
	buffer[SIZE_IP6HEADER + 17] = sum & 0xFF;
} 

void addData(const uint8_t* data, size_t length, int offset) {
	uint8_t tempBuffer[sizeof(totalData)];
	memcpy(tempBuffer, totalData, sizeof(totalData));
	delete totalData;
	totalData = new uint8_t[sizeof(tempBuffer) + length];
	memcpy(totalData, tempBuffer, sizeof(tempBuffer));
	memcpy(totalData + offset, data, length);
}

void printData() {
	printf("Data:\n%s\n",totalData);
}

void sendHttp(const char* str, int seq, int ack, int ctrl) {
	uint8_t buffer[strlen(str)];
	memcpy(buffer, str, strlen(str));
	sendTcp(seq, ack, ctrl, buffer, strlen(str));
	printf("Send HTTP!\n");
}

void sendTcp(int seq, int ack, int ctrl, const uint8_t* buffer = nullptr, size_t length = 0) {
	tcpHeader header;
	header.sourcePort = 50000;
	header.destPort = 7711;
	header.seqNum = seq;
	header.ackNum = ack;
	header.controlBits = ctrl;
	header.window = 8;

	uint8_t serialized[SIZE_TCPHEADER + length];

	header.serialize(serialized);
	memcpy(serialized + SIZE_TCPHEADER, buffer, length);

	sendPacket(serialized, SIZE_TCPHEADER + length);
}

void sendPacket(uint8_t* buffer, size_t length) {
	// dest:   2001:067c:2564:a170:0a00:27ff:fe11:cecb
	// source: 2001:0610:1908:f000:3ea9:f4ff:fe47:ebc8
	static uint8_t destAddress[16] =   { 0x20, 0x01, 0x06, 0x7c, 0x25, 0x64, 0xa1, 0x70, 0x0a, 0x00, 0x27, 0xff, 0xfe, 0x11, 0xce, 0xcb };
	static uint8_t sourceAddress[16] = { 0x20, 0x01, 0x06, 0x7c, 0x25, 0x64, 0xa1, 0x54, 0x06, 0x0c, 0xce, 0xff, 0xfe, 0xd8, 0xba, 0x38};

	ip6Header header;

	header.hopLimit = 8;
	header.nextHeader = 253;
	header.payloadLength = length;
	memcpy(header.destAddress, destAddress, 16);
	memcpy(header.sourceAddress, sourceAddress, 16);

	uint8_t serialized[SIZE_IP6HEADER + length];
	header.serialize(serialized);
	memcpy(serialized + SIZE_IP6HEADER, buffer, length);

	calculateChecksum(buffer);

	send(serialized, static_cast<int>(length + SIZE_IP6HEADER));
}
