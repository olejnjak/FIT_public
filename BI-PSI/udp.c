/*
 *
 *  BI-PSI excercise, UDP downloader & uploader.
 *	Author: Jakub Olejnik <olejnjak@fit.cvut.cz>
 *
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define PORT 4000
#define HEADER_LEN 9
#define TIMEOUT 100000 // 100ms in usecs

#define SYN 0x4
#define FIN 0x2
#define RST 0x1
#define DATA 0x0

#define DOWNLOAD 0x1
#define UPLOAD 0x2

#define SOCKET_T int

#define EXIT_NO_RESPONSE 1
#define EXIT_NO_SYN_RECEIVED 2
#define EXIT_WRONG_ADDRESS 3
#define EXIT_CANT_SEND_SYN 4
#define EXIT_WRONG_SYN 5
#define EXIT_RST_RECEIVED 6
#define EXIT_RST_SENT 7
#define EXIT_TIMEOUT 8

#define FILENAME "foto.png"
#define WINDOW_SIZE 8
#define MAX_SHORT 65535

/**
  * struct representing packet
  */
typedef struct
{
	unsigned char 	id[4];
	unsigned char 	seq[2];
	unsigned char 	ack[2];
	unsigned char 	flag;
	unsigned char 	data[264];
	unsigned short 	data_size;
} packet_t;

/**
  * struct representing connection
  */
typedef struct
{
	unsigned int 		id;
	unsigned short		seq;
	SOCKET_T 			sockfd;
	struct sockaddr_in	addr;
} connection_t;

/**
  * function converts packet_t to unsigned char * to be sent by sendto function
  * caller is responsible to free returned buffer
  */
unsigned char* createPacket (const packet_t* packet, int* packet_len)
{
	if (packet == NULL) // no packet is given
	{
		*packet_len = 0;

		return NULL;
	}

	//allocate and initialize memory for packet
	int allocate = HEADER_LEN + packet->data_size;
	unsigned char * real_packet = (unsigned char*) malloc(allocate);
	memset(real_packet, 0, allocate);

	int size = 0;

	//copy id
	memcpy(real_packet, &packet->id, sizeof(packet->id));
	size += sizeof(packet->id);

	//copy seq number in big endian
	real_packet[4] = packet->seq[0];
	real_packet[5] = packet->seq[1];
	size += sizeof(packet->seq);

	real_packet[6] = packet->ack[1];
	real_packet[7] = packet->ack[0];
	size += sizeof(packet->ack);

	//copy flag
	memcpy(real_packet + size, &packet->flag, sizeof(packet->flag));
	size += sizeof(packet->flag);

	//copy data
	memcpy(real_packet + size, packet->data, packet->data_size);
	size += packet->data_size;

	//set packet size
	*packet_len = size;

	return real_packet;
}

/**
  *
  * function prints packet to stdout
  * used for debug purposes
  *
  */
void printPacket (const packet_t* packet, char printData)
{
	if (packet == NULL)
	{
		printf("No packet.\n");

		return;
	}

	int i;

	printf("%02X%02X%02X%02X ", packet->id[0], packet->id[1], packet->id[2], packet->id[3]);

	unsigned short seq = packet->seq[0];
	seq <<= 8; seq += packet->seq[1];

	printf("seq=%u ", seq);

	unsigned short ack = packet->ack[1];
	ack <<= 8; ack += packet->ack[0];
	printf("ack=%u ", ack);

	printf("flags=%u ", packet->flag);

	printf("data(%d) ", packet->data_size);

	if (printData)
	{
		for(i = 0; i < packet->data_size; i++)
		{
			printf("%X ", packet->data[i]);
		}
	}

	printf("\n");
}

/**
  *
  * takes buffer and converts it to packet representation
  * caller is responsible for freeing returned packet
  *
  */
packet_t* bufferToPacket (unsigned char* buf, int len)
{
	if (len < HEADER_LEN)
	{
		return NULL;
	}

	packet_t* ret = (packet_t*) malloc(sizeof(*ret));

	memcpy(&ret->id, buf, 4);

	ret->seq[0] = buf[4];
	ret->seq[1] = buf[5];

	ret->ack[0] = buf[6];
	ret->ack[1] = buf[7];

	ret->flag = buf[8];

	if (len > HEADER_LEN)
	{
		memcpy(&ret->data, buf+9, len-HEADER_LEN);
		ret->data_size = len-HEADER_LEN;
	}

	else
	{
		ret->data_size = 0;
	}

	return ret;
}

/**
  *
  * returns host IP
  *
  */
struct sockaddr_in getAddr (const char* server)
{
	struct sockaddr_in  	addr;
	struct hostent 			* ph;

 	ph = gethostbyname(server);

	if (!ph)
	{
		perror("Given address is not registered to name service.\n");
		exit (EXIT_WRONG_ADDRESS);
	}

	addr.sin_family = AF_INET;
	addr.sin_port=htons(PORT);
  	bcopy(ph->h_addr_list[0], (char*) &addr.sin_addr, ph->h_length);

	return addr;
}

/**
  *
  * sends a packet
  *
  */
void sendPacket(connection_t* conn, const packet_t* packet)
{
	int len;
	int r_packet_length;
	unsigned char * r_packet;

	memcpy(&packet->id, &conn->id, 4);

	r_packet = createPacket(packet, &r_packet_length);

	//print what i'm sending
	printf("send: ");
	printPacket(packet, 0);

	int opts = O_NONBLOCK;
	opts = opts & (~O_NONBLOCK);


	fcntl(conn->sockfd, F_SETFL, opts);
	len = sendto(conn->sockfd, r_packet, r_packet_length, 0, (struct sockaddr*) &conn->addr, sizeof(conn->addr));
	fcntl(conn->sockfd, F_SETFL, O_NONBLOCK);

	if (len < 0 && packet->flag == SYN)
	{
		close(conn->sockfd);
		free(conn);
		perror("Unable to send SYN packet.");
		exit(EXIT_CANT_SEND_SYN);
	}

	else if (len < 0)
	{
		perror ("Unable to send packet.");
	}

	free (r_packet);
}

/**
  *
  * send RST packet
  *
  */
void sendRst(connection_t* conn)
{
	packet_t packet;

	memcpy (&packet.id, &conn->id, 4);
	memset (&packet.ack, 0, 2);
	memset (&packet.ack, 0, 2);
	packet.flag = RST;
	packet.data_size = 0;

	sendPacket (conn, &packet);
}

/**
  *
  * check whether given packet can be SYN
  *
  */
char isValidSyn (packet_t * packet, connection_t * conn, unsigned char cmd)
{
	if (!packet)
	{
		return 0;
	}

	if (packet->flag == SYN && (packet->data_size == 0 || (packet->data_size != 1 && packet->data [0] != DOWNLOAD && packet->data[0] != UPLOAD)))
	{
		sendRst(conn);

		printf("Invalid data in SYN packet.");
		free(packet);
		exit(EXIT_RST_SENT);
	}

	// missing connection ID
	if (packet->id[0] == 0 && packet->id[1] == 0 && packet->id[2] == 0 && packet->id[3] == 0)
	{
		return 0;
	}

	// SYN doesn't have a seq number
	if (packet->seq[0] != 0 || packet->seq[1] != 0)
	{
		return 0;
	}

	// SYN doesn't have an ACK
	if (packet->ack[0] != 0 || packet->ack[1] != 0)
	{
		return 0;
	}

	// SYN has to have the SYN flag
	if (packet->flag != SYN)
	{
		return 0;
	}

	// check that data contain valid command
	if (packet->data_size != 1 || packet->data[0] != cmd)
	{
		return 0;
	}

	return 1;
}

/**
  *
  * receive one packet from connection
  *
  */
packet_t* receivePacket (connection_t * conn) {
	unsigned char rbuf[265];
	unsigned addr_len = sizeof(conn->addr);

	fd_set fdset;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = TIMEOUT;

	FD_ZERO(&fdset);
  FD_SET(conn->sockfd, &fdset);

	int r = select(conn->sockfd+1, &fdset, NULL, NULL, &tv);


	if (r == 1) {
		int so_error;
    socklen_t len2 = sizeof (so_error);

    getsockopt(conn->sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len2);

    if (so_error == 0) {
    		int len = recvfrom(conn->sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *) &conn->addr, &addr_len);
        return bufferToPacket(rbuf, len);
    }
	}

	return NULL;

}

/**
  *
  * send ACK packet to connection
  *
  */
void sendAck(connection_t * conn) {
	packet_t packet;
	memset(&packet, 0, sizeof(packet));
	memcpy(&packet.id, &conn->id, 4);
	memcpy(&packet.ack, &conn->seq, 2);
	packet.data_size = 0;

	sendPacket(conn, &packet);
}

/**
  *
  * check whether packet is FIN
  *
  */
char isFin (packet_t * packet) {
	if (!packet) return 0;
	if (packet->data_size == 0 && packet->flag == FIN) return 1;
	return 0;
}

/**
  *
  * check whether packet is RST
  *
  */
char isRst (packet_t * packet) {
	if (!packet) return 0;
	if (packet->data_size == 0 && packet->flag == RST) return 1;
	return 0;
}

/**
  *
  * send SYN packet to server
  *
  */
connection_t * sendSyn (SOCKET_T sockfd, const char * server, unsigned char cmd) {
	packet_t packet = {{0,0,0,0},{0,0},{0,0}, SYN, {cmd}, 1};
	connection_t * conn = (connection_t*) malloc (sizeof(*conn));

	//get address
	struct sockaddr_in addr = getAddr(server);
	packet_t * r_packet = NULL;

	memcpy(&conn->id, &packet.id, sizeof(conn->id));
	memcpy(&conn->seq, &packet.seq, sizeof(conn->seq));
	conn->sockfd = sockfd;
	conn->addr = addr;
	sendPacket(conn, &packet);

	//try 20 times
	int i = 1;
	while (1) {
		memset(&conn->id, 0, 4);
		r_packet = receivePacket(conn);
		//printPacket(r_packet, 0);

		if (r_packet != NULL) {
			memcpy(&conn->id, &r_packet->id, 4);
			if(!isValidSyn(r_packet, conn, cmd)) continue;
			else {
				break;
			}
		}
		if (!r_packet) {
			sendPacket(conn, &packet);
			i++;
		}

		free (r_packet);
		r_packet = NULL;

		if (i == 20) {
			printf("No SYN packet received.\n");
			exit(EXIT_NO_SYN_RECEIVED);
		}
	}

	printf("Valid SYN packet received.\n");
	printPacket(r_packet, 0);

	free(r_packet);
	return conn;
}

/**
  *
  * send FIN packet
  *
  */
void sendFin (connection_t * conn, unsigned char cmd) {
	packet_t packet;
	memcpy (&packet.id, &conn->id, 4);
	if (cmd == DOWNLOAD) {
		memcpy (&packet.ack, &conn->seq, 2);
		memset (&packet.seq, 0, 2);
	} else {
		memcpy (&packet.seq, &conn->seq, 2);
		memset (&packet.ack, 0, 2);
	}
	packet.flag = FIN;
	packet.data_size = 0;
	sendPacket (conn, &packet);
}

/**
  *
  * write photo from memory to file
  *
  */
void writePhoto (unsigned char ** data, unsigned char * access, int last_size) {
	unsigned short i = 0;
	FILE * file = fopen(FILENAME, "wb");

	while (1) {
		unsigned short j = i; j += 255;
		if (access[j]) {
			fwrite(data[i], 255, 1, file);
			free (data[i]);
			i += 255;
		}
		else {
			fwrite(data[i], last_size, 1, file);
			free (data[i]);
			break;
		}
	}

	fclose(file);
}

/**
  *
  * download photo
  *
  */
void photoDownload (const char * server) {
	SOCKET_T socketfd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	connection_t * 		conn;
	packet_t 		 *		packet;
	unsigned char **	data;
	unsigned char *		access;
	unsigned short 		max_seq = 65535;
	int 							i;
	int								last_size;


  fcntl(socketfd, F_SETFL, O_NONBLOCK);

	//send SYN to get connection
	conn = sendSyn(socketfd, server, DOWNLOAD);
	conn->seq = 0;

	packet = receivePacket(conn);

	data = (unsigned char**) malloc (70000*sizeof(*data));
	access = (unsigned char*) malloc (70000*sizeof(*access));
	memset (access, 0, 70000*sizeof(*access));

	while (!isFin(packet)) {
		printf("receive: ");
		printPacket(packet, 0);

		if (packet && (isValidSyn(packet, conn, DOWNLOAD) || *((int*)packet->id) != conn->id)) {
			connection_t c2 = *conn;
			memcpy(&c2.id, &packet->id, 4);
			sendRst(&c2);
			free(packet);
			packet = receivePacket(conn);
			continue;
		}

		if (isRst(packet)) {
			printf("RST packet received, terminating.\n");
			free(packet);
			free(conn);
			free(access);
			free(data);
			close(socketfd);

			exit(EXIT_RST_RECEIVED);
		}

		if (packet) {
			unsigned short seq = packet->seq[0];
			seq <<= 8; seq+= packet->seq[1];

			if (packet->data_size > 0 && packet->data_size < 255) {
				max_seq = seq + packet->data_size;
				last_size = packet->data_size;
			}

			if (!access[seq]) {
				data[seq] = (unsigned char*) malloc(300*sizeof(*data[seq]));
				memcpy ( data[seq], packet->data, packet->data_size);
			}
			access[seq] = 1;

			if (conn->seq == seq) {
				access[conn->seq] = 1;

				while(access[conn->seq]) conn->seq += packet->data_size;
			}

			if (conn->seq > max_seq) conn->seq = max_seq;
			sendAck(conn);

			free(packet);
		}
		packet = receivePacket(conn);
	}

	if (packet->data_size != 0) {
		sendRst(conn);
		printf("Invalid FIN packet, containing data.\n");
		free(data);
		free(access);
		free(packet);
		close (socketfd);
		free(conn);
		exit (EXIT_RST_SENT);
	}

	for (i = 0; i < 20; i++) sendFin(conn, DOWNLOAD);

	writePhoto(data, access, last_size);
	printf("All data received, photo written.\n");

	free(data);
	free(access);
	free(packet);
	close (socketfd);
	free(conn);
}

/**
 *
 * reads packets from given file and saves them to array
 *
 */
packet_t ** readPackets (const connection_t * conn, const char * fw) {
	FILE * file = fopen(fw, "rb");
	int i;

	if (!file) return NULL;

	packet_t ** packets = (packet_t**) malloc (70000*sizeof(*packets));
	for (i = 0; i < 70000; i++) packets[i] = NULL;
	//memset (*packets, 0, 70000*sizeof(*packets));

	unsigned short seq = 0;

	while (1) {
		int hseq = htons(seq);
		packet_t * packet = (packet_t*) malloc (sizeof(*packet));
		memcpy (&packet->id, &conn->id, 4);
		memcpy (&packet->seq, &hseq, 2);
		memset (&packet->ack, 0, 2);
		memset (&packet->flag, 0, 1);

		packet->data_size = fread(&packet->data, 1, 255, file);

		packets[seq] = packet;
		seq += packet->data_size;
		if (packet->data_size < 255) break;
	}

	fclose(file);
	return packets;
}

/**
 *
 *
 * sends one sliding window to server
 *
 */
void sendWindow (connection_t * conn, packet_t ** packets, unsigned short start) {
	int i;
	for (i = 0; i < WINDOW_SIZE; i++) {
		sendPacket(conn, packets[start]);

		if (packets[start]->data_size < 255) break;
		start += packets[start]->data_size;
	}
}

/**
 *
 *
 * reads acks received from server or timeouts
 *
 */
void readAcks (const connection_t * conn, unsigned short * seq) {
	fd_set fdset;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = TIMEOUT;
	int r, i;
	unsigned addr_len = sizeof(conn->addr);
	unsigned char rbuf[265];

	for (i = 0; i < WINDOW_SIZE; i++) {
		FD_ZERO(&fdset);
		FD_SET(conn->sockfd, &fdset);
		r = select(conn->sockfd+1, &fdset, NULL, NULL, &tv);
		i++;

		if (r==1) {
			int so_error;
			socklen_t len = sizeof(so_error);

			getsockopt(conn->sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);
			if (so_error == 0) {
				int len = recvfrom(conn->sockfd, rbuf, sizeof(rbuf), 0, (struct sockaddr *) &conn->addr, &addr_len);

				packet_t * p = bufferToPacket(rbuf, len);
				unsigned char c = p->ack[0]; p->ack[0] = p->ack[1]; p->ack[1] = c;
				printf("receive: ");
				printPacket(p, 0);
				free(p);

				unsigned short s;
				memcpy(&s, rbuf+6, 2);
				s = htons(s);
				if (s > *seq || s + 60000 < *seq) *seq = s;
				//if (s > *seq) *seq = s;
			}
		}
	}
}

/**
 *
 * start firmware upload
 *
 */
void firmwareUpload (const char * server, const char * firmware) {
	SOCKET_T 					socketfd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	connection_t 		* conn = sendSyn(socketfd, server, UPLOAD);
	packet_t 			 ** packets = readPackets(conn, firmware);
	int								i;

	fcntl(socketfd, F_SETFL, O_NONBLOCK);

	while (packets[conn->seq]) {
		sendWindow( conn, packets, conn->seq);
		readAcks(conn, &conn->seq);
	}

	conn->seq = htons(conn->seq);
	for (i = 0; i < 20; i++) {
		sendFin(conn, UPLOAD);
		packet_t * p = receivePacket(conn);
		if (isFin(p)) {
			printf("Connection successfully closed.\n");
			break;
		}
		else if (i == 19) {
			printf("Connection hasn't been closed by other side.\n");
			break;
		}
	}


	close(socketfd);
	free (conn);
	for (i = 0; i < 70000; i++) free(packets[i]);
	free (packets);
}

int main (int argc, char * argv[]) {
	if (argc == 2) { /* running photo download */
		photoDownload (argv[1]);
	}

	else if (argc == 3) { /* running firmware upload */
		firmwareUpload (argv[1], argv[2]);
	}

	else {
		printf("Usage:\n\t - photo download: %s <server_address>\n\t - firmware upload: %s <server_address> <firmware_file>\n", argv[0], argv[0]);
	}

	return EXIT_SUCCESS;
}
