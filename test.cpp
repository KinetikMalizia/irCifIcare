#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 6667
#define MAXLINE 4096

int main(void) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE + 1];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "SOCKET ERROR\n";
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &servaddr.sin_addr) <= 0) {
        std::cout << "INET_PTON ERROR\n";
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        std::cout << "CONNECT ERROR\n";
        return 1;
    }
		memset(sendline, 0, MAXLINE);
		snprintf(sendline, sizeof(sendline), "QSERV\r\n");
		if (write(sockfd, sendline, strlen(sendline)) < 0) {
			std::cout << "WRITE ERROR\n";
			return 1;
		}
// 	//for (;;) {
// 		// Send "PING" message
// 		memset(sendline, 0, MAXLINE);
// 		snprintf(sendline, sizeof(sendline), "/QSERV\r\n");
// 		if (write(sockfd, sendline, strlen(sendline)) < 0) {
// 			std::cout << "WRITE ERROR\n";
// 			return 1;
// 	//	}
// 		std::cout << "Sent: " << sendline;

// 		// // Wait for "PONG" response
// 		// bool received_pong = false;
// 		// while (!received_pong) {
// 		// 	memset(recvline, 0, MAXLINE);
// 		// 	if ((n = read(sockfd, recvline, MAXLINE)) < 0) {
// 		// 		std::cout << "READ ERROR\n";
// 		// 		return 1;
// 		// 	} else if (n == 0) {
// 		// 		std::cout << "CONNECTION CLOSED BY SERVER\n";
// 		// 		return 1;
// 		// 	}
// 		// 	std::cout << "Received: " << recvline;
// 		// 	if (strcmp(recvline, "PONG\n") == 0) {
// 		// 		received_pong = true;
// 		// 	}
// 		// }
//     	// sleep(10);
// }
	shutdown(sockfd, 2);
    close(sockfd);
    return 0;
}