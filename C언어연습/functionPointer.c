#include <stdio.h>

int sub(int a, int b){
    return b-a;
}
int repl(int(*avl)(int,int)){
    printf("%d",avl(12332,4));
    return 1;
};

int main(){
    int(*func)(int,int);
    func=sub;
    repl(func);
    void* vtr = func;
    repl((int(*)(int,int)) vtr);
}