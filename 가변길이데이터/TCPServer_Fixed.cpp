#include "../Common.h"

#define SERVERPORT 9000
#define BUFSIZE 50

int main(int argc, char* argv[]){

    int retval;

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM,0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock,(struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char buf[BUFSIZE + 1];

    while(1){
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr,&addrlen);
        if(client_sock == INVALID_SOCKET){
            err_display("accept()");
            break;
        }

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&clientaddr.sin_addr, addr,sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP주소=%s, 포트 번호=%d\n",addr,ntohs(clientaddr.sin_port));

        while (1){
            // 데이터 받기
            retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
            if (retval == SOCKET_ERROR){
                err_display("recv()");
                break; 
            }else if( retval == 0) break;

            // 받은 데이터 출력
            buf[retval] = '\0';
            printf("[TCP\%s:%d] %s\n",addr,ntohs(clientaddr.sin_port),buf);


        }
        close(client_sock);
        printf("[TCP 서버] 클라이언트 종료: IP 주소 =%s, 포트 번호=%d\n",addr,ntohs(clientaddr.sin_port));
    }
    close(client_sock);




}