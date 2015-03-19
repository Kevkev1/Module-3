void connect(void);
int get_received_length(void);
unsigned char* receive(unsigned int timeout);
void send(unsigned char* data, int length);
