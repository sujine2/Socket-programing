// Client side Socket Programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) //argc:명령어라인의 문자열 갯수 argv: 명령어라인의 문자열
{
    int client_fd = 0;
    struct sockaddr_in serv_addr;
    //struct sockaddr_in con_address;
    int addrlen = sizeof(serv_addr);
    int len = 0;
    char buffer[1024] = {0};
    char sendbuffer[1024] = {0};
    char msg[] = "채팅을 입력하세요. :-) 나가기(q or quit)";

    if(strcmp("TCP", argv[1]) == 0){
	     /* Create socket */
	     client_fd = socket(PF_INET, SOCK_STREAM, 0); //소켓 디스크립터 반환!
	     if(client_fd == -1) {
	       printf("error");
		      return -1;}
	     
	    /* Set IP and PORT */
	     memset(&serv_addr,0,sizeof(serv_addr));  
	     serv_addr.sin_family= AF_INET;
	     serv_addr.sin_addr.s_addr = htons(INADDR_ANY); //서버 주소 연결
	     serv_addr.sin_port = htons(atoi(argv[2]));   


	    /* Connect */
	    if (connect(client_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
            	printf("error");
	    	return -1;
            } 
	    else { printf("Client socket Created and Connected!\n");
	    printf("채팅을 입력하세요. :-) 나가기 (q or quit)\n");  }

	    while(1){
                memset(buffer, 0, sizeof(char)*1024);
                memset(sendbuffer, 0, sizeof(char)*1024);

                read(0, sendbuffer, 1024);
		len = send(client_fd, sendbuffer, 1024 , 0);
                if(strcmp(sendbuffer, "quit\n") == 0 || strcmp(sendbuffer,"q\n") == 0) {
		  printf("Connect closed\n");
		  break;
		}

		if(len <= 0) {
			printf("send error\n");
                	return -1;
		}
		memset(sendbuffer, 0, sizeof(char)*1024);

		len = recv(client_fd, buffer, 1024, 0);
		if(strcmp(buffer, "quit\n") == 0 || strcmp(buffer,"q\n") == 0) {
		  printf("Server connect closed\n");
		  break;
		}
		if(len >= 0) 
	        	printf("\n[Server] %s\n", buffer);
		
                memset(buffer, 0, sizeof(char)*1024);
		
             }
	    close(client_fd);
	   
    }
    else if(strcmp("UDP", argv[1]) == 0){
    	    /* Create socket */
	    client_fd = socket(PF_INET, SOCK_DGRAM, 0);
	    if( client_fd == -1) {
	    	printf("socket error");
	    	return -1;  
	     }

	    /* Set IP and port */
	     memset(&serv_addr,0,sizeof(serv_addr)); 
	     serv_addr.sin_family= AF_INET;
	     serv_addr.sin_addr.s_addr = htons(INADDR_ANY); //서버 주소 연결
	     serv_addr.sin_port = htons(atoi(argv[2]));

	    
            while(1){
            	
            	memset(buffer, 0, sizeof(char)*1024);
                memset(sendbuffer, 0, sizeof(char)*1024);

                read(0, sendbuffer, 1024);
		len = sendto(client_fd, sendbuffer, 1024 , 0,(struct sockaddr*)&serv_addr, addrlen);
                if(strcmp(sendbuffer, "quit\n") == 0 || strcmp(sendbuffer,"q\n") == 0) {
		  printf("Connect closed\n");
		  break;
		}

		if(len <= 0) {
			printf("send error\n");
                	return -1;
		}
		memset(sendbuffer, 0, sizeof(char)*1024);


                len = recvfrom(client_fd, buffer, 1024, 0, (struct sockaddr*)&serv_addr, &addrlen);
                if(strcmp(buffer, "quit\n") == 0 || strcmp(buffer,"q\n") == 0) {
		  printf("Server connect closed\n");
		  break;
		}
		if(len >= 0) 
	        	printf("\n[Server] %s\n", buffer);
		
                memset(buffer, 0, sizeof(char)*1024);
            }
            close(client_fd);

    }

    return 0;
}
