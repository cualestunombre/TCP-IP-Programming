#include "../Common.h"

char* SERVERIP = (char*) "127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512


int main(int argc, char* argv[]){

    int retval;

    // 명령행 인수가 있으면 IP 주소로 사용
    if (argc > 1) SERVERIP = argv[1];

    SOCKET sock = socket(AF_INET, SOCK_STREAM,0);

    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0 , sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit(("connect()"));

    char buf[BUFSIZE + 1];
    int len;

    while(1){

        printf("\n[보낼 데이터] ");
        if (fgets(buf,BUFSIZE +1, stdin) == NULL)
            break;
        
        len = (int)strlen(buf);
        if (buf[len-1] == '\n')
            buf[len - 1] = '\0';
        
        if (strlen(buf) == 0)
            break;

        // 데이터 보내기
        // 현재 send함수는 blocking 소켓을 기반으로 동작한다!! 따라서 소켓 송신 버퍼의
        // 여유 공간이 send()함수의 세 번 째 인수인 len보다 작을 경우 호출 프로세스는 대기 상태가 된다
        printf("데이터 보내기");
        retval = send(sock,buf,(int)strlen(buf),0);
        if (retval == SOCKET_ERROR){
            err_display("send()");
            break;
        }

        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n",retval);

        //데이터 받기
        // MSG_PEEK : 수신 버퍼에 데이터 계속 남음
        // MSG_WAITWALL : 원래 recv의 기본 동작은 최대 len크기 만큼만 가져오는 것(len보다 작더라도 안기다림)
        // 그러나 이 옵션을 사용하면 len크기 만큼 복사될 때까지 blocking 된다
        // 만약 recv가 0을 리턴하면, 상대방이 fin 패킷을 날린 것으로, 
        retval = recv(sock,buf,retval,MSG_WAITALL);
        if (retval == SOCKET_ERROR){
            err_display("recv()");
            break;
        }

        // retval이 0을 리턴하면 정상 종료 루틴으로 가야한다
        else if (retval == 0) break;

        buf[retval] = '\0';
        printf("[TCP 클라이언트] %d바이트를 받았습니다.\n",retval);
        printf("[받은 데이터] %s\n",buf);

        

    }

    close(sock);
    return 0;
}
