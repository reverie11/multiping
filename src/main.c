#include "colors.h"
#include "multiping.h"

void sigalrm(int signo){
	if(ti >= ntargets) ti = 0;
	icmp_ping(targetip[ti++]);
	return;
}

void sigint(int signo){
	printf("\nSIGINT detected: stopping multiping...\n");
	struct timeval duration;
	gettimeofday(&endtime, NULL);
	calculate_duration(&duration, &endtime, &starttime);
	printf(COLOR_FG_RED "\n------ multiping statistics ------\n")	;
	printf("%lu packets transmitted\n", ntpackets);
	printf("%lu packets received\n", nrpackets);
	printf("%.1f %% of packet loss\n", (float)(ntpackets-nrpackets)*100/(float)ntpackets );
	printf("%lu bytes of data transmitted\n", sizetdata + ntpackets*20);
	printf("transmission lasts %lu s %lu ms\n\n", duration.tv_sec, duration.tv_usec/1000);
	printf(COLOR_RESET);
	close(sockfd);
	_exit(0);
}

int main(int argc, char** argv)
{
	pid_t pid = getpid() & 0xffff;
	memset(nsent, 0, sizeof(nsent));
	ntargets = 0;
	ntpackets = 0;
	nrpackets = 0;
	ti = 0;

	if(argc == 1){
		fprintf(stderr, COLOR_FG_RED "usage: multiping <IPv4 addresses...>\n" COLOR_RESET);
		return -1;
	}
	for(int i = 0; i < argc -1; i++) {
		if(i > MAXHOSTS) break;

		if( (targetip[i].s_addr = inet_addr(argv[i+1])) == -1){
			fprintf(stderr, "invalid IP addr\n");
			return -1;
		}else ntargets++;

		printf("ip[%d]: %s\n", i, inet_ntoa(targetip[i]));
	}
	printf("----------------------------------\n");
	//printf("number of target: %d\n", ntargets);

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	signal(SIGALRM, sigalrm);
	signal(SIGINT, sigint);

	gettimeofday(&starttime, NULL);
	alarm(1);
	
	while(1)
	{
		printcolor();
		icmp_pong(targetip[ti]);
	}
	close(sockfd);

}

