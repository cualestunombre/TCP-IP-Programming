#include "../Common.h"

#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char *argv[]){
    int retval;
    
    //소켓 생성
    SOCKET listen_sock = socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock == INVALID_SOCKET) err_quit("socket()");

    //bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //네트워크 인터페이스 선택 가능
    serveraddr.sin_port=htons(SERVERPORT);
    retval = bind(listen_sock,(struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(retval == SOCKET_ERROR) err_quit("bind()");

    //listen()
    retval = listen(listen_sock,SOMAXCONN);
    if(retval == SOCKET_ERROR) err_quit("listen()");

    //데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int len; //고정길이 데이터
    char buf[BUFSIZE + 1];

    while(1){
        addrlen=sizeof(clientaddr);
        client_sock= accept(listen_sock,(struct sockaddr *)&clientaddr, &addrlen);
        if(client_sock == INVALID_SOCKET){
            err_display("accept()");
            break;
        }

        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&clientaddr.sin_addr,addr,sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",addr,
        ntohs(clientaddr.sin_port));

        while(1){
            retval = recv(client_sock,(char *)&len, sizeof(int),MSG_WAITALL);
            if(retval == SOCKET_ERROR){
                err_display("recv()");
                break;
            }
            else if(retval==0)
                break;
            retval = recv(client_sock,buf,len,MSG_WAITALL);
            if(retval==SOCKET_ERROR){
                err_display("recv()");
                break;
            }
            else if(retval==0)
                break;
            
            buf[retval] = '\0';
            printf("[TCP/%s:%d %s",addr,ntohs(clientaddr.sin_port),buf);

        }
         close(client_sock);
        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n"
        ,addr,ntohs(clientaddr.sin_port));
    }
    close(listen_sock);
    return 0;





}