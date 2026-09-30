#include "multiping.h"
#include "colors.h"

struct timeval starttime;
struct timeval endtime;
char sendbuf[BUFSIZE];
int pid;
int nsent[MAXHOSTS];
int sockfd;
struct in_addr targetip[MAXHOSTS];
int ntargets;
int ti; // target index
long unsigned ntpackets; // number of transmitted ping packet
long unsigned nrpackets; // number of received ping packet
ssize_t sizetdata; // transmitted data size

void icmp_ping(struct in_addr target_ipaddr)
{
    struct sockaddr_in destsock;
    memset(&destsock, 0, sizeof(destsock));
    destsock.sin_family = AF_INET;
    destsock.sin_addr = target_ipaddr;

    ping_packet_t msg;
    memset(&msg, 0, sizeof(msg)); // Start with a clean slate

    msg.icmph.type = ICMP_ECHO;
    msg.icmph.code = 0;
    msg.icmph.un.echo.id = pid; // Use htons
    msg.icmph.un.echo.sequence = nsent[ti]++;

    // Handle the unaligned timeval as we discussed
    struct timeval now;
    gettimeofday(&now, NULL);
    memcpy(&msg.tv, &now, sizeof(struct timeval));
	//printf("\n%lu %lu\n", now.tv_sec, now.tv_usec);

    memset(msg.padding, 'S', sizeof(msg.padding));
	msg.padding[40-1] = 'X';

    // Calculate real ICMP checksum
    msg.icmph.checksum = 0; 
    msg.icmph.checksum = calculate_checksum(&msg, sizeof(ping_packet_t)) ;

	ssize_t n = sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*) &destsock, sizeof(destsock));
	ntpackets++;
    if ( n < 0) {
		fflush(stdout);
        perror(COLOR_FG_RED "multiping error: sendto: ");
		printf("\n");
		alarm(1);
    } else {
        //printf("ping sent! (seq=%d)\n", nsent - 1);
		sizetdata += n;
    }
}
void icmp_pong(struct in_addr target_ipaddr)
{
    struct timeval now, duration;

	uint8_t buf[128];
	//memset(buf, 'A', 1024);
	struct sockaddr_in destsock;
	ping_packet_t* msg;
	struct iphdr* msgip;
	struct in_addr ia;

	socklen_t destsocklen = sizeof(struct sockaddr_in);
	memset(buf, 0, 128);

	ssize_t n = recvfrom(sockfd, &buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*) &destsock, &destsocklen);
    gettimeofday(&now, NULL);

	if( n < 0 ) return;
    
    msg = (ping_packet_t*) (buf+20);
	msgip = (struct iphdr*) (buf);
	ia.s_addr = (msgip->saddr);

	if (calculate_duration(&duration, &now, &(msg->tv)) == -1){
		fprintf(stderr, "calculated time is invalid\n");
	}

    if (msg->icmph.type == ICMP_ECHOREPLY) {
		if(msg->icmph.un.echo.id == pid)
		{
			nrpackets++;
			printf("ECHOREPLY received from: %s\n", inet_ntoa(ia));
			printf("packet size\t= %d Bytes\n", ntohs(msgip->tot_len));
			printf("sequence\t= %d\n", msg->icmph.un.echo.sequence);
			printf("duration\t= %lu s %.3f ms\n", duration.tv_sec, (float)duration.tv_usec/1000);
			//printf("data: %s\n", msg->padding);
		}
		printf("\n");
	}
	alarm(1);
}

unsigned short calculate_checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int calculate_duration(struct timeval* duration, const struct timeval* late, const struct timeval* early)
{
	if(late->tv_sec < early->tv_sec) return -1;

	duration->tv_sec = late->tv_sec - early->tv_sec;
	if(late->tv_usec > early->tv_usec) duration->tv_usec = late->tv_usec - early->tv_usec;
	else {
		duration->tv_sec--;
		duration->tv_usec = 1000000 + late->tv_usec - early->tv_usec;
	}
	return 0;
}

void printcolor()
{
	switch(ti-1){
		case 0:
			printf(COLOR_FG_GREEN);
		break;
		case 1:
			printf(COLOR_FG_BLUE);
		break;
		case 2:
			printf(COLOR_FG_YELLOW);
		break;
		case 3:
			printf(COLOR_FG_CYAN);
		break;
		case 4:
			printf(COLOR_FG_MAGENTA);
		break;
		default:
			printf(COLOR_RESET);
		break;
	}
}
