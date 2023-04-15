#include <stdio.h>

struct Point {
    int x;
    int y;
};

int summation(int x, int y){
    return x + y;
}

struct Pos {
    struct Point p;
    int (*sum)(int, int);
};

void setPos(struct Pos* pos, int x, int y, int (*sum)(int, int)) {
    pos->p.x = x;
    pos->p.y = y;
    pos->sum = sum;
}
struct Point getPos(struct Pos* pos){
    return pos->p;

}

int main(){
    struct Pos pos;
    setPos(&pos, 5, 5, summation);
    struct Point p = getPos(&pos);
    p.x=1;
    p.y=2;
    struct Point c = getPos(&pos);
    printf("%d %d",c.x,c.y);


    printf("Sum of %d and %d is %d\n", pos.p.x, pos.p.y, pos.sum(pos.p.x, pos.p.y));

    return 0;
}