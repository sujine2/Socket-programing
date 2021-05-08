// Server side Socket Programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h> //socket ip and address
#include <string.h>



int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    struct sockaddr_in con_address;
    int addrlen = sizeof(address);
    int conlen = sizeof(con_address);
    char buffer[1024] = {0};
    char sendbuffer[1024] = {0};

    char msg[] = "클라이언트가 입장하였습니다.채팅을 입력하세요. :-) 나가기(q or quit)";
    int len = 0;

    if(strcmp("TCP", argv[1]) == 0){

		
	    /* Create socket */
	    server_fd = socket(PF_INET, SOCK_STREAM, 0);
	    if( server_fd == -1) {
	    	printf("socket error");
	    	return -1;  
             }

	    /* Set IP and port */
	     memset(&address,0,sizeof(address));
	     address.sin_family= AF_INET;  //ipv4 주소체계에서 사용하므로 항상 AF_INET으로
	     address.sin_addr.s_addr = htons(INADDR_ANY);
	     address.sin_port = htons(atoi(argv[2]));  //port 번호

	    /* Bind */
	     if (bind(server_fd, (struct sockaddr*)&address, addrlen)==-1){ //sever_fd가 가리키는 소켓에 
		printf("bind error");      
		return -1; 
	     }             //구조체가 가리키는 주소 정보할당
             printf("Server: create Tcp socket\n");

	    /* Listen */ 
	    if(listen(server_fd,5)==-1){ //대기 큐5
            	printf("listen error");
		return -1;
             }
	     else {
             	printf("Server: waiting connetion request\n");
             } 
	    
	 

	     /* Accept */
	     new_socket=accept(server_fd, (struct sockaddr*)&address, &addrlen);
	     if(new_socket!=-1) 
             	printf("Client connected!\n"); 
	     else  {
	     	printf("Accept error");
	     	return -1;
             } 

	     printf("%s\n", msg);
             
	     while(1){
                memset(buffer, 0, sizeof(char)*1024);
                memset(sendbuffer, 0, sizeof(char)*1024);

		len = recv(new_socket, buffer, 1024, 0);
		if(strcmp(buffer, "quit\n") == 0 || strcmp(buffer,"q\n") == 0) {
		  printf("Client connect closed\n");
		  break;
		}
		if(len >= 0) 
	        	printf("\n[client] %s\n", buffer);
		
                memset(buffer, 0, sizeof(char)*1024);
		
	        
		read(0, sendbuffer, 1024);
		len = send(new_socket, sendbuffer, 1024 , 0);
                if(strcmp(sendbuffer, "quit\n") == 0 || strcmp(sendbuffer,"q\n") == 0) {
		  printf("Connect closed\n");
		  break;
		}

		if(len <= 0) {
			printf("send error\n");
                	return -1;
		}
		memset(sendbuffer, 0, sizeof(char)*1024);
            }
	    close(server_fd);
            close(new_socket);
           
    }
    else if(strcmp("UDP", argv[1]) == 0){
    	    /* Create socket */
	    server_fd = socket(PF_INET, SOCK_DGRAM, 0);
	    if( server_fd == -1) {
	    	printf("socket error");
	    	return -1;  
	     }

	    /* Set IP and port */
	     memset(&address,0,sizeof(address));
	     address.sin_family= AF_INET;  //ipv4 주소체계에서 사용하므로 항상 AF_INET으로
	     address.sin_addr.s_addr = htons(INADDR_ANY);
	     address.sin_port = htons(atoi(argv[2]));  //port 번호

	    /* Bind */
	     if (bind(server_fd, (struct sockaddr*)&address, addrlen)==-1){ //sever_fd가 가리키는 소켓에 
		printf("bind error");      
		return -1; 
	     }             //구조체가 가리키는 주소 정보할당
	     printf("Server: create Udp socket\n");
             
            while(1){
            	
            	memset(buffer, 0, sizeof(char)*1024);
                memset(sendbuffer, 0, sizeof(char)*1024);

                len = recvfrom(server_fd, buffer, 1024, 0, (struct sockaddr*)&con_address, &conlen);
                if(strcmp(buffer, "quit\n") == 0 || strcmp(buffer,"q\n") == 0) {
		  printf("Client connect closed\n");
		  break;
		}
		if(len >= 0) 
	        	printf("\n[client] %s\n", buffer);
		
                memset(buffer, 0, sizeof(char)*1024);

                read(0, sendbuffer, 1024);
		len = sendto(server_fd, sendbuffer, 1024 , 0,(struct sockaddr*)&con_address, conlen);
                if(strcmp(sendbuffer, "quit\n") == 0 || strcmp(sendbuffer,"q\n") == 0) {
		  printf("Connect closed\n");
		  break;
		}

		if(len <= 0) {
			printf("send error\n");
                	return -1;
		}
		memset(sendbuffer, 0, sizeof(char)*1024);
            }
            close(server_fd);

    }

    return 0;
}
