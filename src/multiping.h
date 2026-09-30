#ifndef MULTIPING_H
#define MULTIPING_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/icmp.h>
#include <linux/ip.h>

#define MAXHOSTS 5
#define BUFSIZE 1000 

extern struct timeval starttime;
extern struct timeval endtime;
extern char sendbuf[BUFSIZE];
extern int pid;
extern int nsent[MAXHOSTS];
extern int sockfd;
extern struct in_addr targetip[MAXHOSTS];
extern int ntargets;
extern int ti; // target index
extern long unsigned ntpackets; // number of transmitted ping packet
extern long unsigned nrpackets; // number of received ping packet
extern ssize_t sizetdata; // transmitted data size

typedef struct {
    struct 	icmphdr icmph;	// 8  Bytes
    struct 	timeval tv;		// 16 Bytes
	char 	padding[40];	// 40 Bytes
} ping_packet_t; 			// Total=64 Bytes

void icmp_ping( struct in_addr target_ipaddr);
void icmp_pong( struct in_addr target_ipaddr);
unsigned short calculate_checksum(void *b, int len);

int calculate_duration(struct timeval* duration, const struct timeval* late, const struct timeval* early); 
// duration = late - early, return -1 if invalid duration

void printcolor();

#endif // MULTIPING_H
