#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>

void error(char *m){
	perror(m);
}

int main(int argc, char *argv[]){
	int sockfd, newsockfd, port, clilen, n;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int buffer_int;
	int buffer_int_times5;

	if(argc<2)
		error("ERROR, no port provided\n");

	port = atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
		error("ERROR opening socket");

	//open socket
	memset((char*) &serv_addr,0,sizeof(serv_addr));
	//details
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port); //host to network
	//bind the socket
	if(bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR binding to socket");

	//causes the program to inform the OS that it is interested in new connections
	//connecting the socket
	//2 = backlog, number of incoming connections that can be queued for acceptance
	listen(sockfd,2);

	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,&clilen);
	if(newsockfd < 0) error("ERROR on accept");
	
	n = read(newsockfd, buffer, 255);
	if(n<0) error("ERROR reading from socket");

	//debug
	//printf("Message received: %s", buffer);

	printf("Message received!\n");

	buffer_int = atoi(buffer);
	buffer_int_times5 = buffer_int*5;
	//Convert int to string
	//itoa is not supported in ubuntu - have to use snprintf instead
	//itoa(buffer_int_times5,buffer,10); 

	snprintf(buffer,sizeof(buffer),"%d", buffer_int_times5);

	//debug
	//printf("Message to send to client: %s\n", buffer);

	printf("Sending message to client...\n");
		
	n = write(newsockfd,buffer,21);
	if(n<0) error("ERROR writing back to socket");


	return 0;
}

