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
	buffer[0] = (version << 4) | ((trafficClass >> 4) & 0x15);
	buffer[1] = (trafficClass & 0x15) << 4 | ((flowLabel >> 20) & 0x15);
	buffer[2] = flowLabel >> 8;
	buffer[3] = flowLabel & 0xff;
	buffer[4] = payloadLength >> 8;
	buffer[5] = payloadLength & 0xff;
	buffer[6] = nextHeader;
	buffer[7] = hopLimit;
	memcpy(buffer + 8, sourceAddress, 16);
	memcpy(buffer + 24, destAddress, 16);
}
