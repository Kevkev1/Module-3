#ifndef IP6_HEADER_H
#define IP6_HEADER_H

#include <stdint.h>

#define SIZE_IP6HEADER 40

class ip6Header {
public:
	ip6Header();

	uint8_t 	version; 			/* 4   bits */
	uint8_t 	trafficClass;		/* 8   bits */
	uint32_t 	flowLabel;			/* 20  bits */
	uint16_t	payloadLength;  	/* 16  bits */
	uint8_t		nextHeader;			/* 8   bits */
	uint8_t		hopLimit;			/* 8   bits */
	uint8_t		sourceAddress[16]; 	/* 128 bits */
	uint8_t		destAddress[16];	/* 128 bits */

	void 		serialize(uint8_t* buffer);
	static ip6Header* deserialize(const uint8_t* buffer);

};

#endif
