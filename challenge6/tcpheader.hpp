#ifndef TCP_HEADER_H
#define TCP_HEADER_H

#include <stdint.h>

#define SIZE_TCPHEADER 20

class tcpHeader {
public:
	tcpHeader();

	uint16_t	sourcePort;	/* 16 bits */
	uint16_t	destPort;	/* 16 bits */
	uint32_t	seqNum;		/* 32 bits */
	uint32_t	ackNum;		/* 32 bits */
	uint8_t		dataOffset;	/* 4 bits */
	uint8_t		reservedBits;	/* 6 bits - MUST BE 0 - */
	uint8_t		controlBits;	/* 6 bits
						URG:  Urgent Pointer field significant
    						ACK:  Acknowledgment field significant
    						PSH:  Push Function
    						RST:  Reset the connection
    						SYN:  Synchronize sequence numbers
    						FIN:  No more data from sender */
	uint16_t	window;		/* 16 bits */
	uint16_t	checksum;	/* 16 bits */
	uint16_t	urgentPointer;	/* 16 bits */
	
	void serialize(uint8_t* buffer);
	static tcpHeader* deserialize(uint8_t* buffer);
};

#endif
