#include "../Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char *argv[]){
	int retval;
	
	if(argc >1) SERVERIP = argv[1];
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock ==INVALID_SOCKET) err_quit("socket()");
	
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr); //숫자로 변환 함
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock,(struct sockaddr *)&serveraddr, sizeof(serveraddr));
	// SYN 패킷 보냄
	if(retval == SOCKET_ERROR) err_quit("connect()");
	char buf[BUFSIZE +1];
	int len;
	while(1){
		printf("\n[보낼 데이터");
		if (fgets(buf,BUFSIZE +1,stdin)==NULL)
			break;
		
		len =  (int)strlen(buf);
		if (buf[len-1]=='\n') buf[len-1]='\0';
		if (strlen(buf)==0) break;
		
		retval = send(sock,buf, (int)strlen(buf), 0);
		//블로킹 소켓으로, 커널의 송신 버퍼 사이즈가 전송하고자 하는 데이터보다 클 때 까지 대기했다가 리턴 함
		if (retval == SOCKET_ERROR){
			err_display("send()");
			break;	
		}
		printf("%d 바이트를 보냈습니다. \n",retval);
		
		retval = recv(sock, buf, retval, MSG_WAITALL);
		//MSG_WAITALL시 len(retval)만큼 데이터가 수신 버퍼에 도착해서 응용 프로그램 버퍼에 전부 복사할
		//때까지 기다리고 리턴한다. 단 TCP연결이 fin이나 rst로 종료된 경우 더 이상 기다리지 않고 수신
		//버퍼의 데이터를 응용 프로그램 버퍼에 복사하고 리턴한다
		/*
		기대가능한 리턴값 : 세 번째 인수인 len보다 크지 않은 범위에서 데이터를 응용프로그램 버퍼에 복사한 후,
		실제 복사한 바이트 수를 리턴한다. 이때 recv()함수의 리턴값은 최소1, 최대 len이다
		*/
			err_display("recv()");
			break; 
		}
		else if(retval==0) break;
		buf[retval] = '\0';
		printf("%d바이트를 받았습니다\n",retval);
		printf("받은 데이터: %s",buf);
	}
	close(sock);
	return 0;
} 
