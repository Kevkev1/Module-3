#include "ip6header.h"

#include <string.h>

ip6Header::ip6Header() {
	this->version = 6;
	this->trafficClass = 0;
	this->flowLabel = 0;

	this->payloadLength = 0;
	this->nextHeader = 0;
	this->hopLimit = 0;
}

void ip6Header::serialize(uint8_t* buffer) {
	// version, trafficClass and flowLabel fields.
	uint32_t firstSegment = (version & 0x15) << 24 | trafficClass << 20 | (flowLabel & 0xFFFFF);

	buffer[0] = firstSegment >> 24;
	buffer[1] = (firstSegment >> 16) & 0xff;
	buffer[2] = (firstSegment >> 8) & 0xff;
	buffer[3] = firstSegment & 0xff;
	buffer[4] = payloadLength >> 8;
	buffer[5] = payloadLength & 0xff;
	buffer[6] = nextHeader;
	buffer[7] = hopLimit;

	memcpy(buffer + 7,  sourceAddress, 16);
	memcpy(buffer + 23, destAddress,   16);
}
