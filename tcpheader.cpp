#include "tcpheader.hpp"

tcpHeader::tcpHeader() {
	this->sourcePort = 0;
	this->destPort = 0;
	this->seqNum = 0;
	this->ackNum = 0;
	this->dataOffset = 0;
	this->reservedBits = 0;
	this->controlBits = 0;
	this->window = 0;
	this->checksum = 0;
	this->urgentPointer = 0;
}

void tcpHeader::serialize(uint8_t* buffer) {	
	buffer[0] = sourcePort >> 8 & 0xFF;
	buffer[1] = sourcePort & 0xFF; 

	buffer[2] = destPort >> 8 & 0xFF;
	buffer[3] = destPort & 0xFF;

	buffer[4] = seqNum >> 24 & 0xFF;
	buffer[5] = seqNum >> 16 & 0xFF;
	buffer[6] = seqNum >> 8 & 0xFF;
	buffer[7] = seqNum & 0xFF;

	buffer[8] = ackNum >> 24 & 0xFF;
	buffer[9] = ackNum >> 16 & 0xFF;
	buffer[10] = ackNum >> 8 & 0xFF;
	buffer[11] = ackNum & 0xFF;

	buffer[12] = dataOffset << 4 & 0xFF;
	buffer[13] = controlBits & 0xFF;
	
	buffer[14] = window >> 8 & 0xFF;
	buffer[15] = window & 0xFF;

	buffer[16] = checksum >> 8 & 0xFF;
	buffer[17] = checksum & 0xFF;

	buffer[18] = urgentPointer >> 8 & 0xFF;
	buffer[19] = urgentPointer & 0xFF;
}

tcpHeader* tcpHeader::deserialize(uint8_t* buffer) {
	tcpHeader* header = new tcpHeader();

	header->sourcePort = (buffer[1] << 8 | buffer[0]);
	header->destPort = (buffer[3] << 8 | buffer[2]);
	header->seqNum = (buffer[7] << 24 | buffer[6] << 16 | buffer[5] << 8 | buffer[0]);
	header->ackNum = (buffer[11] << 24 | buffer[10] << 16 | buffer[9] << 8 | buffer[8]);
	header->dataOffset = (buffer[12] >> 4);
	header->controlBits = (buffer[13]);
	header->window = (buffer[15] << 8 | buffer[14]);
	header->checksum = (buffer[17] << 8 | buffer[16]);
	header->urgentPointer = (buffer[19] << 8 | buffer[18]);

	return header;
}




