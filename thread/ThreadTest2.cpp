#include <stdio.h>
#include <pthread.h>

int sum = 0;

void *MyThread(void* arg){
    // 함수형 포인터에 정수값 전달 가능
    int num = (int)(long long)arg;
    for (int i = 1; i < num; i++)
        sum += i;
    return 0;
}

int main(void){
    int num = 100;
    pthread_t tid;
    pthread_create(&tid, NULL, MyThread, (void *)(long long)num);

    printf("스레드 실행 전. 계산 결과 = %d\n",sum);
    pthread_join(tid,NULL);
    printf("스레드 실행 후. 계산 결과 = %d\n",sum);

    return 0;

}