#include<stdio.h>
#include<limits.h>
#include<locale.h>
#include "diff.h"
#include "max.h"
#include "min.h"
#include "sum.h"
#define INPUT_SIZE 10000
#define NOTHING -1
int arr[100];



int main(){
    char buffer[INPUT_SIZE];
    fgets(buffer, sizeof(buffer), stdin); char* data = buffer;
    int n = 0;
    int t = NOTHING; int number;
    int x;
    int diff;

    if (sscanf(data, "%d%n", &number, &x) == 1) {
        t = number;
        data += x;
        while (sscanf(data, " %d%n", &number, &x) == 1) {
            arr[n] = number;
            n++;
            data += x;}}

    switch (t) {
        case 0: {printf("%d\n", Max(arr, n)); break;}

        case 1: {printf("%d\n", Min(arr, n)); break;}

        case 2: {printf("%d\n",diff_between_max_and_min(arr, diff, n)); break;}

        case 3: {printf("%d\n", sum_before_min(arr, n)); break;}

        default:
            printf("Данные некорректны");}



    return 0;}






