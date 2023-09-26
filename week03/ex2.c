#include<stdio.h>
#include<math.h>

struct Point {
    double x;
    double y;
};

double distance(struct Point* a, struct Point* b) {
    double x = a->x - b->x;
    double y = a->y - b->y;
    return sqrtl(x * x + y * y);
}

double dabs(double v) {
    return v > 0 ? v : -v;
}

double area(struct Point* a, struct Point* b, struct Point* c) {
    //     dabs( x1  *  y2  -  x2  *  y1  +  x2  *  y3  -  x3  *  y2  +  x3  *  y1  -  x1  *  y3 )/2
    return dabs(a->x * b->y - b->x * a->y + b->x * c->y - c->x * b->y + c->x * a->y - a->x * c->y)/2;
}

int main() {
    struct Point a = (struct Point){.x = 2.5, .y = 6.0};
    struct Point b = (struct Point){.x = 1.0, .y = 2.2};
    struct Point c = (struct Point){.x = 10.0, .y = 6.0};
    printf("Distance between A & B: %lf\nArea of triangle ABC:  %lf\n", distance(&a, &b), area(&a, &b, &c));
    return 0;
}
