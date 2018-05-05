#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
    // To generate 1 GB file Containing Digits
    long long int a, i, num;
    int end = -1;

    FILE* fptr;
    fptr = (fopen("number.txt", "w"));
    if (fptr == NULL) {
        printf("Error!");
    }
    for (i = 0; i < 89478486; i++)

    {
        num = rand();
        num = (num << 32) | rand();
        a = (num % (8999999999)) + 1000000000;

        fprintf(fptr, "%lld\n", a);
    }
    fprintf(fptr, "%d\n", end);

    fclose(fptr);
    return 0;
}
