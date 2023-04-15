#include "../Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 50

int main(int argc, char *argv[]){
    int retval;

    if(argc > 1) SERVERIP = argv[1];

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    char buf[BUFSIZE];
    const char*testData[]={
        "오늘은 무엇을 하시나요","여전히 공부하죸ㅋ","수준급 개발자가 되야죠","맞아요 맞아요ㅋㅋ"
    };
    //문자열 리터럴은 자동으로 뒤에 널문자가 추가 됨
    int len;

    for(int i=0; i<4; i++){
        SOCKET sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock ==INVALID_SOCKET) err_quit("socket()");

        //connect()
        retval = connect(sock,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
        if(retval == SOCKET_ERROR) err_quit("connect()");
        
        len = (int)strlen(testData[i]);
        strncpy(buf,testData[i],len);

        retval = send(sock,buf,len,0);
        if(retval == SOCKET_ERROR){
            err_display("send()");
            break;
        }
        printf("[TCP 클라이언트] %d바이트를 보냈습니다. \n",retval);
        
        close(sock);
    }
    return 0;
}