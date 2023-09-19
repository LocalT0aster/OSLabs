#include<stdio.h>
int main()
{
    int num1 = __INT32_MAX__;
    unsigned short num2 = __UINT16_MAX__;
    long num3 = __INT64_MAX__;
    float num4 = __FLT_MAX__;
    double num5 = __DBL_MAX__;
    printf("integer               |SIZE %ld, VALUE: %d\n", sizeof(num1), num1);
    printf("unsigned short integer|SIZE %ld, VALUE: %u\n", sizeof(num2), num2);
    printf("signed long integer   |SIZE %ld, VALUE: %ld\n",sizeof(num3), num3);
    printf("float                 |SIZE %ld, VALUE: %f\n",sizeof(num4), num4);
    printf("double                |SIZE %ld, VALUE: %lf\n",sizeof(num5), num5);
}
